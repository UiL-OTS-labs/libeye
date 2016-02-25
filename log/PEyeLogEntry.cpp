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

#include<cassert>
#include<sstream>
#include"PEyeLogEntry.h"

using namespace std;


/*** PEyeLogEntry ***/

string PEyeLogEntry::m_sep = "\t";

unsigned PEyeLogEntry::m_precision = 2;

PEyeLogEntry::PEyeLogEntry(entrytype etype, double time)
    : m_type(etype), m_time(time)
{
}

void PEyeLogEntry::setSeparator(const std::string& sep) {
    m_sep = sep;
}

string PEyeLogEntry::getSeparator() {
    return m_sep;
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

int PEyeLogEntry::writeBinary(ofstream& stream) const
{
    uint16_t type = getEntryType();
    double time = getTime();
    if (!stream.write(reinterpret_cast<const char*>(&type), sizeof(type)))
        return errno;
    if (!stream.write(reinterpret_cast<const char*>(&time), sizeof(time)))
        return errno;
    return 0;
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

string PGazeEntry::toString()const
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

int PGazeEntry::writeBinary(ofstream& stream)const
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

string PFixationEntry::toString() const
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

int PFixationEntry::writeBinary(ofstream& stream) const
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
                           const string& msg
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

string PMessageEntry::toString()const
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

int PMessageEntry::writeBinary(ofstream& stream) const
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

string PMessageEntry::getMessage() const
{
    return m_message;
}

void PMessageEntry::setMessage(const string& msg)
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

string PSaccadeEntry::toString() const
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

int PSaccadeEntry::writeBinary(ofstream& stream) const
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

