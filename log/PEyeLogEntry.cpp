/*
 * PEyeLogEntry.cpp
 *
 * This file is part of libeye and implements the eyelog entries.
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

#include <cassert>
#include <sstream>
#include <algorithm>
#include "PEyeLogEntry.h"
#include "TypeDefs.h"

struct PEntryPtrSortPredicate {
    bool operator()(const PEntryPtr l, const PEntryPtr r) {
        return l->compare(*r) < 0;
    }
};

/* ** utility functions * **/

PEntryVec copyPEntryVec(const PEntryVec& entries)
{
    PEntryVec out;
    out.reserve(entries.size());
    for (PEntryVec::size_type i = 0; i < entries.size(); i++)
        out.push_back(entries[i]->clone());
    return out;
}

void destroyPEntyVec(PEntryVec& entries)
{
    for (PEntryVec::size_type i = 0; i < entries.size(); i++)
        delete entries[i];
}

void sortPEntryVec(PEntryVec& vec)
{
    std::sort(vec.begin(), vec.end(), PEntryPtrSortPredicate());
}


/* ** PEyeLogEntry * **/

char PEyeLogEntry::m_sep = '\t';

unsigned PEyeLogEntry::m_precision = 2;

PEyeLogEntry::PEyeLogEntry(entrytype etype, double time)
    : m_type(etype), m_time(time)
{
}

void PEyeLogEntry::setSeparator(const String& sep) {
    m_sep = sep[0];
}

String PEyeLogEntry::getSeparator() {
    return String(m_sep);
}

void PEyeLogEntry::setPrecision(unsigned p) {
    if (p > 8)
        p = 8;
    m_precision = p;
}

unsigned PEyeLogEntry::getPrecision(){
    return m_precision;
}

entrytype PEyeLogEntry::getEntryType() const {
    return m_type;
}

double PEyeLogEntry::getTime()const {
    return m_time;
}

int PEyeLogEntry::writeBinary(std::ofstream& stream) const
{
    uint16_t type = getEntryType();
    double time = getTime();
    if (!stream.write(reinterpret_cast<const char*>(&type), sizeof(type)))
        return errno;
    if (!stream.write(reinterpret_cast<const char*>(&time), sizeof(time)))
        return errno;
    return 0;
}

#define HANDLE_DERIVED_COMPARE(entryclass)\
    {\
        const entryclass* self = static_cast<const entryclass*>(this);\
        return self->compare(*static_cast<const entryclass*>(&other));\
    }\

int PEyeLogEntry::compare(const PEyeLogEntry& other) const {
    int diff;
    double diff_float = getTime() - other.getTime();

    // allow difference of 1 nano second (getTime returns ms)
    if      (diff_float < -1e-6)
        return -1;
    else if (diff_float >  1e-6)
        return 1;

    diff = getEntryType() - other.getEntryType();
    if (diff)
        return diff;

    switch (getEntryType()) {
        case LGAZE:
        case RGAZE:
        case AVGGAZE:
            HANDLE_DERIVED_COMPARE(PGazeEntry)
        case LFIX:
        case RFIX:
        case AVGFIX:
             HANDLE_DERIVED_COMPARE(PFixationEntry)
        case LSAC:
        case RSAC:
        case AVGSAC:
             HANDLE_DERIVED_COMPARE(PSaccadeEntry)
//        case STIMULUS:
//             HANDLE_DERIVED_COMPARE(PStimulusEntry)
        case MESSAGE:
             HANDLE_DERIVED_COMPARE(PMessageEntry)
        case TRIAL:
             HANDLE_DERIVED_COMPARE(PTrialEntry)
        case TRIALSTART:
             HANDLE_DERIVED_COMPARE(PTrialStartEntry)
        case TRIALEND:
             HANDLE_DERIVED_COMPARE(PTrialEndEntry)
        default:
            assert(false); // unknown entry type.
            return -1;
    }
}
#undef HANDLE_DERIVED_COMPARE

bool PEyeLogEntry::operator<(const PEyeLogEntry& other) const
{
    return compare(other) < 0;
}

bool PEyeLogEntry::operator>(const PEyeLogEntry& other) const
{
    return compare(other) > 0;
}

bool PEyeLogEntry::operator==(const PEyeLogEntry& other) const
{
    return compare(other) == 0;
}

bool PEyeLogEntry::operator!=(const PEyeLogEntry& other) const
{
    return compare(other) != 0;
}

/*** PGazeEntry ***/

PGazeEntry::PGazeEntry(entrytype t,
                     double time,
                     float x,
                     float y,
                     float pupil
                     )
    : PEyeLogEntry(t, time),
      m_x(x),
      m_y(y),
      m_pupil(pupil)
{
    assert(t == LGAZE || t == RGAZE);
}

