/*
 * EyeEvent.cpp
 *
 * Provides events that occur in an eyetracking experiment.
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

#include "EyeEvent.h"

EyeEvent::EyeEvent(double t)
    : m_time(t)
{
}

void EyeEvent::setTime(double t)
{
    m_time = t;
}

double EyeEvent::getTime(void) const
{
    return m_time;
}

Fixation::Fixation(double time, double dur, double x, double y)
    : EyeEvent(time),
      m_dur(dur),
      m_x(x),
      m_y(y)
{
}

double Fixation::getDuration() const
{
    return m_dur;
}

double Fixation::getX()const
{
    return m_x;
}

double Fixation::getY() const
{
}

