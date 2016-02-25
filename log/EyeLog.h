/*
 * EyeLog.h
 *
 * Public header that provides Logging for Eyetrackers.
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

#ifndef EYELOG_H
#define EYELOG_H

#include "cEyeLog.h"
#include "cError.h"
#include "constants.h"
#include "Shapes.h"

#include <string>
#include <vector>
#include <memory>
#include <cstdlib>

#include "Shapes.h"

typedef Coordinate<float> GazePoint;

class EyeLogEntry {

    public:

        friend class EyeLog;

        EyeLogEntry(const EyeLogEntry& other)
        {
            mentry = eyelog_entry_clone(other.mentry);
        }
        EyeLogEntry(eyelog_entry* e)
            : mentry(e)
        {
        }

        virtual ~EyeLogEntry()
        {
            eyelog_entry_destroy(mentry);
        }

        void setSeparator(char c) const
        {
            eyelog_entry_set_separator(mentry, c);
        }

        char getSeparator() const
        {
            return eyelog_entry_get_separator(mentry);
        }

        void setPrecision(unsigned precision)
        {
            eyelog_entry_set_precision(mentry, precision);
        }

        unsigned getPrecision() const
        {
            return eyelog_entry_get_precision(mentry);
        }

        virtual std::string toString()const throw(std::bad_alloc)
        {
            std::shared_ptr<char> tempstr(eyelog_entry_to_string(mentry), free);
            if (tempstr) {
                std::string ret(tempstr.get());
                return ret;
            } else {
                throw std::bad_alloc();
            }
        }

    protected:
        

        eyelog_entry*   mentry;
};

class GazeEntry : public EyeLogEntry {
    public:
        GazeEntry(entrytype et, double time, float x, float y, float pupsiz)
            : EyeLogEntry(
                    reinterpret_cast<eyelog_entry*>(
                        gaze_entry_new(et, time, x, y, pupsiz)
                        )
                    )
        {
        }

        float getX() const
        {
            return gaze_entry_get_x(getSelfConst());
        }

        float getY() const
        {
            return gaze_entry_get_y(getSelfConst());
        }

        GazePoint getCoordinate()const
        {
            return GazePoint(getX(), getY());
        }

        void setX(float x)
        {
            gaze_entry_set_x(getSelf(), x);
        }

        void setY(float y)
        {
            gaze_entry_set_y(getSelf(), y);
        }

        void setCoordinate(const GazePoint& g)
        {
            setX(g.x);
            setY(g.y);
        }
    
    private:
        gaze_entry* getSelf()
        {
            return reinterpret_cast<gaze_entry*>(mentry);
        }
        
        const gaze_entry* getSelfConst() const
        {
            return reinterpret_cast<gaze_entry*>(mentry);
        }
};

class FixationEntry : public EyeLogEntry {
    public:
        FixationEntry(entrytype et, double time, double dur, float x, float y)
            : EyeLogEntry(
                    reinterpret_cast<eyelog_entry*>(
                        fixation_entry_new(et, time, dur, x, y)
                        )
                    )
        {
        }

        float getX() const
        {
            return fixation_entry_get_x(getSelfConst());
        }
        
        float getY() const
        {
            return fixation_entry_get_y(getSelfConst());
        }

        GazePoint getCoordinate() const
        {
            return GazePoint(getX(), getY());
        }

        void setX(float x)
        {
            fixation_entry_set_x(getSelf(), x);
        }

        void setY(float y)
        {
            fixation_entry_set_y(getSelf(), y);
        }

        void setCoordinate(const GazePoint& g)
        {
            setX(g.x);
            setY(g.y);
        }

    private:
        fixation_entry* getSelf()
        {
            return reinterpret_cast<fixation_entry*>(mentry);
        }
        
        const fixation_entry* getSelfConst() const
        {
            return reinterpret_cast<fixation_entry*>(mentry);
        }
};

class MessageEntry : public EyeLogEntry {
    public :
        MessageEntry(double time, const std::string& msg)
            : EyeLogEntry(
                    reinterpret_cast<eyelog_entry*>(
                        message_entry_new(time, msg.c_str())
                        )
                    )
        {
        }

        std::string getMessage()const throw (std::bad_alloc)
        {
            const message_entry* self= getSelfConst();
            std::shared_ptr<char> m(message_entry_get_message(self), free);
            if (m)
                return std::string(m.get());
            throw std::bad_alloc();
        }

        void setMessage(const std::string& m)
        {
            message_entry* self = getSelf();
            message_entry_set_message(self, m.c_str());
        }
    private:

        message_entry* getSelf()
        {
            return reinterpret_cast<message_entry*>(mentry);
        }
        
        const message_entry* getSelfConst() const
        {
            return reinterpret_cast<message_entry*>(mentry);
        }
};

class SaccadeEntry : public EyeLogEntry {
    public :
        SaccadeEntry(entrytype et,
                     double time,
                     double dur,
                     float x1,
                     float y1,
                     float x2,
                     float y2
                )
            : EyeLogEntry(
                    reinterpret_cast<eyelog_entry*>(
                        saccade_entry_new( et, time, dur, x1, y1, x2, y2)
                        )
                    )
        {
        }

        float getX1()const
        {
            return saccade_entry_get_x1(getSelfConst());
        }

        float getY1()const
        {
            return saccade_entry_get_y1(getSelfConst());
        }

        GazePoint getCoordinate1()const
        {
            const saccade_entry* self = getSelfConst();
            return GazePoint(saccade_entry_get_x1(self),
                             saccade_entry_get_y1(self)
                             );
        }

        float getX2() const
        {
            return saccade_entry_get_x2(getSelfConst());
        }
        
        float getY2() const
        {
            return saccade_entry_get_y2(getSelfConst());
        }

        GazePoint getCoordinate2() const
        {
            const saccade_entry* self = getSelfConst();
            return GazePoint(saccade_entry_get_x2(self),
                             saccade_entry_get_y2(self)
                             );
        }

        void setX1(float x)
        {
            saccade_entry_set_x1(getSelf(), x);
        }
        
        void setY1(float y)
        {
            saccade_entry_set_y1(getSelf(), y);
        }

        void setCoordinate1(const GazePoint& g)
        {
            saccade_entry* self = getSelf();
            saccade_entry_set_x1(self, g.x);
            saccade_entry_set_y1(self, g.y);
        }

        void setX2(float x)
        {
            saccade_entry_set_x2(getSelf(), x);
        }

        void setY2(float y)
        {
            saccade_entry_set_y2(getSelf(), y);
        }

        void setCoordinate2(const GazePoint& g)
        {
            saccade_entry* self = getSelf();
            saccade_entry_set_x2(self, g.x);
            saccade_entry_set_y2(self, g.y);
        }

    private:

        saccade_entry* getSelf()
        {
            return reinterpret_cast<saccade_entry*>(mentry);
        }

        const saccade_entry* getSelfConst()const
        {
            return reinterpret_cast<saccade_entry*>(mentry);
        }
};

class EyeLog {

    public:
        EyeLog()
        {
            mlog = eye_log_new();
        }

        ~EyeLog()
        {
            eye_log_destroy(mlog);
        }

        int open(const std::string& filename)
        {
            return eye_log_open(mlog, filename.c_str());
        }

        void close()
        {
            eye_log_close(mlog);
        }

        void clear()
        {
            eye_log_clear(mlog);
        }

        void reserve(unsigned n)
        {
            eye_log_reserve(mlog, n);
        }

        void addEntry(const EyeLogEntry& e) throw (std::bad_alloc)
        {
            eyelog_entry* copy = eyelog_entry_clone(e.mentry);
            if (copy)
                eye_log_add_entry(mlog, copy);
            else {
                //assert(false); // TODO error handeling
                throw std::bad_alloc();
            }
        }

        int write(eyelog_format format=FORMAT_BINARY)
        {
            return eye_log_write(mlog, format);
        }

        int read(const std::string& filename, bool clear=true)
        {
            return eye_log_read(mlog, filename.c_str(), clear);
        }

        bool isOpen()const
        {
            return eye_log_is_open(mlog);
        }

        std::string filename() const
        {
            return eye_log_get_filename(mlog);
        }

/*
 * Swig doesnt like vectors with object without a default constructor.
 */
#ifndef SWIG
        std::vector<EyeLogEntry> getEntries()const
        {
            eyelog_entry **entries=NULL;
            unsigned size;
            eye_log_get_entries(mlog, &entries, &size);
            std::vector<EyeLogEntry> ret;
            ret.reserve(size);
            for (unsigned i=0; i<size; i++) {
                ret.emplace_back(EyeLogEntry(eyelog_entry_clone(entries[i])));
            }
            return ret;
        }
#else
        std::vector<std::shared_ptr<EyeLogEntry> > getEntries() const
        {
            eyelog_entry **entries=NULL;
            unsigned size;
            eye_log_get_entries(mlog, &entries, &size);
            std::vector<std::shared_ptr<EyeLogEntry> > ret;
            ret.reserve(size);
            for (unsigned i=0; i<size; i++) {
                ret.push_back(
                        std::make_shared<EyeLogEntry>(
                            eyelog_entry_clone(entries[i])
                                )
                            );
            }
            return ret;
        }
#endif

    private:
        eye_log*    mlog;
};

#endif
