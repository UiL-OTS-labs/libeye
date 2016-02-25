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
    ERR_START = -10000,
    ERR_INVALID_PARAMETER,
    ERR_INVALID_FILE_FORMAT
};

#ifdef __cplusplus
extern "C" {
#endif

EYELOG_EXPORT const char* eyelog_error(int code);

#ifdef __cplusplus
}
#endif

#endif
