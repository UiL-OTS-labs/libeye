
#include "DArray.h"
#include <cassert>
#include <utility>


template <class T>
DArray<T>::DArray()
    :
        m_items(nullptr),
        m_size(0),
        m_capacity(0)
{
}

template <class T>
DArray<T>::DArray(const DArray& other)
    :
        m_items(nullptr),
        m_size(0),
        m_capacity(0)
{
    *this = other;
}

template <class T>
DArray<T>::DArray(DArray&& other) noexcept
    :
        m_items(nullptr),
        m_size(0),
        m_capacity(0)
{
    m_items     = other.m_items;
    m_size      = other.m_size;
    m_capacity  = other.m_capacity;
    other.m_items   = nullptr;
    other.m_size    = 0;
    other.m_capacity= 0;
}

template <class T>
DArray<T>::DArray(const T* begin, const T* end)
    :
        m_items(nullptr)
{
    m_size = end - begin;
    m_setCapacity(m_size);
    for (auto it = &m_items[0]; it < &m_items[0] + m_size; ++it)
        *it=*begin++;
}

template <class T>
DArray<T>::DArray(size_type n)
    :
        m_items(nullptr),
        m_size(0),
        m_capacity(0)
{
    m_setCapacity(n);
    m_size = n;
}

template <class T>
DArray<T>::DArray(size_type n, const T& value)
    :
        m_items(nullptr),
        m_size(0),
        m_capacity(0)
{
    m_setCapacity(n);
    for (size_type i = 0; i < m_capacity; ++i) {
        m_items[i] = value;
    }
    m_size = n;
}

template <class T>
DArray<T>& DArray<T>::operator=(const DArray& other)
{
    if (&other != this) {
        clear();
        m_items = new T[other.size()];
        m_capacity = other.size();
        m_size = other.size();
        for (size_type i = 0; i < other.size(); i++)
            m_items[i] = other[i];
    }
    
    return *this;
}

template <class T>
DArray<T>& DArray<T>::operator=(DArray&& other) noexcept
{
    if (&other != this) {
        clear();
        m_items = other.m_items;
        other.m_items = nullptr;
        m_capacity = other.m_capacity;
        other.m_capacity = 0;
        m_size = other.m_size;
        other.m_size = 0;
    }
    
    return *this;
}

template <class T>
DArray<T>::~DArray()
{
    clear();
}

template <class T>
void DArray<T>::m_setCapacity(size_type n)
{
    if (!m_items) {
        m_items = new T[n];
    } else {
        T* temp = new T[n];
        T* ts = &temp[0];
        T* is = &m_items[0];
        for ( ;is < &m_items[0]+m_size; ++ts, ++is) {
            *ts = *is;
        }
        delete[] m_items;
        m_items = temp;
    }
    m_capacity = n;
}

template <class T>
typename DArray<T>::size_type DArray<T>::size()const
{
    return m_size;
}

template <class T>
typename DArray<T>::size_type DArray<T>::capacity()const
{
    return m_capacity;
}

template <class T>
bool DArray<T>::empty()const
{
    return size() == 0;
}

template <class T>
bool DArray<T>::operator==(const DArray& other) const {
    if (size() != other.size())
        return false;
    for (auto i =0u; i < other.size(); ++i)
        if (m_items[i] != other[i])
            return false;
    return true;
}

template <class T>
void DArray<T>::push_back(const T& ref)
{
    if(m_capacity <= m_size) {
        size_type newsize = m_size == 0 ? 1 : m_size * 2;
        m_setCapacity(newsize);
    }
    m_items[m_size++] = ref;
}

template <class T>
void DArray<T>::pop_back(void)
{
    --m_size;
}

template <class T>
void DArray<T>::clear()
{
    delete[] m_items;
    m_items = nullptr;
    m_size=0;
    m_capacity=0;
}

template <class T>
void DArray<T>::reserve(size_type n)
{
    m_setCapacity(n >= m_size ? n : m_size);
}

template <class T>
void DArray<T>::resize(size_type n)
{
    resize(n, T());
}

template <class T>
void DArray<T>::resize(size_type n, const T& value)
{
    if (n < m_size) { // shrinking
        m_size = n;
        if (m_capacity/2 >= m_size) {
            m_setCapacity(m_size);
        }
    }
    else if (n > m_size) { // expanding
        if (n > m_capacity) {
            m_setCapacity(m_capacity*2>n ? m_capacity*2 : n);
        }
        for (size_type i = m_size; i < n; i++) {
            m_items[i] = value;
        }
        m_size = n;
    }
    else {
        // Nothing to do, resizing to equal size.
    }
}

template<class T>
typename DArray<T>::iterator DArray<T>::erase(const_iterator pos)
{
    return erase(pos, pos+1);
}

template<class T>
typename DArray<T>::iterator DArray<T>::erase(const_iterator beg,
                                              const_iterator finish 
                                              )
{
    size_type n_erased = finish - beg;
    size_type start_index = beg - begin();
    iterator cpy_beg = const_cast<iterator>(beg + n_erased);
    iterator cpy_to  = const_cast<iterator>(beg);
    for (; cpy_beg < end(); ++cpy_beg, ++cpy_to)
        std::swap(*cpy_to, *cpy_beg);
    resize(size() - n_erased);

    return begin() + start_index;
}

template<class T>
void DArray<T>::insert(iterator pos, const_iterator beg_it, const_iterator end_it)
{
    size_type ins_sz = end_it - beg_it;
    size_type nmove  = end() - pos;
    size_type move_dist = ins_sz;
    //size_type old_size = size();
    size_type ins_pos = pos - begin();

    resize(m_size + ins_sz);
    iterator it = begin() + size() - 1;
    iterator revend = it - nmove;
    for ( ; it > revend; --it) {
        *it = *(it - move_dist);
    }

    it = begin() + ins_pos;
    for (; it < begin() + ins_pos + ins_sz; ++it, ++beg_it) {
        *it = *beg_it;
    }
}

template<class T>
void DArray<T>::insert(iterator pos, const_reference value)
{
    insert(pos, &value, &value + 1);
}



template<class T>
T& DArray<T>::operator[](size_type n)
{
    return m_items[n];
}

template<class T>
const T& DArray<T>::operator[](size_type n) const
{
    return m_items[n];
}

template<class T>
T* DArray<T>::begin() const
{
    return &m_items[0];
}

template<class T>
T* DArray<T>::end() const
{
    return &m_items[0]+m_size;
}

template<class T>
const T* DArray<T>::cbegin() const
{
    return &m_items[0];
}

template<class T>
const T* DArray<T>::cend() const
{
    return &m_items[0]+m_size;
}

