/*
 * cError.cpp
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

#include "cError.h"
#include <cstring>
#include <cstdio>

const char* eyelog_error(int code)
{
    switch (code){
        case ERR_INVALID_PARAMETER:
            return "Invalid parameter passed";
        case ERR_INVALID_FILE_FORMAT:
            return "The fileformat is not understood";
        default:
            return strerror(code);
    }
}
