/*
 * PCoordinate.h
 *
 * Public header that provides coordinates to the log classes.
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

#include "PCoordinate.h"

PCoordinate::PCoordinate()
    : m_x(0.0), m_y(0.0)
{
}

PCoordinate::PCoordinate(float x, float y)
    : m_x(x), m_y(y)
{
}

PCoordinate::PCoordinate(const PCoordinate& c)
    : m_x(c.m_x), m_y(c.m_y)
{
}

float PCoordinate::getX() const
{
    return m_x;
}

float PCoordinate::getY() const
{
    return m_y;
}

void PCoordinate::setX(float x)
{
    m_x = x;
}

void PCoordinate::setY(float y)
{
    m_y = y;
}