PGazeEntry::PGazeEntry(entrytype t, double time, const PCoordinate& c, float pupil)
    : PEyeLogEntry(t, time),
      m_x(c.getX()),
      m_y(c.getY()),
      m_pupil(pupil)

{
}

PGazeEntry::PGazeEntry(const PGazeEntry& other)
    : PEyeLogEntry(other.getEntryType(), other.getTime()),
      m_x(other.m_x),
      m_y(other.m_y),
      m_pupil(other.m_pupil)
{
}

PEyeLogEntry* PGazeEntry::clone() const
{
    return new PGazeEntry(*this);
}

String PGazeEntry::toString()const
{
    std::stringstream stream;
    stream.setf(std::ios::fixed);
    stream.precision(m_precision);
    const char sep = getSeparator()[0];

    stream << int(getEntryType()) << sep <<
        getTime() << sep <<
        m_x << sep <<
        m_y << sep <<
        m_pupil;
    return String(stream.str().c_str());
}

int PGazeEntry::writeBinary(std::ofstream& stream)const
{
    int ret;
    if ((ret = PEyeLogEntry::writeBinary(stream)) != 0)
        return ret;
    if (!stream.write(reinterpret_cast<const char*>(&m_x), sizeof(m_x)))
        return errno;
    if (!stream.write(reinterpret_cast<const char*>(&m_y), sizeof(m_y)))
        return errno;
    if (!stream.write(reinterpret_cast<const char*>(&m_pupil), sizeof(m_pupil)))
        return errno;
    return ret;
}

int PGazeEntry::compare(const PGazeEntry& other) const
{
    float diff;
    diff = m_x - other.m_x;
    if (bool(diff))
        return diff < 0 ? -1 : 1;
    diff = m_y - other.m_y;
    if (bool(diff))
        return diff < 0 ? -1 : 1;
    diff = m_pupil - other.m_pupil;
    if (bool(diff))
        return diff < 0 ? -1 : 1;
    return 0;
}

float PGazeEntry::getX() const
{
    return m_x;
}

float PGazeEntry::getY() const
{
    return m_y;
}

PCoordinate PGazeEntry::getCoordinate() const
{
    return PCoordinate (m_x, m_y);
}

void PGazeEntry::setX(float x) 
{
    m_x = x;
}

void PGazeEntry::setY(float y)
{
    m_y = y;
}

void PGazeEntry::setCoordinate(const PCoordinate& c)
{
    m_x = c.getX();
    m_y = c.getY(); 
}

float PGazeEntry::getPupil()const
{
    return m_pupil;
}

void PGazeEntry::setPupil(float size)
{
    m_pupil = size;
}

PFixationEntry::PFixationEntry(entrytype t,
                             double time,
                             double duration,
                             float x,
                             float y
                             )
    : PEyeLogEntry(t, time),
      m_dur(duration),
      m_x(x),
      m_y(y)
{
    assert(t == LFIX || t == RFIX);
}

PFixationEntry::PFixationEntry(const PFixationEntry& other)
    : PEyeLogEntry(other.getEntryType(), other.getTime()),
      m_dur(other.m_dur),
      m_x(other.m_x),
      m_y(other.m_y)
{
}

PEyeLogEntry* PFixationEntry::clone()const
{
    return new PFixationEntry(*this);
}

String PFixationEntry::toString() const
{
    std::stringstream stream;
    stream.setf(std::ios::fixed);
    stream.precision(m_precision);
    const char sep = getSeparator()[0];

    stream << int(getEntryType()) << sep <<
              getTime() << sep <<
              m_dur << sep <<
              m_x << sep <<
              m_y;

    return String(stream.str().c_str());
}

int PFixationEntry::writeBinary(std::ofstream& stream) const
{
    int ret;
    if ((ret = PEyeLogEntry::writeBinary(stream)) != 0)
        return ret;
    if (!stream.write(reinterpret_cast<const char*>(&m_dur), sizeof(m_dur)))
        return errno;
    if (!stream.write(reinterpret_cast<const char*>(&m_x), sizeof(m_x)))
        return errno;
    if (!stream.write(reinterpret_cast<const char*>(&m_y), sizeof(m_y)))
        return errno;
            
    return ret;
}

int PFixationEntry::compare(const PFixationEntry& other)const
{
    float diff;
    diff = m_dur - other.m_dur;
    if (bool(diff))
        return diff < 0 ? -1 : 1;

    diff = m_x - other.m_x;
    if (bool(diff))
        return diff < 0 ? -1 : 1;
    
    diff = m_y - other.m_y;
    if (bool(diff))
        return diff < 0 ? -1 : 1;
    return 0;
}

