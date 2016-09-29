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
 * This file includes the PEyeLogEntry class. This is an abstract class
 * that has many concrete classes that derive from it. The derived
 * classes are all event that occur in an eyemomement experiment.
 * The thing they have all in common is an identifier that tell what kind
 * of event it is and a timestamp when it occurred in the experiment.
 */

#ifndef PEYELOGENTRY_H
#define PEYELOGENTRY_H

#include <fstream>
#include "TypeDefs.h"
#include "DArray.h"
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

typedef PEyeLogEntry* PEntryPtr;
typedef DArray<PEntryPtr> PEntryVec;

/**
 * Copy a DArray with PEntryPtr.
 *
 * This functionmakes a deep copy of the entries given as argument.
 * Since it is a deep copy, both the input and output vector
 * contain PEyeLogEntry 's that should eventually be deleted by the caller.
 *
 * @param[in] entries a PEntryVec
 *
 */
PEntryVec copyPEntryVec(const PEntryVec& entries);

/**
 * destroys the contents of a DArray with PEntryPtr, the DArray self 
 * remains intact.
 */
void destroyPEntyVec(PEntryVec& entries);

class PEyeLogEntry {

public :

    PEyeLogEntry(entrytype etype, double eyetrktime);
    virtual ~PEyeLogEntry(){};

    /**
     * \returns a clone of this class
     */
    virtual PEyeLogEntry* clone() const =0;

    /**
     * \returns a PEyeLogEntry in String form.
     */
    virtual String toString()const = 0;
    
    /**
     * Write this entry to a ofstream.
     *
     * Create a binary representation of this entry in a stream.
     * \param stream a opened stream.
     * \return 0 if succesful, or an int otherwise.
     */
    virtual int writeBinary(std::ofstream& stream)const;

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

    bool operator <  (const PEyeLogEntry& rhs)const;
    bool operator >  (const PEyeLogEntry& rhs)const;
    bool operator == (const PEyeLogEntry& rhs)const;
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
     * \note the separator may not be present in an String.
     */
    static void setSeparator(const String& c);

    /**
     * Get the current separator.
     */
    static String getSeparator();

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

    entrytype           m_type;     // the type of message
    double              m_time;     // time on eyetracker.

protected :
    static char         m_sep;      // separates field in the output.
    static unsigned     m_precision;// uses as precision in the output.
};

class PGazeEntry : public PEyeLogEntry {
    friend class PEyeLogEntry;

public :

    PGazeEntry(entrytype t, double time, float x, float y, float pupil);
    PGazeEntry(entrytype t, double time, const PCoordinate& c, float pupil);
    PGazeEntry(const PGazeEntry& gazeentry);

    PEyeLogEntry* clone() const;

    /**
     * @return a String that represents the entry type
     */
    virtual String toString() const;

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

    PEyeLogEntry* clone() const;
    
    virtual String toString() const;
    
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
    PMessageEntry (double eyetime, const String& Message);
    PMessageEntry (const PMessageEntry& messageentry);

    PEyeLogEntry* clone()const;

    virtual String toString() const;
    
    virtual int writeBinary(std::ofstream& stream) const;

    String getMessage()const;
    void setMessage(const String& message);

private:
    /**
     * this only compares the members in PMessageEntry
     */
    virtual int compare(const PMessageEntry& other)const;

    String  m_message;
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

    PEyeLogEntry* clone()const;

    virtual String toString() const;
    
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
                const String& identifier,
                const String& group
                );
    /**
     * Create an empty PTrialEntry
     */
    PTrialEntry();
    
    virtual String toString()const;
    virtual PEntryPtr clone()const;
    virtual int writeBinary(std::ofstream& stream)const;

    /**
     * Sets the trial identifier.
     *
     * Sets the trial identifier.
     * \param identifier an identifier.
     */
    void setIdentifier(const String& identifier);

    /**
     * Sets the trial group.
     *
     * Sets the trial group.
     * \param group a group.
     */
    void setGroup(const String& group);
    
    /**
     * get the trial identifier
     */
    String getIdentifier()const;
    
    /**
     * get the group identifier
     */
    String getGroup()const;

private:
    /**
     * This only compares members of the PTrialEntry
     */
    virtual int compare(const PTrialEntry& other) const;

    String m_identifier;
    String m_group;
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

    virtual String toString()const;
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
    
    virtual String toString()const;
    virtual PEntryPtr clone()const;
private:
    /**
     * returns 0.
     */
    virtual int compare(const PTrialEndEntry& other)const;
};

#endif

