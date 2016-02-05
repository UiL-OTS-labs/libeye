/*
 * EyeLog.cpp this file is part of libeye and responsible for logging experiments
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

#include "EyeLog.h"
#include <sstream>
#include <cassert>
#include <cerrno>
#include <iostream>


using namespace std;

/*** EyeLogEntry ***/

string EyeLogEntry::m_sep = "\t";

unsigned EyeLogEntry::m_precision = 2;

EyeLogEntry::EyeLogEntry(entrytype etype, double time)
    : m_type(etype), m_time(time)
{
}

void EyeLogEntry::setSeparator(const std::string& sep) {
    m_sep = sep;
}

string EyeLogEntry::getSeparator() {
    return m_sep;
}

void EyeLogEntry::setPrecision(unsigned p) {
    if (p > 8)
        p = 8;
    m_precision = p;
}

unsigned EyeLogEntry::getPrecision(){
    return m_precision;
}

entrytype EyeLogEntry::getEntryType() const {
    return m_type;
}

double EyeLogEntry::getTime()const {
    return m_time;
}

int EyeLogEntry::writeBinary(ofstream& stream) const
{
    uint16_t type = getEntryType();
    double time = getTime();
    if (!stream.write(reinterpret_cast<const char*>(&type), sizeof(type)))
        return errno;
    if (!stream.write(reinterpret_cast<const char*>(&time), sizeof(time)))
        return errno;
    return 0;
}

/*** GazeEntry ***/

GazeEntry::GazeEntry(entrytype t,
                     double time,
                     float x,
                     float y,
                     float pupil
                     )
    : EyeLogEntry(t, time),
      m_x(x),
      m_y(y),
      m_pupil(pupil)
{
    assert(t == LGAZE || t == RGAZE);
}

string GazeEntry::toString()const
{
    stringstream stream;
    stream.setf(ios::fixed);
    stream.precision(m_precision);
    const string sep(getSeparator());

    stream << int(getEntryType()) << sep <<
        getTime() << sep <<
        m_x << sep <<
        m_y << sep <<
        m_pupil;
    return stream.str();
}

int GazeEntry::writeBinary(ofstream& stream)const
{
    int ret;
    if ((ret = EyeLogEntry::writeBinary(stream)) != 0)
        return ret;
    if (!stream.write(reinterpret_cast<const char*>(&m_x), sizeof(m_x)))
        return errno;
    if (!stream.write(reinterpret_cast<const char*>(&m_y), sizeof(m_y)))
        return errno;
    if (!stream.write(reinterpret_cast<const char*>(&m_pupil), sizeof(m_pupil)))
        return errno;
    return ret;
}

int readBinaryGaze(ifstream& stream, EntryPtr& out, entrytype et) {
    double time;
    float x, y, pupil;
    
    assert(et == LGAZE || et == RGAZE);

    if (!stream.read(reinterpret_cast<char*>(&time), sizeof(time)))
        return errno;
    if (!stream.read(reinterpret_cast<char*>(&x), sizeof(x)))
        return errno;
    if (!stream.read(reinterpret_cast<char*>(&y), sizeof(y)))
        return errno;
    if (!stream.read(reinterpret_cast<char*>(&pupil), sizeof(pupil)))
        return errno;

    out = EntryPtr(new GazeEntry(et, time, x, y, pupil));
    return 0;
}

FixationEntry::FixationEntry(entrytype t,
                             double time,
                             double duration,
                             float x,
                             float y
                             )
    : EyeLogEntry(t, time),
      m_dur(duration),
      m_x(x),
      m_y(y)
{
    assert(t == LFIX || t == RFIX);
}

string FixationEntry::toString() const
{
    stringstream stream;
    stream.setf(ios::fixed);
    stream.precision(m_precision);
    const string sep(getSeparator());

    stream << int(getEntryType()) << sep <<
              getTime() << sep <<
              m_dur << sep <<
              m_x << sep <<
              m_y << sep;

    return stream.str();
}