float PFixationEntry::getX()const
{
    return m_x;
}

float PFixationEntry::getY()const
{
    return m_y;
}

PCoordinate PFixationEntry::getCoordinate()const
{
    return PCoordinate(m_x, m_y);
}

double PFixationEntry::getDuration() const
{
    return m_dur;
}

void PFixationEntry::setX(float x)
{
    m_x = x;
}

void PFixationEntry::setY(float y)
{
    m_y = y;
}

void PFixationEntry::setCoordinate(const PCoordinate& c)
{
    m_x = c.getX();
    m_y = c.getY();
}

void PFixationEntry::setDuration(double dur)
{
    m_dur = dur;
}

PMessageEntry::PMessageEntry(double time,
                             const String& msg
                             )
    : PEyeLogEntry(MESSAGE, time),
      m_message(msg)
{
}

PMessageEntry::PMessageEntry(const PMessageEntry& other)
    : PEyeLogEntry(other.getEntryType(), other.getTime()),
      m_message(other.m_message)
{
}

PEyeLogEntry* PMessageEntry::clone()const
{
    return new PMessageEntry(*this);
}

String PMessageEntry::toString()const
{
    std::stringstream stream;
    stream.setf(std::ios::fixed);
    stream.precision(m_precision);
    const char sep = getSeparator()[0];

    stream << int(getEntryType()) << sep <<
        getTime() << sep <<
        m_message.c_str();

    return String(stream.str().c_str());
}

int PMessageEntry::writeBinary(std::ofstream& stream) const
{
    int ret;
    uint32_t size = m_message.size();

    if ((ret = PEyeLogEntry::writeBinary(stream)) != 0)
        return ret;
    if (!stream.write(reinterpret_cast<const char*>(&size), sizeof(size)))
        return errno;
    if (!stream.write(m_message.c_str(), size))
        return errno;
            
    return ret;
}

int PMessageEntry::compare(const PMessageEntry& other)const
{
    if (m_message < other.m_message)
        return -1;
    else if(m_message > other.m_message)
        return 1;
    else
        return 0;
}

String PMessageEntry::getMessage() const
{
    return m_message;
}

void PMessageEntry::setMessage(const String& msg)
{
    m_message = msg;
}

PSaccadeEntry::PSaccadeEntry( entrytype t,
                              double time,
                              double duration,
                              float x1,
                              float y1,
                              float x2,
                              float y2
                              )
    : PEyeLogEntry(t, time),
      m_dur(duration),
      m_x1(x1),
      m_y1(y1),
      m_x2(x2),
      m_y2(y2)
{
    assert(t == LSAC || t == RSAC);
}

PSaccadeEntry::PSaccadeEntry(const PSaccadeEntry& other)
    : PEyeLogEntry(other.getEntryType(), other.getTime()),
      m_dur(other.m_dur),
      m_x1(other.m_x1),
      m_y1(other.m_y1),
      m_x2(other.m_x2),
      m_y2(other.m_y2)
{
}

PEyeLogEntry* PSaccadeEntry::clone()const
{
    return new PSaccadeEntry(*this);
}

String PSaccadeEntry::toString() const
{
    std::stringstream stream;
    stream.setf(std::ios::fixed);
    stream.precision(m_precision);
    const char sep = getSeparator()[0];

    stream << int(getEntryType()) << sep <<
              getTime() << sep <<
              m_dur<< sep <<
              m_x1 << sep <<
              m_y1 << sep <<
              m_x2 << sep <<
              m_y2;

    return String(stream.str().c_str());
}

