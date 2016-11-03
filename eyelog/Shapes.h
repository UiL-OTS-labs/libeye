/*
 * Shapes.h
 *
 * Public header that provides an abstraction to some common shapes.
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

#ifndef SHAPES_H
#define SHAPES_H

#include <algorithm>
#include <exception>
#include <limits>


/**
 * This class can be thrown when some of the other classes in this
 * class must do an operation that over/under flows.
 */
class ShapeOverflow : std::exception {
    public:
        const char* what() const throw()
        {
            return "Operation on shape exceeded maximum/minimum value.";
        }
};

/**
 * This class defines a point in a 2D cartesian plane.
 */
template <class T>
class Coordinate {

    public:

        /**
         * Create a coordinate from an x and y coordinate.
         */
        Coordinate (T x, T y)
            : x(x), y(y)
        {
        }

        /**
         * Create a coordinate at the origin.
         */
        Coordinate ()
            : x(0), y(0)
        {
        }

        /**
         * Add two coordinates
         *
         * @return A new coordinate
         */
        Coordinate<T> operator+(const Coordinate<T>& rhs) throw(ShapeOverflow)
        {
            T max = std::numeric_limits<T>::max();
            if (max - rhs.x < x || max - rhs.y < y) {
                throw ShapeOverflow();
            }
            return Coordinate<T>(x + rhs.x, y + rhs.y);
        }
        
        /**
         * Add a coordinate to this coordinate
         *
         * returns itself.
         */
        Coordinate<T>& operator+=(const Coordinate<T>& rhs) throw(ShapeOverflow)                 
        {
            T max = std::numeric_limits<T>::max();
            if (max - rhs.x < x || max - rhs.y < y) {
                throw ShapeOverflow();
            }
            x += rhs.x;
            y += rhs.y;
            return *this;
        }
        
        /**
         * Substract two coordinates
         *
         * @return A new coordinate
         */
        Coordinate<T> operator-(const Coordinate<T>& rhs) throw(ShapeOverflow)
        {
            T min = std::numeric_limits<T>::min();
            if (min + rhs.x > x || min + rhs.y > y) {
                throw ShapeOverflow();
            }
            return Coordinate<T>(x - rhs.x, y - rhs.y);
        }
        
        /**
         * Add a coordinate to this coordinate
         *
         * returns itself.
         */
        Coordinate<T>& operator-=(const Coordinate<T>& rhs) throw(ShapeOverflow)
        {
            T min = std::numeric_limits<T>::min();
            if (min + rhs.x > x || min + rhs.y > y) {
                throw ShapeOverflow();
            }
            x -= rhs.x;
            y -= rhs.y;
            return *this;
        }

        T   x;
        T   y;
};


/**
 * \brief Defines a rectangle
 *
 * The rectangle is defined by a Coordinate in space and
 * its height and width.
 */
template <typename T>
class Rectangle {

    public:

        /**
         * \brief Create a new rectangle from two Coordinates in space
         *
         * The created rectangle always chooses the point with the lowest
         * x and y value of the rectangle.
         */
        Rectangle(Coordinate<T> p1, Coordinate<T> p2)
        {
            point.x = p1.x < p2.x ? p1.x : p2.x;
            point.y = p1.y < p2.y ? p1.y : p2.y;
            width   = p1.x < p2.x ? p2.x - p1.x : p1.x - p2.x;
            height  = p1.y < p2.y ? p2.y - p1.y : p1.y - p2.y;
        }

        /**
         * \brief Create a new rectangle from a 2D Coordinate and a x and y v coordinate.
         *
         * The created rectangle always chooses the point with the lowest
         * x and y value.
         */
        Rectangle(Coordinate<T> p, T nwidth, T nheight)
        {
            point = p;
            width = nwidth;
            height= nheight;
            if (width < 0) {
                point -= Coordinate<T>(width, 0);
                width *= -1;
            }
            if (height < 0) {
                point -= Coordinate<T>(0, height);
                height *= -1;
            }
        }

        /**
         * \brief Creates a new rectangle from an x and y coordinate and it
         * width and height.
         *
         * The created rectangle always chooses the point with the lowest
         * x and y value.
         */
        Rectangle(T x, T y, T nwidth, T nheight) throw (ShapeOverflow)
        {
            point = Coordinate<T>(x, y);
            width = nwidth;
            height= nheight;
            if (width < 0) {
                point -= Coordinate<T>(width, 0);
                width *= -1;
            }
            if (height < 0) {
                point -= Coordinate<T>(0, height);
                height *= -1;
            }
        }

        /**
         * \brief test whether a point is contained by the rectangle.
         *
         * @return true if the point is contained by the rectangle, false
         * otherwise.
         */
        bool contains(T x, T y) const
        {
            if (x < point.x || x >= point.x + width)
                return false;
            if (y < point.y || y >= point.y + height)
                return false;
            return true;
        }

        /**
         * \brief test whether a point is contained by the rectangle.
         *
         * @return true if the point is contained by the rectangle, false
         * otherwise.
         */
        bool contains(const Coordinate<T>& c) const
        {
            return contains(c.x, c.y);
        }

        /**
         * \brief test whether another rectangle is contained by the rectangle.
         *
         * @return true if the point is contained by the rectangle, false
         * otherwise.
         */
        bool contains(const Rectangle<T>& r) const throw(ShapeOverflow)
        {
            return (*this & r).area() == r.area();
        }

        /**
         * \brief computes the area of the rectangle
         *
         * @return the area.
         */
        T area()const throw(ShapeOverflow)
        {
            T max = std::numeric_limits<T>::max();
            if (max / width < height)
                throw ShapeOverflow();
            return width * height;
        }

        /**
         * Get the enclosing rectangle
         *
         * @return A rectangle that is precisely big enhough to encompass
         * both rectangles.
         */
        Rectangle<T> operator|(const Rectangle& rhs) const
        {
            // repectively new x left, y bottom, x right and y top
            T xl = std::min(point.x, rhs.point.x);
            T yb = std::min(point.y, rhs.point.y);
            T xr = std::max(point.x + width,  rhs.point.x + rhs.width); 
            T yt = std::max(point.y + height, rhs.point.y + rhs.height); 
            return Rectangle<T>(Coordinate<T>(xl, yb), Coordinate<T>(xr, yt));
        }

        /**
         * Get the intersecting rectangle
         *
         * @return the rectangle where the two rectangles overlap.
         * If they don't intersect a rect with no width
         * and height is returned.
         */
        Rectangle<T> operator&(const Rectangle& rhs) const
        {
            T xl = std::max(point.x, rhs.point.x);
            T yb = std::max(point.y, rhs.point.y);
            T xr = std::min(point.x + width, rhs.point.x + rhs.width);
            T yt = std::min(point.y + height, rhs.point.y + rhs.height);
            if (xr < xl || yt < yb)
                return Rectangle(0,0,0,0);
            return Rectangle<T>(Coordinate<T>(xl, yb), Coordinate<T>(xr, yt));
        }

        Coordinate<T>   point;
        T               width;
        T               height;
};

#endif

