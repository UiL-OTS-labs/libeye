/*
 * Instantation.cpp
 *
 * This file is part of libeye and implements instantations of templated types.
 *
 * Copyright (C) 2016  Maarten Duijndam
 * 
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 * 
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include "BaseString.cpp"
#include "DArray.cpp"
#include "PEyeLogEntry.h"
#include "PExperiment.h"

// Template to the String type of libeye.
template class DArray<char>; // the underlying allocator for Base string.
template class BaseString<char>;

// Template to a dynamic array of strings.
template class DArray <BaseString<char> >;

// Template to the logentries.
template class DArray <PEntryPtr>;
template class DArray <PTrial>;
//template class DArray <PEyeLogEntry>;
//template class DArray <PGazeEntry>;
//template class DArray <PFixationEntry>;
//template class DArray <PMessageEntry>;
//template class DArray <PSaccadeEntry>;
//template class DArray <PStimulusEntry>;
//template class DArray <PTrialEntry>;