int PSaccadeEntry::writeBinary(std::ofstream& stream) const
{
    int ret;

    if ((ret = PEyeLogEntry::writeBinary(stream)) != 0)
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

int PSaccadeEntry::compare(const PSaccadeEntry& other)const
{
    float diff;

    diff = m_dur - other.m_dur;
    if (bool(diff))
        return diff < 0 ? -1 : 1;

    diff = m_x1 - other.m_x1;
    if (bool(diff))
        return diff < 0 ? -1 : 1;

    diff = m_y1 - other.m_y1;
    if (bool(diff))
        return diff < 0 ? -1 : 1;

    diff = m_x2 - other.m_x2;
    if (bool(diff))
        return diff < 0 ? -1 : 1;

    diff = m_y2 - other.m_y2;
    if (bool(diff))
        return diff < 0 ? -1 : 1;
    return 0;
}

float PSaccadeEntry::getX1()const
{
    return m_x1;
}

float PSaccadeEntry::getY1()const
{
    return m_y1;
}

float PSaccadeEntry::getX2()const
{
    return m_x2;
}

float PSaccadeEntry::getY2()const
{
    return m_y2;
}

PCoordinate PSaccadeEntry::getCoordinate1() const
{
    return PCoordinate(m_x1, m_y1);
}

PCoordinate PSaccadeEntry::getCoordinate2() const
{
    return PCoordinate(m_x2, m_y2);
}

double PSaccadeEntry::getDuration() const
{
    return m_dur;
}

void PSaccadeEntry::setX1(float x)
{
    m_x1 = x;
}

void PSaccadeEntry::setY1(float y)
{
    m_y1 = y;
}

void PSaccadeEntry::setX2(float x)
{
    m_x2 = x;
}

void PSaccadeEntry::setY2(float y)
{
    m_y2 = y;
}

void PSaccadeEntry::setCoordinate1(const PCoordinate& c)
{
    m_x1 = c.getX();
    m_y1 = c.getY();
}

void PSaccadeEntry::setCoordinate2(const PCoordinate& c)
{
    m_x2 = c.getX();
    m_y2 = c.getY();
}

void PSaccadeEntry::setDuration(double time)
{
    m_dur = time;
}


PTrialEntry::PTrialEntry(double time,
                         const String& identifier,
                         const String& group
                         )
    :
        PEyeLogEntry(TRIAL, time),
        m_identifier(identifier),
        m_group(group)
{
}

PTrialEntry::PTrialEntry()
    :
        PEyeLogEntry(TRIAL, 0)
{
}

void PTrialEntry::setIdentifier(const String& identifier)
{
    m_identifier = identifier;
}

void PTrialEntry::setGroup(const String& group)
{
    m_group = group;
}

String PTrialEntry::getIdentifier()const
{
    return m_identifier;
}

String PTrialEntry::getGroup()const
{
    return m_group;
}

String PTrialEntry::toString()const
{
    std::stringstream stream;
    stream.setf(std::ios::fixed);
    stream.precision(m_precision);
    const char sep = getSeparator()[0];

    stream << int(getEntryType()) << sep <<
              getTime()           << sep <<
              m_identifier.c_str()<< sep <<
              m_group.c_str();

    return String(stream.str().c_str());
}

PEntryPtr PTrialEntry::clone()const
{
    return new PTrialEntry(*this);
}

int PTrialEntry::writeBinary(std::ofstream& stream) const
{
    int ret;
    uint32_t identifier_size = m_identifier.size();
    uint32_t group_size = m_group.size();

    if ((ret = PEyeLogEntry::writeBinary(stream)) != 0)
        return ret;

    if (!stream.write( reinterpret_cast<const char*>(&identifier_size),
                       sizeof(identifier_size))
            )
        return errno;
    if (!stream.write(m_identifier.c_str(), identifier_size))
        return errno;
    if (!stream.write( reinterpret_cast<const char*>(&group_size),
                       sizeof(group_size))
            )
        return errno;
    if (!stream.write(m_group.c_str(), group_size))
        return errno;
            
    return ret;
}

int PTrialEntry::compare(const PTrialEntry& other) const
{
    if (m_identifier < other.m_identifier)
        return -1;
    else if (m_identifier > other.m_identifier)
        return 1;

    if (m_group < other.m_group)
        return -1;
    else if(m_group > other.m_group)
        return 1;
    else
        return 0;
}

PTrialStartEntry::PTrialStartEntry(double time)
    : PEyeLogEntry(TRIALSTART, time)
{
}

String PTrialStartEntry::toString() const
{
    std::stringstream stream;
    stream.setf(std::ios::fixed);
    stream.precision(m_precision);
    const char sep = getSeparator()[0];

    stream << int(getEntryType()) << sep <<
              getTime();

    return String(stream.str().c_str());
}

int PTrialStartEntry::compare(const PTrialStartEntry& other) const
{
    (void) other;
    return 0;
}

PEntryPtr PTrialStartEntry::clone() const
{
    return new PTrialStartEntry(*this);
}

PTrialEndEntry::PTrialEndEntry(double time)
    : PEyeLogEntry(TRIALEND, time)
{
}

String PTrialEndEntry::toString() const
{
    std::stringstream stream;
    stream.setf(std::ios::fixed);
    stream.precision(m_precision);
    const char sep = getSeparator()[0];

    stream << int(getEntryType()) << sep <<
              getTime();

    return String(stream.str().c_str());
}

int PTrialEndEntry::compare(const PTrialEndEntry& other) const
{
    (void) other;
    return 0;
}

PEntryPtr PTrialEndEntry::clone() const
{
    return new PTrialEndEntry(*this);
}
