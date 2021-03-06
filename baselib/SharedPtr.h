/*
 * SharedPtr.h
 *
 * Public header that provides a basic shared pointer
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


#if !defined(EYE_SHARED_PTR_H)
#define EYE_SHARED_PTR_H 1

#include <atomic>

#include <baselib/base_export.h>

namespace eye {

    template <class T>
    class BASE_EXPORT SharedPtr {
        
        private:
            T*                  m_ptr;
            std::atomic<int>   *m_refcnt;
            //int                *m_refcnt;

            /**
             * Decrements the reference count
             *
             * If the reference counter drop to zero m_ptr and m_refcnt
             * will be deleted.
             */
            void decrement()const; 

            /**
             * Increments the reference count.
             */
            void increment()const;

        public:


            SharedPtr();
            SharedPtr(T* ptr);
            SharedPtr(const std::nullptr_t& np);
            ~SharedPtr();
            SharedPtr(const SharedPtr& other);

            //// Don't know how to safely implement it.
            //SharedPtr(SharedPtr&& other);
            SharedPtr<T>& operator= (const SharedPtr<T>& rhs);

            T* get();
            const T* get()const;

            T& operator *();
            const T& operator*()const;

            T* operator->();
            const T* operator->()const;
            
            // for ordering in ordered maps and sets.
            bool operator == (const SharedPtr<T>& rhs)const;
            bool operator != (const SharedPtr<T>& rhs)const;

            bool operator <(const SharedPtr<T>& rhs) const;
            bool operator <=(const SharedPtr<T>& rhs) const;
            bool operator >(const SharedPtr<T>& rhs) const;
            bool operator >=(const SharedPtr<T>& rhs) const;

            bool operator != (const std::nullptr_t& nptr) const;
            bool operator == (const std::nullptr_t& nptr) const;

            // for testing whether the pointer is set.
            explicit operator bool() const;

            int refcount()const;
    };

}

#endif

