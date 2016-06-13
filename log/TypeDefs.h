/*
 * Typdefs.h
 *
 * Public header that provides typedefs for types that are used often.
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

#if !defined (TYPEDEFS_H)
#define TYPEDEFS_H 1

//The one and only string type for libeye.
#include "BaseString.h"
typedef BaseString<char> String;

#include "PEyeLogEntry.h"
#include "DArray.h"
// insert typedef to all log entries here.

#endif
