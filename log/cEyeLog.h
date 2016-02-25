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

    /*Wrapper to a PCoordinate*/
    typedef struct coordinate{} coordinate;

    EYELOG_EXPORT  coordinate*      coordinate_new();
    EYELOG_EXPORT  coordinate*      coordinate_new_xy(float x, float y);
    EYELOG_EXPORT  void             coordinate_destroy(coordinate* c);
    EYELOG_EXPORT  void             coordinate_set_x(coordinate* c, float x);
    EYELOG_EXPORT  void             coordinate_set_y(coordinate* c, float y);
    EYELOG_EXPORT  float            coordinate_get_x(const coordinate* c);
    EYELOG_EXPORT  float            coordinate_get_y(const coordinate* c);

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
    EYELOG_EXPORT  float            gaze_entry_get_x(const gaze_entry* g);
    EYELOG_EXPORT  float            gaze_entry_get_y(const gaze_entry* g);
    EYELOG_EXPORT  coordinate*      gaze_entry_get_coordinate(const gaze_entry* g);
    EYELOG_EXPORT  void             gaze_entry_set_x(gaze_entry* g, float x);
    EYELOG_EXPORT  void             gaze_entry_set_y(gaze_entry* g, float y);
    EYELOG_EXPORT  void             gaze_entry_set_coordinate(gaze_entry* g, const coordinate* c);
    
    /*Wrappers to PFixationEntry*/
    typedef struct fixation_entry{} fixation_entry;

    EYELOG_EXPORT  fixation_entry*  fixation_entry_new(entrytype et, double time, double duration,
                                            float x, float y);
    EYELOG_EXPORT  float            fixation_entry_get_x(const fixation_entry* f);
    EYELOG_EXPORT  float            fixation_entry_get_y(const fixation_entry* f);
    EYELOG_EXPORT  coordinate*      fixation_entry_get_coordinate(const fixation_entry* f);
    EYELOG_EXPORT  double           fixation_entry_get_duration(const fixation_entry* f);
    EYELOG_EXPORT  void             fixation_entry_set_x(fixation_entry* f, float x);
    EYELOG_EXPORT  void             fixation_entry_set_y(fixation_entry* f, float y);
    EYELOG_EXPORT  void             fixation_entry_set_coordinate(fixation_entry* f,
                                                                  const coordinate* c);
    EYELOG_EXPORT  void             fixation_entry_set_duration(const fixation_entry* f,
                                                                double time);

    /*Wrappers to PMessageEntry*/
    typedef struct message_entry{} message_entry;

    EYELOG_EXPORT  message_entry*   message_entry_new(double eyetime, const char* msg);
    EYELOG_EXPORT  char*            message_entry_get_message(const message_entry* m);
    EYELOG_EXPORT  void             message_entry_set_message(message_entry* m,
                                                              const char* msg);

    /*Wrappers to PSaccadeEntry*/
    typedef struct saccade_entry{} saccade_entry;

    EYELOG_EXPORT  saccade_entry*   saccade_entry_new(entrytype et, double time, double dur,
                                            float x1, float y1, float x2, float y2);

    EYELOG_EXPORT  float            saccade_entry_get_x1(const saccade_entry* f);
    EYELOG_EXPORT  float            saccade_entry_get_y1(const saccade_entry* f);
    EYELOG_EXPORT  coordinate*      saccade_entry_get_coordinate1(const saccade_entry* f);
    EYELOG_EXPORT  float            saccade_entry_get_x2(const saccade_entry* f);
    EYELOG_EXPORT  float            saccade_entry_get_y2(const saccade_entry* f);
    EYELOG_EXPORT  coordinate*      saccade_entry_get_coordinate2(const saccade_entry* f);
    EYELOG_EXPORT  double           saccade_entry_get_duration(const saccade_entry* f);
    EYELOG_EXPORT  void             saccade_entry_set_x1(saccade_entry* f, float x);
    EYELOG_EXPORT  void             saccade_entry_set_y1(saccade_entry* f, float y);
    EYELOG_EXPORT  void             saccade_entry_set_coordinate1(saccade_entry* f,
                                                                 const coordinate* c);
    EYELOG_EXPORT  void             saccade_entry_set_x2(saccade_entry* f, float x);
    EYELOG_EXPORT  void             saccade_entry_set_y2(saccade_entry* f, float y);
    EYELOG_EXPORT  void             saccade_entry_set_coordinate2(saccade_entry* f,
                                                                 const coordinate* c);
    EYELOG_EXPORT  void             saccade_entry_set_duration(const saccade_entry* f,
                                                               double time);

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
