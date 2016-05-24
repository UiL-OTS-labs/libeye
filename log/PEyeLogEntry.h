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

/**
 * \file PEyeLogEntry.h
 *
 * This file contains the implementation of all the logentries. The
 * file is part of the private section of libeye. It is not part of  
 * the public API of libeye.
 */

#ifndef PEYELOGENTRY_H
#define PEYELOGENTRY_H

#include <string>
#include <vector>
#include <fstream>
#include <memory>
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
class PTrialEntry;
class PTrialStartEntry;
class PTrialEndEntry;

/** typedef to simplify usage of std::shared_ptr<PEyeLogEntry> */
typedef std::shared_ptr<PEyeLogEntry>       PEntryPtr;
typedef std::shared_ptr<PGazeEntry>         PGazePtr;
typedef std::shared_ptr<PFixationEntry>     PFixPtr;
typedef std::shared_ptr<PMessageEntry>      PMsgPtr;
typedef std::shared_ptr<PSaccadeEntry>      PSacPtr;
typedef std::shared_ptr<PStimulusEntry>     PStimPtr;
typedef std::shared_ptr<PTrialEntry>        PTrialPtr;
typedef std::shared_ptr<PTrialStartEntry>   PTrialStartPtr;
typedef std::shared_ptr<PTrialEndEntry>     PTrialEndPtr;

/** typedef to simplify usage of std::vector<PEntryPtr> */
typedef std::vector<PEntryPtr> PEntryVec;


/**
 * A comparator used for sorting ranges of PEntryPtr
 */
class PEyeLogEntryComparator {
    bool operator ()(const PEntryPtr& entry1, const PEntryPtr& entry2);
};

/**
 * Sorts an entryvec
 */
void sortEntryVec(PEntryVec& entries);

/**
 * PEyeLogEntry is the parent class of all logentries.
 *
 * All logentries derive from PEyeLogEntry since all logentries
 * have a timestamp when they occur and that they have a certain type.
 */
class PEyeLogEntry {

protected:

    /**
     * Construct a new PEyeLogEntry.
     */
    PEyeLogEntry(entrytype etype, double eyetrktime);

public :
    
    /**
     * Destruct a PEyeLogEntry.
     */
    virtual ~PEyeLogEntry(){};

    /**
     * \returns a clone of this class
     */
    virtual PEntryPtr clone() const =0;

    /**
     * \returns a PEyeLogEntry in string form.
     */
    virtual std::string toString()const = 0;
    
    /**
     * Write this entry to a ofstream.
     *
     * Create a binary representation of this entry in a stream.
     * \param stream a opened stream.
     * \return 0 if succesful, or an int otherwise.
     */
    virtual int writeBinary(std::ofstream& stream) const;

    /**
     * compares this to another PEyeLogEntry
     *
     * Compares this entry to another entry. This
     * method sorts on time firstly, on entry type
     * secondly and if that doesn't resolve it
     * it calls the compare of an derived class.
     * The comparisons haven't allway got a true
     * mathematical meaning, but this function is 
     * mainly intended for sorting purposes.
     * 
     * \param other a PLogEntry to compare with.
     *
     * \return an integer smaller than, equal to or larger than
     * zero for respectively an item if the item
     * is smaller, equal to or larger than the other
     */
    virtual int compare(const PEyeLogEntry& other)const;

    /**
     * Checks whether this entry should occur before rhs in a log.
     */
    bool operator <  (const PEyeLogEntry& rhs)const;
   
    /**
     * Checks whether this entry should after before rhs in a log.
     */
    bool operator >  (const PEyeLogEntry& rhs)const;
    
    /**
     * Checks whether two PEyeLogEntry are identical
     */
    bool operator == (const PEyeLogEntry& rhs)const;
    /**
     * Checks whether two PEyeLogEntry are not identical
     */
    bool operator != (const PEyeLogEntry& rhs)const;

    // easy to implement but doesn't really make sense
    // because the only reason that these operators exist
    // is sorting and object equality.
    //bool operator <= (const PEyeLogEntry& rhs)const;
    //bool operator >= (const PEyeLogEntry& rhs)const;

    /**
     * set the separator between field in a csv log.
     * 
     * This sets the separator used between the fields in a csv log
     * \note the separator may not be present in an string.
     */
    static void setSeparator(const std::string& c);

    /**
     * Get the current separator.
     */
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

    /**
     * Obtains the type of item.
     */
    entrytype       getEntryType()  const;

    /**
     * Returns the time when the entry was sampled.
     */
    double          getTime() const;

private :

    entrytype       m_type;     ///< the type of message
    double          m_time;     ///< time on eyetracker.

protected :
    static std::string m_sep;        ///< separates field in the output.
    static unsigned    m_precision;  ///< uses as precision in the output.
};

/**
 * PGazeEntry is an implementation of gaze in a log.
 *
 * All logentries derive from PEyeLogEntry since all logentries
 * have a timestamp when they occur and that they have a certain type.
 */
