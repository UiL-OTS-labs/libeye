/*
 * BaseString.h
 *
 * Public header that provides a basic templated string.
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


#if !defined(BASE_STRING_H)
#define BASE_STRING_H 1

#include <utility>

#include "DArray.h"
#include "eyelog_export.h"

template <typename T>
class EYELOG_EXPORT BaseString {

    public:

        typedef typename DArray<T>::iterator         iterator;
        typedef typename DArray<T>::const_iterator   const_iterator;
        typedef typename DArray<T>::size_type        size_type;
        typedef typename DArray<T>::value_type       value_type;
        typedef typename DArray<T>::pointer          pointer;
        typedef typename DArray<T>::const_pointer    const_pointer;
        typedef typename DArray<T>::reference        reference;
        typedef typename DArray<T>::const_reference  const_reference;

        BaseString();
        explicit BaseString(const_iterator begin, const_iterator end);
        BaseString(const_pointer begin);
        explicit BaseString(pointer begin);
        BaseString(const BaseString& other);
        explicit BaseString(BaseString&& other) noexcept;
        explicit BaseString(const char& c);

        BaseString& operator=(const BaseString& rhs);
        BaseString& operator=(BaseString&& rhs) noexcept;
        BaseString& operator=(const_pointer ptr);

        BaseString& operator+=(const BaseString& rhs);

        void                push_back (const_reference value);
        void                pop_back(void);
        reference           operator[](size_type pos);
        const_reference     operator[](size_type pos)const;

        size_type   size() const;
        size_type   length() const;
        size_type   capacity() const;

        iterator    erase(const_iterator pos);
        iterator    erase(const_iterator begin, const_iterator end);
        BaseString& erase(size_type pos, size_type count);
        void        clear();
        void        reserve(size_type n);
        void        resize(size_type n);
        void        resize(size_type n, const value_type& val);
        void        insert(iterator pos, const_iterator begin, const_iterator end);
        
        iterator        begin()const;
        iterator        end()const;

        const_iterator  cbegin()const;
        const_iterator  cend() const;

        const_pointer   c_str()const;

        bool operator ==(const BaseString& other)const;
        bool operator !=(const BaseString& other)const;
        bool operator < (const BaseString& other)const;
        bool operator > (const BaseString& other)const;
        
        int             compare(const BaseString& other)const;
    
    private:

        DArray<T>    m_chars;
};

/**
 * Regular take two reference and create new string
 */
template <class T>
inline BaseString<T> operator+(const BaseString<T>& lhs, const BaseString<T>& rhs)
{
    BaseString<T> temp;
    temp.reserve(lhs.size() + rhs.size());
    temp.insert(temp.end(), lhs.begin(), lhs.end());
    temp.insert(temp.end(), rhs.begin(), rhs.end());
    return temp;
}

/**
 * concatenate String and character.
 */
template<class T>
inline BaseString<T> operator+(const BaseString<T>& lhs, const char c)
{
    BaseString<T> temp;
    temp.reserve(lhs.size() + 1);
    temp+=lhs;
    temp.push_back(c);
    return temp;
}

/** Move semantics */
template <class T>
inline BaseString<T> operator+(BaseString<T>&& lhs, const BaseString<T>& rhs)
{
    lhs += rhs;
    return std::move(lhs);
}

template <class T>
inline BaseString<T> operator+(const BaseString<T>& lhs, BaseString<T>&& rhs)
{  
    rhs.insert(rhs.begin(), lhs.begin(), lhs.end());
    return std::move(rhs);
}

template <class T>
inline BaseString<T> operator+(BaseString<T>&& lhs,  BaseString<T>&& rhs)
{
    lhs += rhs;
    return std::move(lhs);
}

#endif
