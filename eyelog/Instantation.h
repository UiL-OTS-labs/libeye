/*
 * Instantation.h
 *
 * Public header that provides instantations of templated types.
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
 * \file Instantation.h declares the existance of some templated types.
 */

#if !defined(EYE_INSTANTATION_H)
#   define EYE_INSTANTATION_H 1


#include "BaseString.h"
#include "DArray.h"
#include "PEyeLogEntry.h"

extern BaseString<char>;
extern DArray<char>;

extern DArray<PEntryPtr>;

#endif