class PGazeEntry : public PEyeLogEntry {

    friend class PEyeLogEntry;

public :

    /**
     * Construct a PGazeEntry
     *
     * \param t     The entrytype of the gaze must be LGAZE or RGAZE.
     * \param time  The time in ms of the gaze.
     * \param x     The x coordinate of the gaze.
     * \param y     The y coordinate of the gaze.
     * \param pupil The pupilsize of the sample unit is probably eyetracker
     *              dependent.
     */
    PGazeEntry(entrytype t, double time, float x, float y, float pupil);
   
    /**
     * Construct a PGazeEntry
     *
     * \param t     The entrytype of the gaze must be LGAZE or RGAZE.
     * \param time  The time in ms of the gaze.
     * \param c     The 2D coordinate of the sample.
     * \param pupil The pupilsize of the sample unit is probably eyetracker
     *              dependent.
     */
    PGazeEntry(entrytype t, double time, const PCoordinate& c, float pupil);
    
    /**
     * Construct a PGazeEntry
     *
     * copy constructor
     *
     * \param gazeentry another gazeentry
     */
    PGazeEntry(const PGazeEntry& gazeentry);

    /**
     * \returns a pointer to a new PEyeLogEntry
     */
    virtual PEntryPtr clone() const;

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
    /**
     * only compares the members in PGazeEntry
     */
    virtual int compare(const PGazeEntry& other)const;

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
    friend class PEyeLogEntry;

public:

    //PFixationEntry (const Fixation& fix);
    PFixationEntry (entrytype e,
                   double time,
                   double duration,
                   float x,
                   float y
            );
    PFixationEntry(const PFixationEntry& fixentry);

    virtual PEntryPtr clone() const;
    
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
    
    /**
     * this compares only the members in PFixationEntry
     */
    virtual int compare(const PFixationEntry& other) const;

    double  m_dur;     // the duration of the fixation
    float   m_x;       
    float   m_y;
};

/**
 * This allows for logging custom messages
 */
class PMessageEntry : public PEyeLogEntry {
    friend class PEyeLogEntry;

public :
    PMessageEntry (double eyetime, const std::string& Message);
    PMessageEntry (const PMessageEntry& messageentry);

    virtual PEntryPtr clone()const;

    virtual std::string toString() const;
    
    virtual int writeBinary(std::ofstream& stream) const;

    std::string getMessage()const;
    void setMessage(const std::string& message);

private:
    /**
     * this only compares the members in PMessageEntry
     */
    virtual int compare(const PMessageEntry& other)const;

    std::string  m_message;
};

class PSaccadeEntry : public PEyeLogEntry {
    friend class PEyeLogEntry;

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

    virtual PEntryPtr clone()const;

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
    
    /**
     * This only compares members of PSaccadeEntry
     */
    virtual int compare(const PSaccadeEntry& other)const;

    double  m_dur;     // the duration of the saccade
    float   m_x1;
    float   m_y1;
    float   m_x2;
    float   m_y2;
};

class PTrialEntry : public PEyeLogEntry
{
    friend class PEyeLogEntry;
public:
    /**
     * Create a PTrialEntry instance
     */
    PTrialEntry(double time,
                const std::string& identifier,
                const std::string& group
                );
    
    virtual std::string toString()const;
    virtual PEntryPtr clone()const;
    virtual int writeBinary(std::ofstream& stream)const;

    /**
     * Sets the trial identifier.
     *
     * Sets the trial identifier.
     * \param identifier an identifier.
     */
    void setIdentifier(const std::string& identifier);

    /**
     * Sets the trial group.
     *
     * Sets the trial group.
     * \param group a group.
     */
    void setGroup(const std::string& group);
    
    /**
     * get the trial identifier
     */
    std::string getIdentifier()const;
    
    /**
     * get the group identifier
     */
    std::string getGroup()const;

private:
    /**
     * This only compares members of the PTrialEntry
     */
    virtual int compare(const PTrialEntry& other) const;

    std::string m_identifier;
    std::string m_group;
};

/**
 * Marks a start of trial in a logfile
 */
class PTrialStartEntry : public PEyeLogEntry
{
    friend class PEyeLogEntry;
public:
    /**
     * Create a PTrialStartEntry instance
     */
    PTrialStartEntry(double time);

    virtual std::string toString()const;
    virtual PEntryPtr clone()const;
private:
    /**
     * returns 0 
     */
    virtual int compare(const PTrialStartEntry& other)const;
};

/**
 * Marks an end of trial in a logfile
 */
class PTrialEndEntry : public PEyeLogEntry
{
    friend class PEyeLogEntry;
public:
    /**
     * Create a PTrialEndEntry instance
     */
    PTrialEndEntry(double time);
    
    virtual std::string toString()const;
    virtual PEntryPtr clone()const;
private:
    /**
     * returns 0.
     */
    virtual int compare(const PTrialEndEntry& other)const;
};

#endif

