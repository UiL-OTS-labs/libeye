/*
 * cEyeLog.cpp
 *
 * Provides C interface for the PEyeLog classes.
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


#include "PEyeLog.h"
#include "cEyeLog.h"
#include <cstring>
#include <string>

/*
* Silence MS VC++ about strdup not being part of ISO C++ it will remain valid POSIX C
*/
#if defined(MSDOS) || defined(OS2) || defined(WIN32) || defined(__CYGWIN__)
#  define strdup _strdup
#endif

void eyelog_entry_destroy(eyelog_entry* e)
{
    delete reinterpret_cast<PEyeLogEntry*>(e);
}

eyelog_entry* eyelog_entry_clone(const eyelog_entry* other)
{
    const PEyeLogEntry* o = reinterpret_cast<const PEyeLogEntry*>(other);
    return reinterpret_cast<eyelog_entry*>(o->clone());
}

char* eyelog_entry_to_string(const eyelog_entry* e)
{
    return strdup(reinterpret_cast<const PEyeLogEntry*>(e)->toString().c_str());
}

void eyelog_entry_set_separator(eyelog_entry*e, char c)
{
    std::string sep = "";
    sep += c;
    reinterpret_cast<PEyeLogEntry*>(e)->setSeparator(sep);
}

char eyelog_entry_get_separator(const eyelog_entry* e)
{
    return reinterpret_cast<const PEyeLogEntry*>(e)->getSeparator().c_str()[0];
}

void eyelog_entry_set_precision(eyelog_entry* e, unsigned prec)
{
    reinterpret_cast<PEyeLogEntry*>(e)->setPrecision(prec);
}

unsigned eyelog_entry_get_precision(const eyelog_entry* e)
{
    return reinterpret_cast<const PEyeLogEntry*>(e)->getPrecision();
}

gaze_entry* gaze_entry_new(entrytype e, double time, float x, float y, float pupsiz)
{
    gaze_entry* r = NULL;
    try {
        r = reinterpret_cast<gaze_entry*>(new PGazeEntry(e, time, x, y, pupsiz));
    } catch(...) {
    }
    return r;
}

char* gaze_entry_to_string(const gaze_entry* g)
{
    return strdup(reinterpret_cast<const PGazeEntry*>(g)->toString().c_str());
}

fixation_entry* fixation_entry_new(entrytype et,
                                   double t,
                                   double d,
                                   float x,
                                   float y
                                   )
{
    fixation_entry* ret = NULL;
    try {
        ret = reinterpret_cast<fixation_entry*>(
                new PFixationEntry(et, t, d, x, y)
                );
    } catch (...) {

    }
    return ret;
}

char* fixation_entry_to_string(const fixation_entry* f)
{
    return strdup(reinterpret_cast<const PFixationEntry*>(f)->toString().c_str());
}

message_entry* message_entry_new(double t, const char* msg)
{
    message_entry* r = NULL;
    try {
        r = reinterpret_cast<message_entry*>(new PMessageEntry(t, msg));
    } catch(...) {
    }
    return r;
}

char* message_entry_to_string(const message_entry* m)
{
    return strdup(reinterpret_cast<const PMessageEntry*>(m)->toString().c_str());
}

saccade_entry* saccade_entry_new(entrytype et, double t, double d, float x1, float y1,
                                 float x2, float y2)
{
    saccade_entry* r = NULL;
    try {
        r = reinterpret_cast<saccade_entry*>(
                new PSaccadeEntry(et, t, d, x1, y1, x2, y2)
                );
    } catch(...) {
    }
    return r;
}

char* saccade_entry_to_string(const saccade_entry *e)
{
    return strdup(reinterpret_cast<const PSaccadeEntry*>(e)->toString().c_str()); 
}

eye_log* eye_log_new()
{
    eye_log* log = nullptr;
    try {
        log = reinterpret_cast<eye_log*>(new PEyeLog);
    } catch (...) {
    }
    return log;
}

void eye_log_destroy(eye_log* log)
{
    delete reinterpret_cast<PEyeLog*>(log);
}

int eye_log_open(eye_log* log, const char* filename) 
{
    PEyeLog* l = reinterpret_cast<PEyeLog*>(log);
    return l->open(filename);
}

void eye_log_close(eye_log* log)
{
    reinterpret_cast<PEyeLog*>(log)->close();
}

void eye_log_clear(eye_log* log)
{
    reinterpret_cast<PEyeLog*>(log)->clear();
}

void eye_log_reserve(eye_log* log, unsigned size)
{
    reinterpret_cast<PEyeLog*>(log)->reserve(size);
}

void eye_log_add_entry(eye_log* log, eyelog_entry* entry)
{
    PEyeLog      *l = reinterpret_cast<PEyeLog*>(log);
    PEyeLogEntry *e = reinterpret_cast<PEyeLogEntry*>(entry);
    l->addEntry(e);
}

int eye_log_write(eye_log* log, eyelog_format f)
{
    return reinterpret_cast<PEyeLog*>(log)->write(f);
}

int eye_log_read(eye_log* log, const char* filename, bool clear)
{
    PEyeLog* l = reinterpret_cast<PEyeLog*>(log);
    return l->read(filename, clear);
}

bool eye_log_is_open(eye_log* log)
{
	return reinterpret_cast<PEyeLog*>(log)->isOpen();
}

const char* eye_log_get_filename(eye_log* log)
{
    return reinterpret_cast<PEyeLog*>(log)->getFilename();
}

void eye_log_get_entries(eye_log             *log,
                         eyelog_entry        ***entries,
                         unsigned            *size
                         )
{
    PEyeLog* l = reinterpret_cast<PEyeLog*>(log);
    const std::vector<PEyeLogEntry*> &le = l->getEntries();
    PEyeLogEntry** data = const_cast<PEyeLogEntry**>(le.data());
    *entries = reinterpret_cast<eyelog_entry**>(data);
    *size = le.size();
}
