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


/**
 * EyeLogEntry is the base class for all events that occur in a eyemovement
 * experiment.
 *
 * There are many events in an experiment. The events can be characterised
 * by a time on which they occurred and the type of event they are.
 */
class EyeLogEntry {

    public:

        friend class EyeLog;

        /**
         * Construct a EyeLogEntry from another entry.
         *
         * This is a deep copy, thus the two will not point to the same
         * eyelogentry instance.
         */
        EyeLogEntry(const EyeLogEntry& other)
        {
            mentry = eyelog_entry_clone(other.mentry);
        }

        /**
         * Construct a eyelog_entry.
         *
         * This is a shallow copy, thus the two will be identical.
         * Note that the EyeLogEntry will destroy the when deleted.
         * or by raii principals.
         */
        EyeLogEntry(eyelog_entry* e)
            : mentry(e)
        {
        }
        
        /**
         * Destroy this entry.
         */
        virtual ~EyeLogEntry()
        {
            eyelog_entry_destroy(mentry);
        }

        /**
         * Set separator as delimiter in the log messages
         *
         * The separator that is used to delimit columns in a logfile
         * of a non binary log, is shared by all LogEntries.
         * Therefore you should make sure that this is not changed from any
         * thread when you are writing an ascii log.
         *
         * \param c the character used as separator.
         *
         * \Note This function is not thread safe, an log in another
         *       thread might also start using this separator.
         */
        void setSeparator(char c) const
        {
            eyelog_entry_set_separator(mentry, c);
        }

        /**
         * Get the separator that is used as delimiter in the logs.
         *
         * \returns A character that is used to separate columns in a logfile.
         */
        char getSeparator() const
        {
            return eyelog_entry_get_separator(mentry);
        }

        /**
         * Set the precision which is used to store floating point numbers in
         * a logfile.
         *
         * All logentries in a program use the same precision, it is a static
         * member variable that is used to set the precision for floating point
         * numbers
         *
         * Note this function is not thread safe.
         */
        void setPrecision(unsigned precision)
        {
            eyelog_entry_set_precision(mentry, precision);
        }

        /**
         * Get the precision which is used to store floating point numbers in
         * a logfile.
         */
        unsigned getPrecision() const
        {
            return eyelog_entry_get_precision(mentry);
        }

        /**
         * obtain the string format of the logentry.
         */
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

        /**
         * Compares two logentries.
         *
         * This function can be used to compare to entries for
         * sorting purposes. They are sorted on time, entrytype and then
         * some entrytype dependent parameters.
         */
        int compare(const EyeLogEntry& other) const
        {
            return eyelog_entry_compare(mentry, other.mentry);
        }

        /**
         * returns true if this EyeLogEntry should appear before the other
         * EyeLogEntry.
         */
        bool operator< (const EyeLogEntry& other) const
        {
            return bool(eyelog_entry_lt(mentry, other.mentry));
        }

        /**
         * returns true if this EyeLogEntry should appear before the other
         * EyeLogEntry.
         */
        bool operator> (const EyeLogEntry& other) const
        {
            return bool(eyelog_entry_gt(mentry, other.mentry));
        }

        /**
         * \returns true if this is equal to other.
         */
        bool operator==(const EyeLogEntry& other) const
        {
            return bool(eyelog_entry_eq(mentry, other.mentry));
        }

        /**
         * returns true if this is not equal to other.
         */
        bool operator!=(const EyeLogEntry& other) const
        {
            return bool(eyelog_entry_ne(mentry, other.mentry));
        }

    protected:

        /**
         * This holds the representation of the LogEntry.
         */
        eyelog_entry*   mentry;
};

/**
 * A GazeEntry tells about a gaze sample that has occurred during an experiment
 */
class GazeEntry : public EyeLogEntry {
    public:
        
        /**
         * Construct a new GazeEntry
         *
         * \param et        an entrytype must be LGAZE or RGAZE
         * \param time      is time in milliseconds
         * \param x         The x coorinate of the sample
         * \param y         The y coorinate of the sample
         * \param pupsiz    The pupil size of the sample
         */
        GazeEntry(entrytype et, double time, float x, float y, float pupsiz)
            : EyeLogEntry(
                    reinterpret_cast<eyelog_entry*>(
                        gaze_entry_new(et, time, x, y, pupsiz)
                        )
                    )
        {
        }

        /**
         * \return the x value of the sample.
         */
        float getX() const
        {
            return gaze_entry_get_x(getSelfConst());
        }

        /**
         * \return the y value of the sample.
         */
        float getY() const
        {
            return gaze_entry_get_y(getSelfConst());
        }

        /**
         * \return the coordinate of the sample
         */
        GazePoint getCoordinate()const
        {
            return GazePoint(getX(), getY());
        }
        
        /**
         * set the x value of the sample.
         */
        void setX(float x)
        {
            gaze_entry_set_x(getSelf(), x);
        }

        /**
         * set the y value of the sample.
         */
        void setY(float y)
        {
            gaze_entry_set_y(getSelf(), y);
        }

