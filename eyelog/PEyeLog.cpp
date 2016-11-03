/*
 * PEyeLog.cpp
 *
 * This file is part of libeye and responsible for logging experiments
 *
 * Copyright (C) 2016  Maarten Duijndam
 * 
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 * 
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include "PEyeLog.h"
#include "cError.h"
#include "TypeDefs.h"
#include <cassert>
#include <cerrno>
#include <sstream>
#include <cctype>
#include <iostream>


using namespace std;

/* reading of binary gaze entries */

int readBinaryGaze(ifstream& stream, PEyeLogEntry** out, entrytype et) {
    double time;
    float x, y, pupil;
    
    assert(et == LGAZE || et == RGAZE);
    assert(*out == nullptr);

    if (!stream.read(reinterpret_cast<char*>(&time), sizeof(time)))
        return errno;
    if (!stream.read(reinterpret_cast<char*>(&x), sizeof(x)))
        return errno;
    if (!stream.read(reinterpret_cast<char*>(&y), sizeof(y)))
        return errno;
    if (!stream.read(reinterpret_cast<char*>(&pupil), sizeof(pupil)))
        return errno;

    *out = new PGazeEntry(et, time, x, y, pupil);
    return 0;
}

int readBinaryFix(ifstream& stream, PEyeLogEntry** out, entrytype et) {
    double time;
    double duration;
    float x, y;
    
    assert(et == LFIX || et == RFIX);
    assert(*out == NULL);

    if (!stream.read( (char*)&time      , sizeof(time)))
        return errno;
    if (!stream.read( (char*)&duration  , sizeof(duration)))
        return errno;
    if (!stream.read( (char*)&x         , sizeof(x)))
        return errno;
    if (!stream.read( (char*)&y         , sizeof(y)))
        return errno;

    *out = new PFixationEntry(et, time, duration, x, y);

    return 0;
}

int readBinaryMessage(ifstream& stream, PEyeLogEntry** out) {
    double time;
    uint32_t s;
    String::size_type size;
    String msg;

    assert(*out == nullptr);
    
    if (!stream.read( (char*)&time  , sizeof(time)))
        return errno;
    if (!stream.read( (char*)&s , sizeof(s)))
        return errno;
    size = String::size_type(s);
    msg.resize(size);
    if (!stream.read(&msg[0], size))
        return errno;

    *out = new PMessageEntry(time, msg);

    return 0;
}

int readBinarySac(ifstream& stream, PEyeLogEntry** out, entrytype et) {
    double time;
    double duration;
    float x1, y1, x2, y2;
    
    assert(et == LSAC || et == RSAC);

    if (!stream.read( (char*)&time      , sizeof(time)))
        return errno;
    if (!stream.read( (char*)&duration  , sizeof(duration)))
        return errno;
    if (!stream.read( (char*)&x1        , sizeof(x1)))
        return errno;
    if (!stream.read( (char*)&y1        , sizeof(y1)))
        return errno;
    if (!stream.read( (char*)&x2        , sizeof(x2)))
        return errno;
    if (!stream.read( (char*)&y2        , sizeof(y2)))
        return errno;

    *out = new PSaccadeEntry(et, time, duration, x1, y1, x2, y2);

    return 0;
}

PEyeLog::PEyeLog()
    : m_filename(),
      m_isopen(false)
{
    this->clear();
}

PEyeLog::~PEyeLog()
{
    clear();
}

int PEyeLog::open(const String& fname)
{
    m_filename = fname;
    m_file.open(fname.c_str(), fstream::binary|fstream::out);
    if (!m_file.is_open())
        return errno;
    return 0;
}

void PEyeLog::close()
{
    m_file.close();
    m_filename = "";
}

void PEyeLog::clear()
{
    destroyPEntyVec(m_entries);
    m_entries.clear();
}

void PEyeLog::reserve(unsigned size)
{
    if (size > m_entries.size())
        m_entries.reserve(size);
}

void PEyeLog::addEntry(PEyeLogEntry* p)
{
    m_entries.push_back(p);
}

void PEyeLog::setEntries(const PEntryVec& entries, bool empty)
{
    if (empty)
        clear();

    m_entries.reserve(entries.size() + m_entries.size());
    for (const auto* entry : entries)
        m_entries.push_back(entry->clone());
}

int PEyeLog::read(const String& file, bool clear_content)
{
    if (clear_content)
        clear();
    
    return readLog(this, file);
}

