/*
 * BaseString.cpp
 *
 * Provides the implementation of the BaseString.
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

#include "Atomic.h"

#if defined(HAVE_STDATOMIC_H)

atomic_int atomic_increment(atomic_int* val)
{
    return atomic_fetch_add(val, 1);
}

atomic_int atomic_decrement(atomic_int* val)
{
    return atomic_fetch_sub(val, 1);
}

#elif defined(HAVE_WINDOWS_H) && !defined(HAVE_WINDOWS_H)

atomic_int atomic_increment(atomic_int* val)
{
    return InterlockedIncrement(val);
}

atomic_int atomic_decrement(atomic_int* val)
{
    return InterlockedAdd(val, -1);
}

#else
// Oops nothing present to implement atomics
#pragma message "No header to implement Atomic.h"
#endif
