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


#ifndef PEYE_LOG_H
#define PEYE_LOG_H

#include "TypeDefs.h"
#include "DArray.h"
#include <fstream>
#include "PEyeLogEntry.h"
#include "constants.h"

/**
 * readLog opens a logfile
 *
 * readLog opens a logfile and determines its contents.
 * tries to read the binary format first, if that fails
 * @param out, will be initialized.
 * @param filename, the file to open.
 */
EYELOG_EXPORT int  readLog(PEyeLog* out, const String& filename);


/**
 * PEyeLog is a utility to log events. Logged times are in milliseconds
 * Logged events start with their type sep zeptime sep eyetrackertime
 * It is possible to generate a Eyelink compatible logfile.
 * The eyelog will destroy all entries that are given to it, either
 * read from disk or via addEntrie(s).
 */
class EYELOG_EXPORT PEyeLog {

public :

    PEyeLog();
    ~PEyeLog();

    /**
     * open the log file
     * 
     * @return 0 if succesfull, -1 if not.
     */
    int open(const String& fname);

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
     * Adds an entry to this log.
     *
     * Add an entry to the log. Note the log takes ownership of the
     * log entry, there for you might want to call PEyeLogEntry::clone()
     * to pass this entry.
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
    int read(const String& filename, bool clear_content=true);

    /**
     * Checks whether the file is open.
     */
    bool isOpen() const; 

    /**
     * Returns the filename. You still need to check
     * whether the file is open.
     */
    const char* getFilename() const;

    /**
     * Gets all logentries.
     */
    const DArray<PEyeLogEntry*>& getEntries()const;

    /**
     * Sets the logentries and optionally clear the existing.
     *
     * Sets the entries, although the array is const, the 
     * entries are not, and the eyelog will own them
     * and delete them when it is ready with them. Thus
     * you might want to clone the entries prior to inserting them
     */
    void setEntries(const DArray<PEyeLogEntry*>& entries,
                    bool clear=true
                    );

private:

    PEyeLog(const PEyeLog&);
    PEyeLog& operator=(const PEyeLog&);

    DArray<PEyeLogEntry*>   m_entries;

    mutable std::ofstream   m_file;

    String                  m_filename;
    bool                    m_isopen;
    bool                    m_writebinary;
};

#endif
