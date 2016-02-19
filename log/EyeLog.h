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

#include "cEyeLog.h"
#include "constants.h"

#include <string>
#include <vector>
#include <memory>
#include <cstdlib>

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

        virtual std::string toString()const 
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

        void addEntry(const EyeLogEntry& e)
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
