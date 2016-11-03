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

class PGaze {
    
    public:

        PGaze(const Coordinate& coor);
        float getX() const;
        float getY() const;
        void setX(float x);
        void setY(float y);

    protected:

        PCoordinate m_coor;
};

/**
 * Get information about a fixation.
 * 
 * Fixations are mainly charachterized by
 * there, position and duration.
 */
class PFixation{

    public:

        PFixation(double dur, float x, float y);
        double getDuration() const;
        float getX() const;
        float getY() const;

    protected:

        double      m_dur;
        PCoordinate m_coor;
};
