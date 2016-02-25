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

#ifndef PCOORDINATE_H
#define PCOORDINATE_H


/**
 * PCoordinate a simple class that can specify a coordinate in 2D space
 */
class PCoordinate {

    public:

        PCoordinate();
        PCoordinate(float x, float y);

        float getX()const;
        float getY()const;
        void setX(float x);
        void setY(float y);

    private:

        float   m_x;
        float   m_y;
};

#endif
