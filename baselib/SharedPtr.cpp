/*
 * SharedPtr.cpp
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


#include "SharedPtr.h"
#include <algorithm>

using namespace eye;

template <class T>
void SharedPtr<T>::increment() const
{
    (*m_refcnt)++;
}

template <class T>
void SharedPtr<T>::decrement() const
{
    if (!m_refcnt)
        return;
    if ( m_refcnt->fetch_sub(1) - 1 <= 0) {
        delete m_refcnt;
        delete m_ptr;
    }
}

template <class T>
SharedPtr<T>::SharedPtr()
{
    m_ptr   = nullptr;
    m_refcnt= new std::atomic<int>(0);
    increment();
}

template <class T>
SharedPtr<T>::SharedPtr(T* obj)
{
    m_ptr   = obj;
    m_refcnt= new std::atomic<int>(0);
    //m_refcnt= new int(0);
    increment();
}

template <class T>
SharedPtr<T>::SharedPtr(const std::nullptr_t& ptr)
{
    m_ptr   = ptr;
    m_refcnt= new std::atomic<int>(0);
    increment();
}

template <class T>
SharedPtr<T>::SharedPtr(const SharedPtr<T>& obj)
{
    m_ptr   = obj.m_ptr;
    m_refcnt= obj.m_refcnt;
    increment();
}

//template <class T>
//SharedPtr<T>::SharedPtr(SharedPtr<T>&& obj)
//{
//    m_ptr   = nullptr;
//    m_refcnt= new std::atomic<int>(0);
//    increment();
//
//    obj.m_refcnt->exchange(*m_refcnt);
//    std::swap<T*>(m_ptr, obj.m_ptr);
//}

template <class T>
SharedPtr<T>::~SharedPtr()
{
    decrement();
}

template <class T>
SharedPtr<T>& SharedPtr<T>::operator=(const SharedPtr<T>& rhs)
{
    if (this != &rhs) {
        if (m_refcnt) {
            decrement();
            m_refcnt = rhs.m_refcnt;
            m_ptr = rhs.m_ptr;
        }
        else {
            m_refcnt = new std::atomic<int>(0);
            m_ptr = rhs.m_ptr;
        }
        increment();
    }

    return *this;
}

template <class T>
T* SharedPtr<T>::get()
{
    return m_ptr;
}

template <class T>
const T* SharedPtr<T>::get() const
{
    return m_ptr;
}

template <class T>
T* SharedPtr<T>::operator->()
{
    return m_ptr;
}

template <class T>
const T* SharedPtr<T>::operator->() const
{
    return m_ptr;
}

template <class T>
T& SharedPtr<T>::operator*()
{
    return *m_ptr;
}

template <class T>
const T& SharedPtr<T>::operator*() const
{
    return *m_ptr;
}

template <class T>
int SharedPtr<T>::refcount()const
{
    if (!m_refcnt)
        return -1;
    return *m_refcnt;
}

template <class T>
bool SharedPtr<T>::operator==(const SharedPtr<T>& rhs)const
{
    return **this == *rhs;
}

template <class T>
bool SharedPtr<T>::operator != (const SharedPtr<T>& rhs) const
{
    return **this != *rhs;
}

template <class T>
bool SharedPtr<T>::operator<(const SharedPtr<T>& rhs)const
{
    return **this < *rhs;
}

template<class T>
bool SharedPtr<T>::operator<=(const SharedPtr<T>& rhs)const
{
    return **this <= *rhs;
}

template <class T>
bool SharedPtr<T>::operator>(const SharedPtr<T>& rhs)const
{
    return **this > *rhs;
}

template<class T>
bool SharedPtr<T>::operator>=(const SharedPtr<T>& rhs)const
{
    return **this >= *rhs;
}

template<class T>
bool SharedPtr<T>::operator == (const std::nullptr_t& np) const
{
    return get() == np;
}

template<class T>
bool SharedPtr<T>::operator != (const std::nullptr_t& np) const
{
    return get() != np;
}


template <class T>
SharedPtr<T>::operator bool() const
{
    return get() != nullptr;
}

// instantations
template class SharedPtr<int>;

