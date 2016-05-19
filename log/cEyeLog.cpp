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
#include "PCoordinate.h"
#include "cEyeLog.h"
#include <cstring>
#include <cassert>
#include <string>

using std::shared_ptr;

/*
 * Silence MS VC++ about strdup not being part of ISO C++ it will remain valid POSIX C
 */
#if defined(MSDOS) || defined(OS2) || defined(WIN32) || defined(__CYGWIN__)
#  define strdup _strdup
#endif

coordinate* coordinate_new()
{
    coordinate* c = NULL;
    try {
        PCoordinate* pc = new PCoordinate;
        c = (coordinate*) new shared_ptr<PCoordinate>(pc);
    } catch(...) {
    }
    return c;
}

coordinate* coordinate_new_xy(float x, float y)
{
    coordinate* c = NULL;
    try {
        PCoordinate* pc = new PCoordinate(x, y);
        c = reinterpret_cast<coordinate*>(new shared_ptr<PCoordinate>(pc));
    } catch(...) {
    }
    return c;
}

void coordinate_destroy(coordinate* coor)
{
    assert(coor);
    delete reinterpret_cast<shared_ptr<PCoordinate>*>(coor);
}

void coordinate_set_x(coordinate* coor, float x)
{
    assert(coor);
    (*reinterpret_cast<shared_ptr<PCoordinate>*>(coor))->setX(x);
}

void coordinate_set_y(coordinate* coor, float y)
{
    assert(coor);
    (*reinterpret_cast<shared_ptr<PCoordinate>*>(coor))->setY(y);
}

float coordinate_get_x(const coordinate* coor)
{
    assert(coor);
    return (*reinterpret_cast<const shared_ptr<const PCoordinate>*>(coor))->getX();
}

float coordinate_get_y(const coordinate* coor)
{
    assert(coor);
    return (*reinterpret_cast<const shared_ptr<const PCoordinate>*>(coor))->getY();
}

void eyelog_entry_destroy(eyelog_entry* e)
{
    delete reinterpret_cast<shared_ptr<PEyeLogEntry>*>(e);
}

eyelog_entry* eyelog_entry_clone(const eyelog_entry* other)
{
    assert(other);
    const PEntryPtr*    o   = reinterpret_cast<const PEntryPtr*>(other);
    PEntryPtr*          copy= new PEntryPtr((*o)->clone());
    eyelog_entry*       ret = reinterpret_cast<eyelog_entry*>(copy);
    return ret;
}

char* eyelog_entry_to_string(const eyelog_entry* e)
{
    assert(e);
    const PEntryPtr* entry = reinterpret_cast<const PEntryPtr*>(e);
    return strdup((*entry)->toString().c_str());
}

int eyelog_entry_compare(const eyelog_entry* e1, const eyelog_entry* e2)
{
    assert(e1 && e2);
    const PEntryPtr* entry1 = reinterpret_cast<const PEntryPtr*>(e1);
    const PEntryPtr* entry2 = reinterpret_cast<const PEntryPtr*>(e2);
    return (*entry1)->compare(**entry2);
}

int eyelog_entry_lt(const eyelog_entry* e1, const eyelog_entry* e2)
{
    assert(e1 && e2);
    const PEntryPtr* entry1 = reinterpret_cast<const PEntryPtr*>(e1);
    const PEntryPtr* entry2 = reinterpret_cast<const PEntryPtr*>(e2);
    return **entry1 < **entry2 ? 1 : 0;
}

int eyelog_entry_gt(const eyelog_entry* e1, const eyelog_entry* e2)
{
    assert(e1 && e2);
    const PEntryPtr* entry1 = reinterpret_cast<const PEntryPtr*>(e1);
    const PEntryPtr* entry2 = reinterpret_cast<const PEntryPtr*>(e2);
    return **entry1 > **entry2 ? 1 : 0;
}

int eyelog_entry_eq(const eyelog_entry* e1, const eyelog_entry* e2)
{
    assert(e1 && e2);
    const PEntryPtr* entry1 = reinterpret_cast<const PEntryPtr*>(e1);
    const PEntryPtr* entry2 = reinterpret_cast<const PEntryPtr*>(e2);
    return **entry1 == **entry2 ? 1 : 0;
}