int FixationEntry::writeBinary(ofstream& stream) const
{
    int ret;
    if ((ret = EyeLogEntry::writeBinary(stream)) != 0)
        return ret;
    if (!stream.write(reinterpret_cast<const char*>(&m_dur), sizeof(m_dur)))
        return errno;
    if (!stream.write(reinterpret_cast<const char*>(&m_x), sizeof(m_x)))
        return errno;
    if (!stream.write(reinterpret_cast<const char*>(&m_y), sizeof(m_y)))
        return errno;
            
    return ret;
}

int readBinaryFix(ifstream& stream, EntryPtr& out, entrytype et) {
    double time;
    double duration;
    float x, y;
    
    assert(et == LFIX || et == RFIX);

    if (!stream.read( (char*)&time      , sizeof(time)))
        return errno;
    if (!stream.read( (char*)&duration  , sizeof(duration)))
        return errno;
    if (!stream.read( (char*)&x         , sizeof(x)))
        return errno;
    if (!stream.read( (char*)&y         , sizeof(y)))
        return errno;

    out = EntryPtr(new FixationEntry(et, time, duration, x, y));

    return 0;
}

MessageEntry::MessageEntry(double time,
                           const string& msg
                           )
    : EyeLogEntry(MESSAGE, time),
      m_message(msg)
{
}

string MessageEntry::toString()const
{
    stringstream stream;
    stream.setf(ios::fixed);
    stream.precision(m_precision);
    const string sep(getSeparator());

    stream << int(getEntryType()) << sep <<
        getTime() << sep <<
        m_message;

    return stream.str();
}

int MessageEntry::writeBinary(ofstream& stream) const
{
    int ret;
    uint32_t size = m_message.size();

    if ((ret = EyeLogEntry::writeBinary(stream)) != 0)
        return ret;
    if (!stream.write(reinterpret_cast<const char*>(&size), sizeof(size)))
        return errno;
    if (!stream.write(m_message.c_str(), size))
        return errno;
            
    return ret;
}

int readBinaryMessage(ifstream& stream, EntryPtr& out) {
    double time;
    uint32_t size;
    string msg;
    
    if (!stream.read( (char*)&time  , sizeof(time)))
        return errno;
    if (!stream.read( (char*)&size  , sizeof(size)))
        return errno;
    msg.resize(size);
    if (!stream.read( (char*) msg.data(), size))
        return errno;

    out = EntryPtr(new MessageEntry(time, msg));

    return 0;
}


SaccadeEntry::SaccadeEntry( entrytype t,
                            double time,
                            double duration,
                            float x1,
                            float y1,
                            float x2,
                            float y2
                            )
    : EyeLogEntry(t, time),
      m_dur(duration),
      m_x1(x1),
      m_y1(y1),
      m_x2(x2),
      m_y2(y2)
{
    assert(t == LSAC || t == RSAC);
}

string SaccadeEntry::toString() const
{
    stringstream stream;
    stream.setf(ios::fixed);
    stream.precision(m_precision);
    const string sep(getSeparator());

    stream << int(getEntryType()) << sep <<
              getTime() << sep <<
              m_dur<< sep <<
              m_x1 << sep <<
              m_y1 << sep <<
              m_x2 << sep <<
              m_y2 << sep;

    return stream.str();
}

int SaccadeEntry::writeBinary(ofstream& stream) const
{
    int ret;

    if ((ret = EyeLogEntry::writeBinary(stream)) != 0)
        return ret;
    if (!stream.write(reinterpret_cast<const char*>(&m_dur), sizeof(m_dur)))
        return errno;
    if (!stream.write(reinterpret_cast<const char*>(&m_x1), sizeof(m_x1)))
        return errno;
    if (!stream.write(reinterpret_cast<const char*>(&m_y1), sizeof(m_y1)))
        return errno;
    if (!stream.write(reinterpret_cast<const char*>(&m_x2), sizeof(m_x2)))
        return errno;
    if (!stream.write(reinterpret_cast<const char*>(&m_y2), sizeof(m_y2)))
        return errno;

    return ret;
}

