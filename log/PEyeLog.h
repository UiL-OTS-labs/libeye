/*
 * PEyeLog.h
 *
 * Private header that provides Logging for Eyetrackers.
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

//#include <log/eyelog_export.h>
#include "cEyeLog.h"


/* Forward declaration to classes in this header. */
class PEyeLog;
class PEyeLogEntry;
class PGazeEntry;
class PFixationEntry;
class PMessageEntry;
class PSaccadeEntry;
class PStimulusEntry;

///* Shared pointers to the classes, they are used a lot.*/
//typedef std::shared_ptr<PEyeLogEntry>    EntryPtr;
//typedef std::shared_ptr<PGazeEntry>      GazePtr;
//typedef std::shared_ptr<PFixationEntry>  FixPtr;
//typedef std::shared_ptr<PMessageEntry>   MsgPtr;
//typedef std::shared_ptr<PSaccadeEntry>   SacPtr;
//typedef std::shared_ptr<PStimulusEntry>  StimPtr;

//#if (defined(MSDOS) || defined(OS2) || defined(WIN32) || defined(__CYGWIN__)) && defined(BUILD_EYELOG_SHARED)
//	template class  std::basic_string<char>;
//	template class  std::vector<EntryPtr>;
//	template class  std::basic_ofstream<char>;
//#endif

/**
 * readLog opens a logfile
 *
 * readLog opens a logfile and determines its contents.
 * tries to read the binary format first, if that fails
 * @param out, will be initialized.
 * @param filename, the file to open.
 */
int  readLog(PEyeLog* out, const std::string& filename);

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
    PGazeEntry(const PGazeEntry& gazeentry);

    PEyeLogEntry* clone() const;

//    static GazePtr createShared(entrytype t,
//                                double time,
//                                float x,
//                                float y,
//                                float pupil
//                                )
//    {
//        return std::make_shared<PGazeEntry>(t, time, x, y, pupil);
//    }

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
    
//    static FixPtr createShared(entrytype e,
//                               double time,
//                               double dur,
//                               float x,
//                               float y
//                               )
//    {
//        return std::make_shared<PFixationEntry>(PFixationEntry(e, time, dur, x, y));
//    }

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
class PMessageEntry : public PEyeLogEntry {

public :
    PMessageEntry (double eyetime, const std::string& Message);
    PMessageEntry (const PMessageEntry& messageentry);

    PEyeLogEntry* clone()const;

//    static MsgPtr createShared(double eyetime, const std::string& Message)
//    {
//        return std::make_shared<PMessageEntry>(eyetime, Message);
//    }

    virtual std::string toString() const;
    
    virtual int writeBinary(std::ofstream& stream) const;

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



//    static SacPtr createShared(entrytype e,
//                               double time,
//                               double duration,
//                               float  x1,
//                               float  y1,
//                               float  x2,
//                               float  y2
//                               )
//    {
//        return std::make_shared<PSaccadeEntry>(e, time, duration, x1, y1, x2, y2);
//    }

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
 * PEyeLog is a utility to log events. Logged times are in milliseconds
 * Logged events start with their type sep zeptime sep eyetrackertime
 * It is possible to generate a Eyelink compatible logfile.
 */
class PEyeLog {

public :

    PEyeLog();
    ~PEyeLog();

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
     * Reserves a number of entries this can be
     * used to allocate space in advance and
     * is therefore an optimization if one knows the 
     * size upfront.
     *
     * @param size The number of entries to allocate space for.
     */
    void reserve(unsigned size);

    /**
     * Adds an entry to this log
     */
    void addEntry(PEyeLogEntry* entry);

    /**
     * writes the file in a binary format.
     *
     * @return returns 0 when succesfull or an value from errno.h when not.
     */
    int write(eyelog_format f=FORMAT_BINARY)const;

    /**
     * Opens file and reads the contents, before reading
     * The current content of this log is cleared.
     *
     * @param filename the name of the file to open.
     *
     * @return 0 or an error from errno (use eg strerror to examine).
     */
    int read(const std::string& filename, bool clear_content=true);

    /**
     * Checks whether the file is open.
     */
    bool isOpen() const; 

    /**
     * Returns the filename. You still need to check
     * whether the file is open.
     */
    const char* getFilename() const;

    const std::vector<PEyeLogEntry*>& getEntries()const;

private:

    std::vector<PEyeLogEntry*>   m_entries;

    mutable std::ofstream       m_file;

    std::string                 m_filename;
    bool                        m_isopen;
    bool                        m_writebinary;
};

#endif