int PEyeLog::write(eyelog_format f) const
{
    int ret = 0;
    if (f == FORMAT_BINARY) {
        for (const auto& entry : m_entries) {
            ret = entry->writeBinary(m_file);
            if (ret)
                return ret;
        }
    }
    else if (f == FORMAT_CSV) {
        for (unsigned i = 0; i < m_entries.size(); ++i) {
            String line;
            // only last line is without lineterminator.
            if (i == m_entries.size() -1) 
                line = m_entries[i]->toString();
            else
                line = m_entries[i]->toString() + '\n';

            if (!(m_file << line.c_str())) {
                return errno;
            }
        }
    }
    else {
        ret = ERR_INVALID_PARAMETER;
    }
    return ret;
}

bool PEyeLog::isOpen() const
{
    return m_file.is_open();
}

const char* PEyeLog::getFilename()const
{
    return m_filename.c_str();
}

const DArray<PEyeLogEntry*>& PEyeLog::getEntries()const
{
    return m_entries;
}

/* *******
 * Implementation of functions that load a PEyeLog from disk.
 */

/**
 * returns the contents of a file in a String
 */
String readStreamAsString(ifstream& s)
{
    String ret;
    assert(s.tellg() == ifstream::pos_type(0));
    s.seekg(0, s.end);
    streampos size = s.tellg();
    s.seekg(0);
    ret.resize(string::size_type(size));
    s.read(&ret[0], ret.size());
    return std::move(ret);
}

/**
 * returns a DArray of Strings.
 */
DArray<String> getLines(ifstream& stream)
{
    DArray<String> output;
    string buffer;

    assert(stream.good());

    while (getline(stream, buffer,'\n'))
        output.push_back(String(buffer.c_str()));
    return output;
}

bool is_a_digit(const String& token)
{
    for (int c: token) {
        if (c < '0' || c > '9')
            return false;
    }
    return true;
}

// deprecated prefer the overload with String instead.
bool is_a_digit(const std::string& token)
{
    for (int c: token) {
        if (c < '0' || c > '9')
            return false;
    }
    return true;
}

int readAscManual(std::ifstream& stream, PEyeLog* plog)
{
    DArray<String> lines = getLines(stream);
    bool isleft = false;
    DArray<PEyeLogEntry*>::size_type startsize = plog->getEntries().size();

    // loops over all lines ignoring those values it doesn't understand
    for (const auto& line : lines) {
        istringstream stream(line.c_str());
        string token;

        stream >> token;

        if (is_a_digit(token)) { // either bi or monocular sample
            float x1, y1, p1=0, x2, y2, p2=0;
            double time = atof(token.c_str());
            string leftover;
            std::getline(stream, leftover);
            int matched = sscanf(leftover.c_str(),
                    "%f%f%f%f%f%f",
                    &x1, &y1, &p1, &x2, &y2, &p2
                    );
            if (matched >= 3 && matched < 6) { // monocular sample
                plog->addEntry(
                        
                        new PGazeEntry( isleft ? LGAZE : RGAZE,
                                       time,
                                       x1,
                                       y1,
                                       p1
                                       )
                        );
            }
            else if (matched == 6) { // binocular sample
                plog->addEntry(
                        new PGazeEntry( LGAZE,
                                       time,
                                       x1,
                                       y1,
                                       p1
                                       )
                        );
                                           
                plog->addEntry(
                        new PGazeEntry( RGAZE,
                                       time,
                                       x2,
                                       y2,
                                       p2
                                       )
                        );
            }
        }
        else if (token == "EFIX") {
            string c;
			double tstart, tend, dur;
		    float x, y; 
            if (stream >> c >> tstart >> tend >> dur >> x >> y) {
                plog->addEntry(
                        new PFixationEntry(c == "L" ? LFIX : RFIX,
                                          tstart,
                                          dur,
                                          x,
                                          y
                                          )
                        );
            }
        }
        else if (token == "MSG") {
            double time;
            string msg;
            string leftover;
            if (! (stream >> time) )
                continue;
            std::getline(stream, leftover);
            
            // remove leading whitespace
            string::iterator it;
            for (it = leftover.begin(); it < leftover.end(); it++)
                if (! std::isspace(*it))
                    break;
            msg = string(it, leftover.end());

            while(msg.size() > 0 && isspace(msg[msg.size()-1]) )//rm trailing whitespace
                msg.resize(msg.size()-1);

            plog->addEntry(
                    new PMessageEntry(
                        time,
                        String(&msg[0], &msg[0] + msg.size())
                        )
                    );

        }
        else if (token == "SAMPLES") {
            string gaze, leftorright;
            if (stream >> gaze >> leftorright) {
                if (gaze != "GAZE")
                    continue;
                isleft = leftorright == "RIGHT" ? true : false;
            }
        }
    }
    return plog->getEntries().size() > startsize ? 0 : ERR_INVALID_FILE_FORMAT;
}

