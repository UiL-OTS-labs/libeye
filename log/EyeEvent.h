/*
 * EyeEvent.h
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

/**
 * Parent class for all types of events in an typical eyemovent experiment.
 */
class EyeEvent {

    public :

        EyeEvent(double time);

        double getTime() const;
        void setTime(double time);
   
    protected :

        double m_time;

};


/**
 * Get information about a fixation.
 * 
 * Fixations are mainly charachterized by
 * there, position and duration.
 */
class Fixation : public EyeEvent {

    public:

        Fixation(double time, double dur, double x, double y);
        double getDuration() const;
        double getX() const;
        double getY() const;

    protected:

        double m_dur;
        double m_x;
        double m_y;
};
