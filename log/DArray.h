/*
 * DArray.h
 *
 * Public header that provides dynamic arrays.
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

#if !defined(DARRAY_H)
#   define DARRAY_H 1

#include <cstddef>

template <class T>
class DArray {
    public:

        typedef T*          iterator;
        typedef const T*    const_iterator;
        typedef std::size_t size_type;
        typedef T           value_type;
        typedef T*          pointer;
        typedef const T*    const_pointer;
        typedef T&          reference;
        typedef const T&    const_reference;

        DArray();
        DArray(size_type n, const T& value);
        DArray(size_type n);
        DArray(const T* begin, const T*end);
        DArray(const DArray& other);
        DArray(DArray&& other) noexcept;
        DArray& operator=(const DArray& rhs);
        DArray& operator=(DArray&& rhs)noexcept;
        ~DArray();

        void push_back(const T& ref);
        void pop_back(void);

        size_type   size()const;
        size_type   capacity()const;
        bool        empty()const;

        void reserve(size_type n);

        void insert(iterator pos, const_iterator beg, const_iterator end);
        void insert(iterator pos, const_reference value);
        void resize(size_type n);
        void resize(size_type n, const T& value);
        iterator erase(const_iterator pos);
        iterator erase(const_iterator begin, const_iterator end);
        void clear();

        reference       operator [](size_type size);
        const_reference operator [](size_type size)const;

        bool            operator == (const DArray& other) const;

        iterator begin() const;
        iterator end() const;

        const_iterator cbegin() const;
        const_iterator cend() const;

    private :

        void m_setCapacity(size_type n);

    private :

        T*          m_items;
        size_type   m_size;
        size_type   m_capacity;
};

#endif