        /**
         * set the coordinate (x and y) value of the sample.
         */
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

/**
 * A FixationEntry tells about a fixation that has occurred during an experiment
 */
class FixationEntry : public EyeLogEntry {
    public:

        /**
         * Construct a new FixationEntry
         *
         * \param et    must be LFIX or RFIX
         * \param time  The starting time in ms.
         * \param dur   The duration in ms of the fixation.
         * \param x     The x coordinate of the fixation.
         * \param y     The y coordinate of the fixation.
         */
        FixationEntry(entrytype et, double time, double dur, float x, float y)
            : EyeLogEntry(
                    reinterpret_cast<eyelog_entry*>(
                        fixation_entry_new(et, time, dur, x, y)
                        )
                    )
        {
        }

        /**
         * \returns the x coordinate of the fixation
         */
        float getX() const
        {
            return fixation_entry_get_x(getSelfConst());
        }
        
        /**
         * \returns the y coordinate of the fixation
         */
        float getY() const
        {
            return fixation_entry_get_y(getSelfConst());
        }

        /**
         * \returns the coordinate of the fixation
         */
        GazePoint getCoordinate() const
        {
            return GazePoint(getX(), getY());
        }
 
        /**
         * set the x coordinate of the fixation
         */
        void setX(float x)
        {
            fixation_entry_set_x(getSelf(), x);
        }

        /**
         * set the y coordinate of the fixation
         */
        void setY(float y)
        {
            fixation_entry_set_y(getSelf(), y);
        }

        /**
         * Set the coordinate (x and y) of the fixation
         */
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

/**
 * A message is a user defined message. It can be displayed a meta information
 * of a trial or experiment.
 */
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

        /**
         * \returns the message of this entry
         */
        std::string getMessage()const throw (std::bad_alloc)
        {
            const message_entry* self= getSelfConst();
            std::shared_ptr<char> m(message_entry_get_message(self), free);
            if (m)
                return std::string(m.get());
            throw std::bad_alloc();
        }

        /**
         * sets the message of this entry
         */
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

/**
 * A SaccadeEntry is an entry that logs information about a fixation.
 *
 * The saccade is characterized by a start and end point in both space and time.
 */
class SaccadeEntry : public EyeLogEntry {
    public :

        /**
         * Construct a new SaccadeEntry
         *
         * \param et        must be LSAC or RSAC
         * \param time      the start in ms in time
         * \param dur       the duration of the saccade
         * \param x1        The x coordinate of the start postion
         * \param y1        The y coordinate of the start postion
         * \param x2        The x coordinate of the end postion
         * \param y2        The y coordinate of the end postion
         */
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

        /**
         * \returns the x value of the start position of the saccade.
         */
        float getX1()const
        {
            return saccade_entry_get_x1(getSelfConst());
        }

        /**
         * \returns the y value of the start position of the saccade.
         */
        float getY1()const
        {
            return saccade_entry_get_y1(getSelfConst());
        }

        /**
         * \returns the coordinate (x and y value) of the start position of the
         * saccade.
         */
        GazePoint getCoordinate1()const
        {
            const saccade_entry* self = getSelfConst();
            return GazePoint(saccade_entry_get_x1(self),
                             saccade_entry_get_y1(self)
                             );
        }

        /**
         * \returns the x value of the end position of the saccade.
         */
        float getX2() const
        {
            return saccade_entry_get_x2(getSelfConst());
        }
        
        /**
         * \returns the y value of the end position of the saccade.
         */
        float getY2() const
        {
            return saccade_entry_get_y2(getSelfConst());
        }

        /**
         * \returns the coordinate (x and y value) of the end position
         * of the saccade.
         */
        GazePoint getCoordinate2() const
        {
            const saccade_entry* self = getSelfConst();
            return GazePoint(saccade_entry_get_x2(self),
                             saccade_entry_get_y2(self)
                             );
        }

        /**
         * set the x value of the start postion of the saccade entry
         */
        void setX1(float x)
        {
            saccade_entry_set_x1(getSelf(), x);
        }
        
        /**
         * set the y value of the start position the saccade entry.
         */
        void setY1(float y)
        {
            saccade_entry_set_y1(getSelf(), y);
        }

        /**
         * set the coordinate of the start position of the saccade entry.
         */
        void setCoordinate1(const GazePoint& g)
        {
            saccade_entry* self = getSelf();
            saccade_entry_set_x1(self, g.x);
            saccade_entry_set_y1(self, g.y);
        }

        /**
         * set the value x value of the end position of the saccade entry
         */
        void setX2(float x)
        {
            saccade_entry_set_x2(getSelf(), x);
        }

        /**
         * set the y value of the end position of the saccade entry.
         */
        void setY2(float y)
        {
            saccade_entry_set_y2(getSelf(), y);
        }

        /**
         * Set the coordinate (x an y value) of the saccade entry.
         */
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

/**
 * A TrialEntry marks a point in time where a new trial starts.
 */
class TrialEntry : public EyeLogEntry {
    
    public:

        /**
         * Construct a new trial entry.
         *
         * \param time          The start in ms of the trial.
         * \param identifier    The identifier of the trial.
         * \param group         The grouping variable of the trial.
         */
        TrialEntry(double time,
                   const std::string& identifier,
                   const std::string& group)
            : EyeLogEntry (
                    reinterpret_cast<eyelog_entry*>(trial_entry_new(
                            time,
                            identifier.c_str(),
                            group.c_str()
                            )
                        )
                    )
        {
        }

        /**
         * Set the identifier of the trial.
         */
        void setIdentifier(const std::string& identifier)
        {
            trial_entry* self = getSelf();
            trial_entry_set_identifier(self, identifier.c_str());
        }

        /**
         * Set the group of the trial.
         */
        void setGroup(const std::string& group)
        {
            trial_entry* self = getSelf();
            trial_entry_set_group(self, group.c_str());
        }

        /**
         * \returns the identifier of the trial.
         */
        std::string getIdentifier()const
        {
            const trial_entry* self = getSelfConst();
            std::shared_ptr<char> i(trial_entry_get_identifier(self), free);
            if (i)
                return std::string(i.get());
            throw std::bad_alloc();
        }
        
        /**
         * returns a string with the grouping variable of the trial.
         */
        std::string getGroup()const
        {
            const trial_entry* self = getSelfConst();
            std::shared_ptr<char> i(trial_entry_get_identifier(self), free);
            if (i)
                return std::string(i.get());
            throw std::bad_alloc();
        }

    private:

        trial_entry* getSelf()
        {
            return reinterpret_cast<trial_entry*>(mentry);
        }
        
        trial_entry* getSelfConst() const
        {
            return reinterpret_cast<trial_entry*>(mentry);
        }

};

/**
 * Marks the start of the trial. It mainly exists to discard LogEntries in a
 * trial that precede this entry in time.
 */
class TrialStartEntry : public EyeLogEntry {
    
    public:

        TrialStartEntry(double time)
            : EyeLogEntry (
                    reinterpret_cast<eyelog_entry*>(
                        trial_start_entry_new(time)
                            )
                        )
        {
        }
};

/**
 * Marks the start of the trial. It mainly exists to discard LogEntries in a
 * trial that do not precede this entry in time.
 */
class TrialEndEntry : public EyeLogEntry {
    
    public:

        /**
         */
        TrialEndEntry(double time)
            : EyeLogEntry (
                    reinterpret_cast<eyelog_entry*>(
                        trial_end_entry_new(time)
                            )
                        )
        {
        }
};


/**
 * The eyelog either save or load logentries from an eyemovement experiment.
 *
 * The eyelog can be used either to load the data of a file from an experiment.
 * On the other hand during a experiment entries can be added to a EyeLog in order
 * to save them to a file.
 */
class EyeLog {

    public:

        /**
         * Construct a new EyeLog instance.
         */
        EyeLog()
        {
            mlog = eye_log_new();
        }

        /**
         * Destruct an EyeLog instance.
         */
        ~EyeLog()
        {
            eye_log_destroy(mlog);
        }

        /**
         * This opens a file in order to save an experiment.
         *
         * \returns 0 if successfull or use eyelog_error to obtain information about the error.
         */
        int open(const std::string& filename)
        {
            return eye_log_open(mlog, filename.c_str());
        }

        /**
         * close the log.
         */
        void close()
        {
            eye_log_close(mlog);
        }
    
        /**
         * This function clears all currently contained logentries inside of 
         * the EyeLog instance.
         */
        void clear()
        {
            eye_log_clear(mlog);
        }

        /**
         * This function can be used to reserve space for growth of the Log.
         *
         * It might be wise to do this as an optimalization. This way one can
         * prevent resizing of an array with many entries at a time critical
         * moment.
         * One could make a calculation of the number of entries are expected
         * during an experiment and create a bit of extra space.
         *
         * \param n The number of entries for which space should be reserved.
         */
        void reserve(unsigned n)
        {
            eye_log_reserve(mlog, n);
        }

        /**
         * Add a new entry to the log.
         *
         * \todo don't clone here, but add it without cloning.
         */
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

        /**
         * Write a the log with a format.
         *
         * \param format, must be FORMAT_BINARY or FORMAT_CSV
         *
         * \returns 0 when succesfull or another number if not.
         */
        int write(eyelog_format format=FORMAT_BINARY)
        {
            return eye_log_write(mlog, format);
        }

        /**
         * Write a the log with a format.
         *
         * \param format, must be FORMAT_BINARY or FORMAT_CSV
         *
         * \returns 0 when succesfull
         */
        int read(const std::string& filename, bool clear=true)
        {
            return eye_log_read(mlog, filename.c_str(), clear);
        }

        /**
         * \returns whether the file has been opened.
         */
        bool isOpen()const
        {
            return eye_log_is_open(mlog);
        }

        /**
         * \returns the filename of the log.
         */
        std::string filename() const
        {
            return eye_log_get_filename(mlog);
        }

/*
 * Swig doesnt like vectors with object without a default constructor.
 */
#ifndef SWIG
        /**
         * Return a vector with all logentries.
         */
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
        /**
         * Return a vector with shared_ptr
         */
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