int readBinarySac(ifstream& stream, EntryPtr& out, entrytype et) {
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

    out = EntryPtr(new SaccadeEntry(et, time, duration, x1, y1, x2, y2));

    return 0;
}

EyeLog::EyeLog()
    : m_filename(),
      m_isopen(false)
{
}

EyeLog::~EyeLog()
{
}

int EyeLog::open(const string& fname)
{
    m_filename = fname;
    m_file.open(fname.c_str(), fstream::binary|fstream::out);
    if (!m_file.is_open())
        return errno;
    return 0;
}

void EyeLog::close()
{
    m_file.close();
    m_filename = "";
}

void EyeLog::clear()
{
    m_entries.clear();
}

void EyeLog::reserve(unsigned size)
{
    if (size > m_entries.size())
        m_entries.reserve(size);
}

void EyeLog::addEntry(EntryPtr p)
{
    m_entries.push_back(p);
}

int EyeLog::read(const string& file, bool clear_content) {
    if (clear_content)
        clear();
    
    return readLog(this, file);
}

int EyeLog::write(format f) const
{
    int ret = 0;
    for (const auto& entry : m_entries) {
        ret = entry->writeBinary(m_file);
        if (ret)
            return ret;
    }
    return ret;
}

bool EyeLog::isOpen() const
{
    return m_file.is_open();
}

string EyeLog::getFilename()const
{
    return m_filename;
}

const std::vector<EntryPtr>& EyeLog::getEntries() const
{
    return m_entries;
}

/********
 * Implementation of functions that load a EyeLog from disk.
 */

/**
 * returns the contents of a file in a string
 */
string readStreamAsString(ifstream& s)
{
    string ret;
    assert(s.tellg() == 0);
    s.seekg(0, s.end);
    streampos size = s.tellg();
    s.seekg(0);
    ret.resize(size);
    s.read(&ret[0], size);
    return ret;
}

/**
 * returns a vector of strings.
 */
vector<string> getLines(ifstream& stream)
{
    vector<string> output;
    string buffer;

    assert(stream.good());

    while (getline(stream, buffer,'\n'))
        output.push_back(buffer);
    return output;
}


