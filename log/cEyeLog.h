/*
 * cEyeLog.h
 *
 * Provides C interface for the EyeLog classes.
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

#ifndef C_EYELOG_H
#define C_EYELOG_H

#include "eyelog_export.h"
#include "constants.h"


#ifdef __cplusplus
extern "C" {
#endif

    /*wrappers to PEyeLogEntry*/
    typedef struct eyelog_entry{} eyelog_entry;
    
    // eyelog_entry is an abstract type.
    //eyelog_entry* eyelog_entry_new();
    EYELOG_EXPORT  void	            eyelog_entry_destroy(eyelog_entry* e);
    EYELOG_EXPORT  eyelog_entry*    eyelog_entry_clone(const eyelog_entry* other);
    EYELOG_EXPORT  char*            eyelog_entry_to_string(const eyelog_entry* e);
    EYELOG_EXPORT  void             eyelog_entry_set_separator(eyelog_entry* e, char c);
    EYELOG_EXPORT  char             eyelog_entry_get_separator(const eyelog_entry* e);
    EYELOG_EXPORT  void             eyelog_entry_set_precision(eyelog_entry* e,
                                           unsigned precision);
    EYELOG_EXPORT  unsigned         eyelog_entry_get_precision(const eyelog_entry* e);

    /*Wrappers to PEyelog*/
    typedef struct gaze_entry{} gaze_entry;

    EYELOG_EXPORT  gaze_entry*      gaze_entry_new(entrytype et, double time,
                                            float x, float y, float psiz);
    EYELOG_EXPORT  char*            gaze_entry_to_string(const gaze_entry* g);
    
    /*Wrappers to PFixationEntry*/
    typedef struct fixation_entry{} fixation_entry;

    EYELOG_EXPORT  fixation_entry*  fixation_entry_new(entrytype et, double time, double duration,
                                            float x, float y);
    EYELOG_EXPORT  char*            fixation_entry_to_string(const fixation_entry* f);

    /*Wrappers to PMessageEntry*/
    typedef struct message_entry{} message_entry;

    EYELOG_EXPORT  message_entry*   message_entry_new(double eyetime, const char* msg);
    EYELOG_EXPORT  char*            message_entry_to_string(const message_entry* m);

    /*Wrappers to PSaccadeEntry*/
    typedef struct saccade_entry{} saccade_entry;

    EYELOG_EXPORT  saccade_entry*   saccade_entry_new(entrytype et, double time, double dur,
                                            float x1, float y1, float x2, float y2);

    EYELOG_EXPORT  char*            saccade_entry_to_string(const saccade_entry* s);

    /*Wrapper to PEyeLog*/
    typedef struct eye_log{} eye_log;

    EYELOG_EXPORT  eye_log*         eye_log_new();
    EYELOG_EXPORT  void             eye_log_destroy(eye_log* log);
    EYELOG_EXPORT  int              eye_log_open(eye_log* log, const char* filename);
    EYELOG_EXPORT  void             eye_log_close(eye_log* log);
    EYELOG_EXPORT  void             eye_log_clear(eye_log* log);
    EYELOG_EXPORT  void             eye_log_reserve(eye_log* log, unsigned size);
    EYELOG_EXPORT  void             eye_log_add_entry(eye_log* log, eyelog_entry* entry);
    EYELOG_EXPORT  int              eye_log_write(eye_log* log, eyelog_format format);
    EYELOG_EXPORT  int              eye_log_read(eye_log* log,
                                            const char* filename,
                                            bool clear
                                            );
    EYELOG_EXPORT  bool             eye_log_is_open(eye_log* log);
    EYELOG_EXPORT  const char*      eye_log_get_filename(eye_log* log);
    EYELOG_EXPORT  void             eye_log_get_entries(eye_log* log,
                                            eyelog_entry*** pentries,
                                            unsigned* size
                                            );

#ifdef __cplusplus
}
#endif

#endif
