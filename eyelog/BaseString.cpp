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

#include "BaseString.h"
#include <cassert>

template<class T>
BaseString<T>::BaseString()
{
    m_chars.push_back(value_type('\0'));
}

template<class T>
BaseString<T>::BaseString(const_iterator begin, const_iterator end)
{
    m_chars.reserve(end - begin + 1);
    while (begin != end)
        m_chars.push_back(*begin++);
    m_chars.push_back(value_type('\0'));
}

template<class T>
BaseString<T>::BaseString(const_pointer ptr)
{
    const_pointer end = ptr;
    while (*++end != value_type('\0'))
        ;
    m_chars.reserve(end - ptr);
    do {
        m_chars.push_back(*ptr);
    } while (*ptr++ != '\0');
}

template<class T>
BaseString<T>::BaseString(pointer ptr)
{
    pointer end = ptr;
    while (*++end != value_type('\0'))
        ;
    m_chars.reserve(end - ptr);
    do {
        m_chars.push_back(*ptr);
    } while (*ptr++ != '\0');
}

template<class T>
BaseString<T>::BaseString(const BaseString& other)
    : m_chars(other.m_chars)
{
    assert(other.m_chars.size() >= 1);
    assert(other.m_chars[other.m_chars.size()-1]== value_type('\0'));
}

template<class T>
BaseString<T>::BaseString(BaseString&& other) noexcept
{
    m_chars = std::move(other.m_chars);
}

template<class T>
BaseString<T>::BaseString(const char& c)
{
    m_chars.push_back(c);
    m_chars.push_back(value_type('\0'));
}

template<class T>
typename BaseString<T>::iterator BaseString<T>::erase(const_iterator pos)
{
    return m_chars.erase(pos);
}

template<class T>
typename BaseString<T>::iterator BaseString<T>::erase(const_iterator beg,
                                                      const_iterator end
                                                      )
{
    return m_chars.erase(beg, end);
}

template<class T>
BaseString<T>& BaseString<T>::erase(size_type index,
                                    size_type count 
                                    )
{
    m_chars.erase(begin() + index, begin() + index + count);
    return *this;
}

template<class T>
void BaseString<T>::insert(iterator pos, const_iterator begin, const_iterator end)
{
    m_chars.insert(pos, begin, end);
}

template<class T>
BaseString<T>& BaseString<T>::operator=(const BaseString& other)
{
    if (this != &other) {
        m_chars = other.m_chars;
    }
    return *this;
}

template<class T>
BaseString<T>& BaseString<T>::operator=(BaseString&& other) noexcept
{
    if (this != &other) {
        m_chars = std::move(other.m_chars);
    }
    return *this;
}

template<class T>
BaseString<T>& BaseString<T>::operator=(const_pointer ptr) 
{
    return *this = std::move(BaseString<T>(ptr));
}

template<class T>
BaseString<T>& BaseString<T>::operator+=(const BaseString& other)
{
    m_chars.insert(m_chars.end()-1, other.m_chars.cbegin(), other.m_chars.cend()-1);
    return *this;
}

template<class T>
typename BaseString<T>::const_pointer BaseString<T>::c_str() const
{
    return &m_chars[0];
}

template<class T>
void BaseString<T>::push_back(const_reference value)
{
    m_chars.insert(end(), value);
}

template<class T>
void BaseString<T>::pop_back()
{
    m_chars.erase(end()-1, end());
}


template<class T>
typename BaseString<T>::reference BaseString<T>::operator[](size_type n)
{
    return m_chars[n];
}

template<class T>
typename BaseString<T>::const_reference
BaseString<T>::operator[](size_type n) const
{
    return m_chars[n];
}

template<class T>
typename BaseString<T>::size_type BaseString<T>::size()const
{
    return m_chars.size() - 1;
}

template<class T>
typename BaseString<T>::size_type BaseString<T>::length()const
{
    return m_chars.size() - 1;
}

template<class T>
typename BaseString<T>::size_type BaseString<T>::capacity()const
{
    return m_chars.capacity() - 1;
}

template<class T>
void BaseString<T>::clear()
{
    m_chars.clear();
    m_chars.push_back(value_type('\0'));
}

template<class T>
void BaseString<T>::reserve(size_type n)
{
    m_chars.reserve(n+1);
}

template<class T>
void BaseString<T>::resize(size_type n)
{
    resize(n, value_type('\0'));
}

template<class T>
void BaseString<T>::resize(size_type n, const value_type& v)
{
    m_chars.pop_back();
    m_chars.resize(n, v);
    m_chars.push_back(value_type('\0'));
} 

template<class T>
typename BaseString<T>::iterator BaseString<T>::begin() const
{
    return m_chars.begin();
}

template<class T>
typename BaseString<T>::iterator BaseString<T>::end() const
{
    return begin() + size();
}

template<class T>
typename BaseString<T>::const_iterator BaseString<T>::cbegin() const
{
    return m_chars.cbegin();
}

template<class T>
typename BaseString<T>::const_iterator BaseString<T>::cend() const
{
    return m_chars.cbegin() + size();
}

template<class T>
int BaseString<T>::compare(const BaseString& other) const
{
    auto max = other.size() < size() ? other.size() : size();
    int ret = 0;
    for (auto i = 0u; i < max; ++i) {
        ret = m_chars[i] - other[i];
        if (ret)
            return ret;
    }
    return size() - other.size();
}

template<class T>
bool BaseString<T>::operator==(const BaseString& other) const
{
    return this->compare(other) == 0;
}

template<class T>
bool BaseString<T>::operator!=(const BaseString& other) const
{
    return this->compare(other) != 0;
}

template<class T>
bool BaseString<T>::operator < (const BaseString& other) const
{
    return this->compare(other) < 0;
}

template<class T>
bool BaseString<T>::operator > (const BaseString& other) const
{
    return this->compare(other) > 0;
}