int readCsvFormat(std::ifstream& stream, PEyeLog* plog)
{
    int result = 0;
    bool noerror = true;
    while (stream && noerror) {
        double time, dur;
        float x1, y1, x2, y2, p;
        string msg;
        unsigned type;
        entrytype e;

        if(stream >> type)
            ;
        else if (stream.eof())
            break;
        else {
            return ERR_INVALID_FILE_FORMAT;
        }
        switch (e = entrytype(type)) {
            case LGAZE:
            case RGAZE:
                if (stream >> time >> x1 >> y1 >> p)
                    plog->addEntry(new PGazeEntry(e, time, x1, y1, p));
                else {
                    noerror = false;
                    return ERR_INVALID_FILE_FORMAT;
                }
                break;
            case LFIX:
            case RFIX:
                if (stream >> time >> dur >> x1 >> y1)
                    plog->addEntry(
                            new PFixationEntry(e, time, dur, x1, y1)
                            );
                else {
                    noerror = false;
                    return ERR_INVALID_FILE_FORMAT;
                }
                break;
            case STIMULUS:
                assert(1==0); // not implemented yet.
            case MESSAGE:
                if (stream >> time) {
                    char c;
                    // remove leading whitespace.
                    while (stream >> c) {
                        if (!std::isspace(c)) {
                            stream.unget();
                            break;
                        }
                    }
                    std::getline(stream, msg, '\n');
                    plog->addEntry(
                            new PMessageEntry(
                                time,
                                String(&msg[0], &msg[0]+msg.size())
                                )
                            );
                }
                else {
                    noerror = false;
                    return ERR_INVALID_FILE_FORMAT;
                }
                break;
            case LSAC:
            case RSAC:
                if (stream >> time >> dur >> x1 >> y1 >> x2 >> y2)
                    plog->addEntry(
                            new PSaccadeEntry(e, time, dur, x1, y1, x2, y2)
                            );
                else {
                    noerror = false;
                    return ERR_INVALID_FILE_FORMAT;
                }
                break;
            default:
                result = ERR_INVALID_FILE_FORMAT;
                noerror = false;
        };
    }
    return result;
}

int readBinary(std::ifstream& stream, PEyeLog* plog)
{
    int result = 0;
    assert(stream.is_open());
    
    while (stream) {
        uint16_t e;
        entrytype et;
        
        stream.read((char*)&e, sizeof(e));
        if (stream.eof())
            break;

        et = entrytype(e);
        PEyeLogEntry* p = nullptr;
        switch(et) {
            case LGAZE:
            case RGAZE:
                result = readBinaryGaze(stream, &p, et);
                break;
            case LFIX:
            case RFIX:
                 result = readBinaryFix(stream, &p, et);
                break;
            case MESSAGE:
                 result = readBinaryMessage(stream, &p);
                break;
            case LSAC:
            case RSAC:
                 result = readBinarySac(stream, &p, et);
                break;
            default:
                return -1;
        }
        
        if (result){
            plog->clear();
            return result;
        }

        if (p)
            plog->addEntry(p);
    }
    return result;
}

/**
 * writes a binary logfile.
 */
int writeBinary(std::ofstream& stream, const PEyeLog& l)
{
    int result = 0;

    auto& entries = l.getEntries();
    for (auto& e: entries) {
        e->writeBinary(stream);
        if (!stream.good()) {
            result = errno;
            break;
        }
    }
    return result;
}

int readLog(PEyeLog* out, const String& filename) {
    ifstream stream;
    int result; 
    stream.open(filename.c_str(), ios::in | ios::binary);

    if ( !stream.is_open() )
        return errno;

    // First we try to read as binary
    result = ::readBinary(stream, out);

    /*Clear file status try read in our CsvFormat*/
    if (result != 0) {
        stream.seekg(0);
        stream.clear();
        assert(stream.good());
        result = readCsvFormat(stream, out);
    }

    /*Clear file status try read in Eyelink EDF format*/
    if (result != 0) {
        stream.seekg(0);
        stream.clear();
        assert(stream.good());
        result = readAscManual(stream, out);
    }

    return result;
}
