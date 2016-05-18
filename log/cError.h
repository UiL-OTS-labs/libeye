/*
 * cError.h
 *
 * Provides functions to turn error codes into string
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
 * \file cError.h
 *
 * This file contains the api that is useful for handeling errors
 * that can occur when using libeye.
 */

#ifndef C_ERROR_H
#define C_ERROR_H

#include "eyelog_export.h"

/**
 * \brief eyelog_error_t contains a code used to indicate which error occured.
 *
 * Eyelog uses errors that are negative for errors that directly relate to
 * some logical error while using this library.
 */
enum eyelog_error_t {
    ERR_START = -10000,     ///< just a arbitrairy starting point for errors.
    ERR_INVALID_PARAMETER,  ///< The function has been given an invalid parameter value.
    ERR_INVALID_FILE_FORMAT ///< The file format is not understood.   
};

#ifdef __cplusplus
extern "C" {
#endif

/**
 * eyelog_error is usefull to obtain information about an error that has
 * occured while useing libeye.
 *
 * \returns a string that describes the error that has occured.
 */
EYELOG_EXPORT const char* eyelog_error(int code);

#ifdef __cplusplus
}
#endif

#endif
