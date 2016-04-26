/*
 * constants.h
 *
 * Provides enumerations for the library.
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

#ifndef CONSTANTS_H
#define CONSTANTS_H


/**
 * entrytype define with what kind of log entry we are dealing with.
 *
 * Note the order of the entries in the entrytype enum greatly affect
 * how the files are saved and restored. Therefore only add members to
 * the back and reordering the enumeration is only allowed in a
 * new major revision of libeye.
 */
enum entrytype {
    LGAZE,      //!< Left gaze sample.
    RGAZE,      //!< Right gaze sample.
    LFIX,       //!< Left eye fixation.
    RFIX,       //!< Right eye fixation.
    STIMULUS,   //!< Filename of the stimulus.
    MESSAGE,    //!< A user defined message.
    LSAC,       //!< A saccade of the left eye.
    RSAC,       //!< A saccade of the right eye.
    AVGGAZE,    //!< A gaze sample of the position between the left and right eye.
    AVGFIX,     //!< A fixation of the average gaze samples.
    AVGSAC,     //!< A saccade of the average gaze sample.
    TRIAL,      //!< A new trial
    TRIALSTART, //!< Ignore data before TRIALSTART in trial
    TRIALEND    //!< Ignore data after TRIALEND in trial
};

/**
 * eyelog_format
 */
enum eyelog_format {
    FORMAT_BINARY,  //!< Used when writing a binary version of the log
    FORMAT_CSV      //!< Used when writing a CSV version of the log
};

#endif