//int readAscFormat(ifstream& stream, EyeLog* plog)
//{
//    int result = 0;
//    //vector<string> lines = getLines(stream);
//    assert(stream.is_open());
//    std::vector<string> content = getLines(stream);
//    plog->clear();
//    bool isleft(false);
//
//    // Regular expressiong for a float in a Asc file, not
//    // a general regular expression. Note the the float is
//    // grouped.
//    string cflt     = R"(([-+]?[0-9]*\.?[0-9]+))";
//    string space    = R"(\s+)";
//    string cdigit   = R"((\d+))";
//
//    string remsg    = R"(MSG\s+(\d+)\s+([^\r\n]*).*)";
//    string resampleformat = R"(^SAMPLES\s+GAZE\s+(\w+).+)";
//    string reduosample = cdigit + space +
//                         cflt   + space +
//                         cflt   + space +
//                         cflt   + space + 
//                         cflt   + space +
//                         cflt   + space +
//                         cflt   + ".*$";
//
//    string remonosample = cdigit + space +
//                          cflt   + space +
//                          cflt   + space +
//                          cflt   + ".*$";
//    
//    string reendfix = R"(^EFIX)" + space + R"((R|L))" + space +
//                               cdigit + space +
//                               cdigit + space +
//                               cdigit + space +
//                               cflt   + space +
//                               cflt   + space +
//                               cdigit + ".*";
//
//    //cout << "remsg = " << remsg << endl;
//    //cout << "resampleformat= " << resampleformat << endl;
//    //cout << "reduosample= " << reduosample << endl;
//    //cout << "remonosample= " << remonosample << endl;
//    //cout << "reendfix= " << reendfix << endl;
//
//    regex msg (remsg);
//    regex duosample (reduosample);
//    regex monosample (remonosample);
//    regex endfix (reendfix);
//    regex sampleformat (resampleformat);
//    
//    for (const auto& line : content) {
//        smatch m;
//
//        if ( regex_match(line, m, duosample) ) {
//            //cout << "found duo sample" << endl;
//            string time(m[1].first, m[1].second);
//            string xl(m[2].first, m[2].second);
//            string yl(m[3].first, m[3].second);
//            string pl(m[4].first, m[4].second);
//            string xr(m[5].first, m[5].second);
//            string yr(m[6].first, m[6].second);
//            string pr(m[7].first, m[7].second);
//            plog->addEntry( EntryPtr(
//                        new GazeEntry(LGAZE,
//                                      atof(time.c_str()),
//                                      atof(xl.c_str()),
//                                      atof(yl.c_str()),
//                                      atof(pl.c_str())
//                                      )
//                        )
//                    );
//            plog->addEntry( EntryPtr(
//                        new GazeEntry(RGAZE,
//                                      atof(time.c_str()),
//                                      atof(xr.c_str()),
//                                      atof(yr.c_str()),
//                                      atof(pr.c_str())
//                                      )
//                        )
//                    );
//            continue;
//        }
//        if ( regex_match(line, m, monosample) ) {
//            //cout << "found mono sample" << endl;
//            string time(m[1].first, m[1].second);
//            string x(m[2].first, m[2].second);
//            string y(m[3].first, m[3].second);
//            string p(m[4].first, m[4].second);
//            plog->addEntry(
//                    EntryPtr(
//                        new GazeEntry(isleft ? LGAZE : RGAZE,
//                                      atof(time.c_str()),
//                                      atof(x.c_str()),
//                                      atof(y.c_str()),
//                                      atof(p.c_str())
//                                      )
//                        )
//                    );
//            continue;
//        }
//        if (regex_match(line, m, endfix)) {
//            //cout << "found efix" << endl;
//            string eye(m[1].first, m[1].second);
//            string fixstart(m[2].first, m[2].second);
//            string duration(m[4].first, m[4].second);
//            string x (m[5].first, m[5].second);
//            string y (m[6].first, m[6].second);
//
//            plog->addEntry(
//                    EntryPtr(
//                        new FixationEntry( eye == "L" ? LFIX : RFIX,
//                                           atof(fixstart.c_str()),
//                                           atof(duration.c_str()),
//                                           atof(x.c_str()),
//                                           atof(y.c_str())
//                                           )
//                        )
//                    );
//
//            continue;
//        }
//        if ( regex_match(line, m, msg) ) {
//            //cout << "found msg" << endl;
//            string message (m[1].first, m[1].second);
//            string time(m[2].first, m[2].second);
//            double t = atof(time.c_str());
//            plog->addEntry( EntryPtr(new MessageEntry(t, message)) );
//            continue;                    
//        }
//        if ( regex_match(line, m, sampleformat) ){
//            //cout << "found sampleformat" << endl;
//            string lr (m[1].first, m[1].second);
//            std::transform (lr.begin(), lr.end(), lr.begin(), ::tolower);
//            isleft = lr == "right" ? false : true;
//            continue;
//        }
//    }
//    return result;
//}

bool is_a_digit(const string& token)
{
    for (int c: token) {
        if (c < '0' || c > '9')
            return false;
    }
    return true;
}

