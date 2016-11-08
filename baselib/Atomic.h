/*
 * Atomic.h
 *
 * Public header that provides a atomic int for reference counting
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


#if !defined(BASE_ATOMIC_H)
#define BASE_ATOMIC_H 1

#include "libeye-config.h"

#if defined(HAVE_STDATOMIC_H)
#include <stdatomic.h>
#endif

namespace eye {
#if defined(HAVE_STDATOMIC_H)

    typedef _Atomic int atomic_int;

#elif defined(HAVE_WINDOWS_H) && !defined(HAVE_STDATOMIC_H)

    typedef volatile LONG atomic_int;

#endif

    atomic_int atomic_increment(atomic_int* v);
    atomic_int atomic_decrement(atomic_int* v);

}

#endif
