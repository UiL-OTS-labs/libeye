/*
 * PEyeLogEntry.h
 *
 * Private header that provides entries to log Eyetrackers logs.
 *
 * Copyright (c) 2016 M.J.A. Duijndam.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, see Licenses at www.gnu.org.
 */

#ifndef PEYELOGENTRY_H
#define PEYELOGENTRY_H

#include <string>
#include <fstream>
#include "constants.h"
#include "PCoordinate.h"

/* Forward declaration to classes in this header. */
class PEyeLog;
class PEyeLogEntry;
class PGazeEntry;
class PFixationEntry;
class PMessageEntry;
class PSaccadeEntry;
class PStimulusEntry;

class PEyeLogEntry {

public :

    PEyeLogEntry(entrytype etype, double eyetrktime);
    virtual ~PEyeLogEntry(){};

    /**
     * \returns a clone of this class
     */
    virtual PEyeLogEntry* clone() const =0;

    virtual std::string toString()const = 0;
    virtual int writeBinary(std::ofstream& stream)const;

    static void setSeparator(const std::string& c);
    static std::string getSeparator();

    /**
     * sets precision used in the output(the number of decimals behind the dot).
     *
     * @param p a number between 0-8.
     */
    static void setPrecision(unsigned p);

    /**
     * \return the precision.
     */
    static unsigned getPrecision();

//protected :

    /**
     * Obtains the type of item.
     */
    entrytype       getEntryType()  const;

    /**
     * Returns the time when the entry was sampled.
     */
    double          getTime() const;

private :

    entrytype       m_type;     // the type of message
    double          m_time;     // time on eyetracker.

protected :
    static std::string m_sep;        // separates field in the output.
    static unsigned    m_precision;  // uses as precision in the output.
};

class PGazeEntry : public PEyeLogEntry {

public :

    PGazeEntry(entrytype t, double time, float x, float y, float pupil);
    PGazeEntry(entrytype t, double time, const PCoordinate& c, float pupil);
    PGazeEntry(const PGazeEntry& gazeentry);

    PEyeLogEntry* clone() const;

    /**
     * @return a string that represents the entry type
     */
    virtual std::string toString() const;

    /**
     * write an entry to and output stream.
     *
     * @param stream a opened ofstream for binary data.
     */
    virtual int writeBinary(std::ofstream& stream) const;

    /**
     * returns the x coordinate
     */
    float getX()const;
    
    /**
     * returns the y coordinate
     */
    float getY()const;

    /**
     * returns the coordinate
     */
    PCoordinate getCoordinate()const;
    
    /**
     * sets the x coordinate
     */
    void setX(float x);
    
    /**
     * set the y coordinate
     */
    void setY(float y);

    /**
     * set the y coordinate
     */
    void setCoordinate(const PCoordinate& c);

    /**
     * returns the pupilsize in the sample.
     */
    float getPupil()const;

    /**
     * sets the pupilsize in the sample.
     */
    void setPupil(float pupsize);

private :

    float m_x;
    float m_y;
    float m_pupil;
};

/**
 * PFixationEntry contains the parameters about a fixation.
 *
 * A fixation is best described by a start time, a duration
 * and its location.
 */
class PFixationEntry : public PEyeLogEntry {

public:

    //PFixationEntry (const Fixation& fix);
    PFixationEntry (entrytype e,
                   double time,
                   double duration,
                   float x,
                   float y
            );
    PFixationEntry(const PFixationEntry& fixentry);

    PEyeLogEntry* clone() const;
    
    virtual std::string toString() const;
    
    virtual int writeBinary(std::ofstream& stream) const;

    float getX()const;
    float getY()const;
    PCoordinate getCoordinate()const;
    double getDuration()const;

    void setX(float x);
    void setY(float y);
    void setCoordinate(const PCoordinate& c);
    void setDuration(double dur);

private:

    double  m_dur;     // the duration of the fixation
    float   m_x;       
    float   m_y;
};

/**
 * This allows for logging custom messages
 */
class PMessageEntry : public PEyeLogEntry {

public :
    PMessageEntry (double eyetime, const std::string& Message);
    PMessageEntry (const PMessageEntry& messageentry);

    PEyeLogEntry* clone()const;

    virtual std::string toString() const;
    
    virtual int writeBinary(std::ofstream& stream) const;

    std::string getMessage()const;
    void setMessage(const std::string& message);

private:

    std::string  m_message;
};

class PSaccadeEntry : public PEyeLogEntry {

public:

    //PSaccadeEntry
    PSaccadeEntry(entrytype e,
                  double time,
                  double duration,
                  float x1,
                  float y1,
                  float x2,
                  float y2
                  );

    PSaccadeEntry(const PSaccadeEntry& sacentry);

    PEyeLogEntry* clone()const;

    virtual std::string toString() const;
    
    virtual int writeBinary(std::ofstream& stream) const;

    float getX1()const;
    float getY1()const;
    float getX2()const;
    float getY2()const;
    PCoordinate getCoordinate1()const;
    PCoordinate getCoordinate2()const;
    double getDuration()const;

    void setX1(float x);
    void setY1(float y);
    void setX2(float x);
    void setY2(float y);
    void setCoordinate1(const PCoordinate& c);
    void setCoordinate2(const PCoordinate& c);
    void setDuration(double dur);

private:

    double  m_dur;     // the duration of the saccade
    float   m_x1;
    float   m_y1;
    float   m_x2;
    float   m_y2;
};



#endif