int readAscManual(std::ifstream& stream, EyeLog* plog)
{
    vector<string> lines = getLines(stream);
    bool isleft = false;

    // loops over all lines ignoring those values it doesn't understand
    for (const auto& line : lines) {
        istringstream stream(line);
        string token;

        stream >> token;

        if (is_a_digit(token)) { // either bi or monocular sample
            double x1, y1, p1=0, x2, y2, p2=0;
            double time = atof(token.c_str());
            string leftover;
            std::getline(stream, leftover);
            int matched = sscanf(leftover.c_str(),
                    "%lf%lf%lf%lf%lf%lf",
                    &x1, &y1, &p1, &x2, &y2, &p2
                    );
            if (matched >= 3 && matched < 6) { // monocular sample
                plog->addEntry(
                        EntryPtr(
                            new GazeEntry( isleft ? LGAZE : RGAZE,
                                           time,
                                           x1,
                                           y1,
                                           p1
                                )
                            )
                        );
            }
            else if (matched == 6) { // binocular sample
                plog->addEntry(
                        EntryPtr(
                            new GazeEntry( LGAZE,
                                           time,
                                           x1,
                                           y1,
                                           p1
                                )
                            )
                        );
                                           
                plog->addEntry(
                        EntryPtr(
                            new GazeEntry( RGAZE,
                                           time,
                                           x2,
                                           y2,
                                           p2
                                )
                            )
                        );
            }
        }
        else if (token == "EFIX") {
            string c;
            double tstart, tend, dur, x, y; 
            if (stream >> c >> tstart >> tend >> dur >> x >> y) {
                plog->addEntry(
                        EntryPtr (
                            new FixationEntry(c == "L" ? LFIX : RFIX,
                                              tstart,
                                              dur,
                                              x,
                                              y
                                )
                            )
                        );
            }
        }
        else if ( token == "MSG") {
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
                    EntryPtr(new MessageEntry(time, msg))
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
    return 0;
}

int readCsvFormat(std::ifstream& stream, EyeLog* plog)
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
            return 1;
        }
        switch (e = entrytype(type)) {
            case LGAZE:
            case RGAZE:
                if (stream >> time >> x1 >> y1 >> p)
                    plog->addEntry(
                            make_shared<GazeEntry>(e, time, x1, y1, p)
                            );
                else {
                    noerror = false;
                    return 1;
                }
                break;
            case LFIX:
            case RFIX:
                if (stream >> time >> dur >> x1 >> y1)
                    plog->addEntry(
                            make_shared<FixationEntry>(e, time, dur, x1, y1)
                            );
                else {
                    noerror = false;
                    return 1;
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
                            make_shared<MessageEntry>(time, msg)
                            );
                }
                else {
                    noerror = false;
                    return 1;
                }
                break;
            case LSAC:
            case RSAC:
                if (stream >> time >> dur >> x1 >> y1 >> x2 >> y2)
                    plog->addEntry(
                            make_shared<SaccadeEntry>(e, time, dur, x1, y1, x2, y2)
                            );
                else {
                    noerror = false;
                    return 1;
                }
                break;
            default:
                result = 1;
                noerror = false;
        };
    }
    return result;
}

int readBinary(std::ifstream& stream, EyeLog* plog)
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
        EntryPtr p;
        switch(et) {
            case LGAZE:
            case RGAZE:
                result = readBinaryGaze(stream, p, et);
                break;
            case LFIX:
            case RFIX:
                 result = readBinaryFix(stream, p, et);
                break;
            case MESSAGE:
                 result = readBinaryMessage(stream, p);
                break;
            case LSAC:
            case RSAC:
                 result = readBinarySac(stream, p, et);
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
int writeBinary(std::ofstream& stream, const EyeLog& l)
{
    int result = 0;

    auto entries = l.getEntries();
    for (const auto& e: entries) {
        e->writeBinary(stream);
        if (!stream.good()) {
            result = errno;
            break;
        }
    }
    return result;
}

int readLog(EyeLog* out, const string& filename) {
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

    /*Clear file status try read in Eyeling EDF format*/
    if (result != 0) {
        stream.seekg(0);
        stream.clear();
        assert(stream.good());
        result = readAscManual(stream, out);
    }

    return result;
}