int eyelog_entry_ne(const eyelog_entry* e1, const eyelog_entry* e2)
{
    assert(e1 && e2);
    const PEntryPtr* entry1 = reinterpret_cast<const PEntryPtr*>(e1);
    const PEntryPtr* entry2 = reinterpret_cast<const PEntryPtr*>(e2);
    return **entry1 != **entry2 ? 1 : 0;
}

void eyelog_entry_set_separator(eyelog_entry*e, char c)
{
    assert(e);
    std::string sep = "";
    sep += c;
    (*reinterpret_cast<PEntryPtr*>(e))->setSeparator(sep);
}

char eyelog_entry_get_separator(const eyelog_entry* e)
{
    assert(e);
    return (*reinterpret_cast<const PEntryPtr*>(e))->getSeparator().c_str()[0];
}

void eyelog_entry_set_precision(eyelog_entry* e, unsigned prec)
{
    assert(e);
    (*reinterpret_cast<PEntryPtr*>(e))->setPrecision(prec);
}

unsigned eyelog_entry_get_precision(const eyelog_entry* e)
{
    assert(e);
    return (*reinterpret_cast<const PEntryPtr*>(e))->getPrecision();
}

gaze_entry* gaze_entry_new(entrytype e, double time, float x, float y, float pupsiz)
{
    assert(e == LGAZE || e == RGAZE);
    gaze_entry* r = NULL;
    try {
        PGazeEntry* pe = new PGazeEntry(e, time, x, y, pupsiz);
        r = reinterpret_cast<gaze_entry*>(new PGazePtr(pe));
    } catch(...) {
    }
    return r;
}

float gaze_entry_get_x(const gaze_entry* g)
{
    assert(g);
    return (*reinterpret_cast<const PGazePtr*>(g))->getX();
}

float gaze_entry_get_y(const gaze_entry* g)
{
    assert(g);
    return (*reinterpret_cast<const PGazePtr*>(g))->getY();
}

coordinate* gaze_entry_get_coordinate(const gaze_entry* g)
{
    assert(g);
    coordinate* ret = NULL;
    try {
        const PGazePtr* gaze = reinterpret_cast<const PGazePtr*>(g);
        ret = new shared_ptr<PCoordinate>(
                std::make_shared<PCoordinate>((*gaze)->getCoordinate())
                );
    } catch (...) {
    }
    return ret;
}

void gaze_entry_set_x(gaze_entry* g, float x)
{
    assert(g);
    (*reinterpret_cast<PGazePtr*>(g))->setX(x);
}

void gaze_entry_set_y(gaze_entry* g, float y)
{
    assert(g);
    (*reinterpret_cast<PGazePtr*>(g))->setY(y);
}

