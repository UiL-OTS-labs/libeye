/*
 * EyeLog.h
 *
 * Provides Logging for Eyetrackers.
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


#ifndef EYE_LOG_H
#define EYE_LOG_H

#include <string>
#include <vector>
#include <fstream>
#include <memory>
//#include "EyeEvent.h"

enum entrytype {
    LGAZE,
    RGAZE,
    LFIX,
    RFIX,
    STIMULUS,
    MESSAGE,
    LSAC,
    RSAC
};

class EyeLog;
class EyeLogEntry;

typedef std::shared_ptr<EyeLogEntry> entryptr;

/**
 * readLog opens a logfile
 *
 * readLog opens a logfile and determines its contents.
 * tries to read the binary format first, if that fails
 * @param out, will be initialized.
 * @param filename, the file to open.
 */
int readLog(EyeLog* out, const std::string& filename);

class EyeLogEntry {

public :

    EyeLogEntry(entrytype etype, double eyetrktime);

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
    static int getPrecision();

protected :

    /**
     * Obtains the type of item.
     */
    entrytype       getEntryType()  const;

    /**
     *
     */
    double          getTime() const;

private :

    entrytype       m_type;     // the type of message
    double          m_time;     // time on eyetracker.

protected :
    static std::string m_sep;        // separates field in the output.
    static unsigned    m_precision;  // uses as precision in the output.
};

class GazeEntry : public EyeLogEntry {

public :

    GazeEntry(entrytype t, double time, float x, float y, float pupil);

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

private :

    float m_x;
    float m_y;
    float m_pupil;
};

/**
 * FixationEntry contains the parameters about a fixation.
 *
 * A fixation is best described by a start time, a duration
 * and its location.
 */
class FixationEntry : public EyeLogEntry {

public:

    //FixationEntry (const Fixation& fix);
    FixationEntry (entrytype e,
                   double time,
                   double duration,
                   float x,
                   float y
            );

    virtual std::string toString() const;
    
    virtual int writeBinary(std::ofstream& stream) const;

private:

    double  m_dur;     // the duration of the fixation
    float   m_x;       
    float   m_y;
};

/**
 * This allows for logging custom messages
 */
class MessageEntry : public EyeLogEntry {

public :
    MessageEntry (double eyetime, const std::string& Message);

    virtual std::string toString() const;
    
    virtual int writeBinary(std::ofstream& stream) const;

private:

    std::string  m_message;
};

class SaccadeEntry : public EyeLogEntry {

public:

    //FixationEntry (const Fixation& fix);
    SaccadeEntry (entrytype e,
                  double time,
                  double duration,
                  double x1,
                  double y1,
                  double x2,
                  double y2
                  );

    virtual std::string toString() const;
    
    virtual int writeBinary(std::ofstream& stream) const;

private:

    double  m_dur;     // the duration of the saccade
    float   m_x1;
    float   m_y1;
    float   m_x2;
    float   m_y2;
};


/**
 * EyeLog is a utility to log events. Logged times are in milliseconds
 * Logged events start with their type sep zeptime sep eyetrackertime
 * It is possible to generate a Eyelink compatible logfile.
 */
class EyeLog {

public :

    enum format {BINARY, CSV /*,EYELINK_ASC*/};

    EyeLog();
    ~EyeLog();

    /**
     * open the log file
     * 
     * @return 0 if succesfull, -1 if not.
     */
    int open(const std::string& fname);

    /**
     * close the log file
     */
    void close();

    /**
     * clears all contents.
     */
    void clear();

    /**
     * reserves a number 
     */
    void reserve(unsigned size);

    /**
     * Add's an entry to this log
     */
    void addEntry(entryptr entry);

    /**
     * writes the file in a binary format.
     *
     * @return returns 0 when succesfull or an value from errno.h when not.
     */
    int write(format f=BINARY)const;

    /**
     * Checks whether the file is open.
     */
    bool isOpen() const; 

    /**
     * Returns the filename. You still need to check
     * whether the file is open.
     */
    std::string getFilename() const;

    const std::vector<entryptr>& getEntries()const;

private:

    std::vector<entryptr>   m_entries;

    mutable std::ofstream   m_file;

    std::string             m_filename;
    bool                    m_isopen;
    bool                    m_writebinary;
};

#endif
