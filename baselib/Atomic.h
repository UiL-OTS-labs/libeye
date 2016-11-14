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

#if defined(HAVE_WINDOWS_H) and ( !defined(__GNUC__) || !defined(__clang__) )
//#include <windows.h>
#endif

namespace eye {

    class AtomicCounter {

        public:
            AtomicCounter() : m_cnt (0) {};
            AtomicCounter(int n) : m_cnt (n) {};

            /**
             * Atomically increment m_count by val
             */
            int increment(int val=1) noexcept
            {
#if defined(__GNUC__) || defined(__clang__)
                return __sync_add_and_fetch(&m_cnt, val);
#elif defined(_MSC_VER)
                return InterlockedAdd(&m_cnt, val);
#endif
            };

            /**
             * Atomically decrement m_count by val
             */
            int decrement(int val=1) noexcept
            {
                return increment(-val);
            }

            int count() noexcept
            {
                return m_cnt;
            }

        private:

            /**
             * The integer value that represent the count.
             */
            volatile int32_t    m_cnt;
    };

}

#endif