void gaze_entry_set_coordinate(gaze_entry* g, coordinate* c)
{
    assert(g && c);
    (*reinterpret_cast<PGazePtr*>(g))->setCoordinate(
            **(reinterpret_cast<std::shared_ptr<PCoordinate>*>(c)
            );
}

fixation_entry* fixation_entry_new(entrytype et,
                                   double t,
                                   double d,
                                   float x,
                                   float y
                                   )
{
    assert(et == LFIX || et == RFIX);
    fixation_entry* ret = NULL;
    try {
        ret = reinterpret_cast<fixation_entry*>(
                new PFixationEntry(et, t, d, x, y)
                );
    } catch (...) {

    }
    return ret;
}

float fixation_entry_get_x(const fixation_entry* f)
{
    assert(f);
    return reinterpret_cast<const PFixationEntry*>(f)->getX();
}

float fixation_entry_get_y(const fixation_entry* f)
{
    assert(f);
    return reinterpret_cast<const PFixationEntry*>(f)->getY();
}

coordinate* fixation_entry_get_coordinate(const fixation_entry* f)
{
    assert(f);
    coordinate* ret = NULL;
    try {
        ret = reinterpret_cast<coordinate*>(
                new PCoordinate(reinterpret_cast<const PFixationEntry*>(f)->getCoordinate())
                );
    } catch(...) {
    }
    return ret;
}

double fixation_entry_get_duration(const fixation_entry* f)
{
    assert(f);
    return reinterpret_cast<const PFixationEntry*>(f)->getDuration();
}

void fixation_entry_set_x(fixation_entry* f, float x)
{
    assert(f);
    reinterpret_cast<PFixationEntry*>(f)->setX(x);
}

void fixation_entry_set_y(fixation_entry* f, float y)
{
    assert(f);
    reinterpret_cast<PFixationEntry*>(f)->setY(y);
}

void fixation_entry_set_coordinate(fixation_entry* f, coordinate* c)
{
    assert(f && c);
    reinterpret_cast<PFixationEntry*>(f)->setCoordinate(
            *reinterpret_cast<PCoordinate*>(c)
            );
}

void fixation_entry_set_duration(fixation_entry* f, double d)
{
    assert(f);
    reinterpret_cast<PFixationEntry*>(f)->setDuration(d);
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

char* message_entry_get_message(const message_entry* m)
{
    assert(m);
    return strdup(reinterpret_cast<const PMessageEntry*>(m)->getMessage().c_str());
}

void message_entry_set_message(message_entry* m, const char* msg)
{
    assert(m && msg);
    try {
        std::string message(msg);
        reinterpret_cast<PMessageEntry*>(m)->setMessage(message);
    } catch(...) {
        assert(false);
        // improve error handeling here if the string can't be allocated eg.
    }
}

saccade_entry* saccade_entry_new(entrytype et, double t, double d, float x1, float y1,
                                 float x2, float y2)
{
    assert(et == LSAC || et == RSAC);
    saccade_entry* r = NULL;
    try {
        r = reinterpret_cast<saccade_entry*>(
                new PSaccadeEntry(et, t, d, x1, y1, x2, y2)
                );
    } catch(...) {
    }
    return r;
}

float saccade_entry_get_x1(const saccade_entry* s)
{
    assert(s);
    return reinterpret_cast<const PSaccadeEntry*>(s)->getX1();
}

float saccade_entry_get_y1(const saccade_entry* s)
{
    assert(s);
    return reinterpret_cast<const PSaccadeEntry*>(s)->getY1();
}

float saccade_entry_get_x2(const saccade_entry* s)
{
    assert(s);
    return reinterpret_cast<const PSaccadeEntry*>(s)->getX2();
}

float saccade_entry_get_y2(const saccade_entry* s)
{
    assert(s);
    return reinterpret_cast<const PSaccadeEntry*>(s)->getY2();
}

double saccade_entry_get_duration(const saccade_entry* s)
{
    assert(s);
    return reinterpret_cast<const PSaccadeEntry*>(s)->getDuration();
}

coordinate* saccade_entry_get_coordinate1(const saccade_entry* s)
{
    assert(s);
    coordinate* ret = NULL;
    try {
        ret = reinterpret_cast<coordinate*>(
                new PCoordinate(reinterpret_cast<const PSaccadeEntry*>(s)->getCoordinate1())
                );
    } catch(...) {
    }
    return ret;
}

coordinate* saccade_entry_get_coordinate2(const saccade_entry* s)
{
    assert(s);
    coordinate* ret = NULL;
    try {
        ret = reinterpret_cast<coordinate*>(
                new PCoordinate(reinterpret_cast<const PSaccadeEntry*>(s)->getCoordinate2())
                );
    } catch(...) {
    }
    return ret;
}

void saccade_entry_set_x1(saccade_entry* s, float x)
{
    assert(s);
    reinterpret_cast<PSaccadeEntry*>(s)->setX1(x);
}

void saccade_entry_set_y1(saccade_entry* s, float y)
{
    assert(s);
    reinterpret_cast<PSaccadeEntry*>(s)->setY1(y);
}

void saccade_entry_set_x2(saccade_entry* s, float x)
{
    assert(s);
    reinterpret_cast<PSaccadeEntry*>(s)->setX2(x);
}

void saccade_entry_set_y2(saccade_entry* s, float y)
{
    assert(s);
    reinterpret_cast<PSaccadeEntry*>(s)->setY2(y);
}

void saccade_entry_set_duration(saccade_entry* s, double dur)
{
    assert(s);
    reinterpret_cast<PSaccadeEntry*>(s)->setDuration(dur);
}

void saccade_entry_set_coordinate1(saccade_entry* s, coordinate* c)
{
    assert(s && c);
    reinterpret_cast<PSaccadeEntry*>(s)->setCoordinate1(
            *reinterpret_cast<PCoordinate*>(c)
            );
}

void saccade_entry_set_coordinate2(saccade_entry* s, coordinate* c)
{
    assert(s && c);
    reinterpret_cast<PSaccadeEntry*>(s)->setCoordinate2(
            *reinterpret_cast<PCoordinate*>(c)
            );
}


trial_entry* trial_entry_new(double time, const char* identifier, const char* group)
{
    trial_entry* r = NULL;
    try {
        r = reinterpret_cast<trial_entry*>(
                new PTrialEntry(time, identifier, group)
                );
    } catch(...) {
    }
    return r;
}

char* trial_entry_get_identifier(const trial_entry* t)
{
    assert(t);
    return strdup(reinterpret_cast<const PTrialEntry*>(t)->getIdentifier().c_str());
}

char* trial_entry_get_group(const trial_entry* t)
{
    assert(t);
    return strdup(reinterpret_cast<const PTrialEntry*>(t)->getGroup().c_str());
}

void trial_entry_set_identifier(trial_entry* t, const char* identifier)
{
    assert(t && identifier);
    try {
        std::string i(identifier);
        reinterpret_cast<PTrialEntry*>(t)->setIdentifier(i);
    }
    catch(...) {
        // improve error handeling like std::bad_alloc
    }
}

void trial_entry_set_group(trial_entry* t, const char* group)
{
    assert(t && group);
    try {
        std::string g(group);
        reinterpret_cast<PTrialEntry*>(t)->setGroup(g);
    }
    catch(...) {
        // improve error handeling like std::bad_alloc
    }
}

trial_start_entry* trial_start_entry_new(double time)
{
    trial_start_entry* r = NULL;
    try {
        r = reinterpret_cast<trial_start_entry*> (new PTrialStartEntry(time));
    }
    catch (...) {
    }
    return r;
}

trial_end_entry* trial_end_entry_new(double time)
{
    trial_end_entry* r = NULL;
    try {
        r = reinterpret_cast<trial_end_entry*> (new PTrialEndEntry(time));
    }
    catch (...) {
    }
    return r;
}

/* *** Implementation of the eyelog *** */

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
    assert(log && filename);
    PEyeLog* l = reinterpret_cast<PEyeLog*>(log);
    return l->open(filename);
}

void eye_log_close(eye_log* log)
{
    assert(log);
    reinterpret_cast<PEyeLog*>(log)->close();
}

void eye_log_clear(eye_log* log)
{
    assert(log);
    reinterpret_cast<PEyeLog*>(log)->clear();
}

void eye_log_reserve(eye_log* log, unsigned size)
{
    assert(log);
    reinterpret_cast<PEyeLog*>(log)->reserve(size);
}

void eye_log_add_entry(eye_log* log, eyelog_entry* entry)
{
    assert(log && entry);
    PEyeLog      *l = reinterpret_cast<PEyeLog*>(log);
    PEyeLogEntry *e = reinterpret_cast<PEyeLogEntry*>(entry);
    l->addEntry(e);
}

int eye_log_write(eye_log* log, eyelog_format f)
{
    assert(log);
    return reinterpret_cast<PEyeLog*>(log)->write(f);
}

int eye_log_read(eye_log* log, const char* filename, bool clear)
{
    assert(log && filename);
    PEyeLog* l = reinterpret_cast<PEyeLog*>(log);
    return l->read(filename, clear);
}

bool eye_log_is_open(eye_log* log)
{
    assert(log);
	return reinterpret_cast<PEyeLog*>(log)->isOpen();
}

const char* eye_log_get_filename(eye_log* log)
{
    assert(log);
    return reinterpret_cast<PEyeLog*>(log)->getFilename();
}

void eye_log_get_entries(eye_log             *log,
                         eyelog_entry        ***entries,
                         unsigned            *size
                         )
{
    assert(log && entries && size);
    PEyeLog* l = reinterpret_cast<PEyeLog*>(log);
    const std::vector<PEyeLogEntry*> &le = l->getEntries();
    PEyeLogEntry** data = const_cast<PEyeLogEntry**>(le.data());
    *entries = reinterpret_cast<eyelog_entry**>(data);
    *size = le.size();
}
