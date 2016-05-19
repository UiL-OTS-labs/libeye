/*
 * PExperiment.h
 *
 * Provides a Experiment and a trial. An experiment is a container
 * for trials and trials are logentries separated on LogEntry.
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

#include"constants.h"
#include"PEyeLogEntry.h"
#include"PEyeLog.h"
#include<vector>
#include<map>

/**
 * This class represents a trial in an experiment.
 */
class PTrial {
    
    public:

        /**
         * Create a PTrial from a PTrialEntry
         */
        PTrial (std::shared_ptr<PTrialEntry> entry);

        /**
         * Destroy a PTrial instance
         */
        virtual ~PTrial();

        /**
         * Add a new entry to the entries in this trial.
         *
         * This clones the entry and puts it in the correct
         * vector with log entries.
         */
        void addEntry(const PEntryPtr entry);

        /**
         * returns a vector with specific entrytype
         *
         * Returns a vector with a specific entrytype. If no such
         * entry type is added, the PTrial returns an empty 
         * vector.
         */
        const PEntryVec& operator[](entrytype type) const;

        /**
         * Returns all the entries that belong to this trial.
         */
        PEntryVec getEntries() const;

        /**
         * Clears all logentries.
         */
        void clear();

        /**
         * compares this trial to another trial
         */
        bool operator ==(const PTrial& rhs) const;

        /**
         * Confirms whether a trial is not the same as another trial.
         */
        bool operator !=(const PTrial& rhs) const;

        /**
         * Gets the trial identifier, specifies a specific trial.
         *
         * The trial identifier can be used to identify a specific
         * trial.
         */
        std::string getIdentifier() const;
        
        /**
         * Gets the group identifier, specifies a specific trial.
         *
         * The trial identifier can be used to identify a specific
         * trial.
         */
        std::string getGroup()const;

    private:

        /**
         * The trial entry belonging to this trial.
         */
        std::shared_ptr<PTrialEntry> m_entry;

        /**
         * This map contains all entries that belong to this trial.
         */
        std::map<entrytype, PEntryVec> m_entries;

};

/**
 * An instance of PExperiment contains a number of trials.
 */
class PExperiment {

    public:

        /**
         * Create an empty PExperiment.
         */
        PExperiment();
        
        /**
         * Create an experiment from an vector of PEyeLogEntry.
         */
        PExperiment(const PEntryVec& entries);
        
        /**
         * Create an experiment from PEyeLog instance
         */
        PExperiment(const PEyeLog& log);
        
        /**
         * compares this experiment to another experiment
         */
        bool operator ==(const PExperiment& rhs) const;

        /**
         * Confirms whether a experiment is not the same as another experiment.
         */
        bool operator !=(const PExperiment& rhs) const;

        /**
         * Return the experiment in a log suitable for saving.
         *
         * \param output [out] The output will be initialized
         * \param append [in]  If append is true all entries
         *                     will be appended to the log.
         *                     otherwise it is cleared.
         */
        void getLog(PEyeLog& output, bool append=false)const;

        /**
         * tells how many trials are in the experiment.
         */
        unsigned nTrials()const;

        /**
         * Gets the trial identifier
         */
        std::string identifier()const;

        /**
         * Gets the group identifier
         */
        std::string group()const;
    
    private:

        /**
         * initialize a PExperiment with a vector of PEyeLogEntries
         */
        void initFromEntryVec(const PEntryVec& entries);

        /**
         * Data before the first trial is considered meta data
         */
        PEntryVec               m_metadata;

        /**
         * All the trials in the experiment.
         */
        std::vector<PTrial>     m_trials;
};
