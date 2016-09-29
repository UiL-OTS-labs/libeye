/*
 * Experiment.cpp
 *
 * Provides implementation of an Eyetracking experiment.
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

#include <cassert>
#include "PExperiment.h"
#include "PEyeLogEntry.h"

PTrial::PTrial(const PTrialEntry* entry)
    : m_entry(*entry)
{
}

PTrial::PTrial(const PTrialEntry& entry)
    : m_entry(entry)
{
}

PTrial::PTrial()
    : m_entry()
{
}

PTrial::~PTrial()
{
    for (auto& pair : m_entries)
        destroyPEntyVec(pair.second);
}

void PTrial::addEntry(const PEntryPtr entry)
{
    entrytype t = entry->getEntryType();
    auto it = m_entries.find(t);
    if (it != m_entries.end())
        it->second.push_back(entry->clone());
    m_entries[t] = PEntryVec();
    m_entries[t].push_back(entry->clone());
}

const DArray<PEyeLogEntry*>& PTrial::operator[](entrytype t) const
{
    const static PEntryVec empty; // empty return value.
    const auto it = m_entries.find(t);
    if (it != m_entries.end())
        return it->second;
    else
        return empty;
}

PEntryVec PTrial::getEntries() const
{
    PEntryVec vec;
    vec.push_back(m_entry.clone());
    for (const auto& pair : m_entries)
        for (const auto* entry: pair.second)
            vec.push_back(entry->clone());
    return vec;
}

void PTrial::clear() 
{
    for(auto& pair: m_entries) {
        auto& vec = pair.second;
        destroyPEntyVec(vec);
    }
    m_entries.clear();
}

/*
 * Compares fist on m_entry, than it compares the m_entries map
 */
bool PTrial::operator==(const PTrial& other) const
{
    if (m_entry != other.m_entry)
        return false;

    if (m_entries.size() != other.m_entries.size())
        return false;

    auto it1 = m_entries.cbegin();
    auto it2 = other.m_entries.cbegin();
    /* Compare keys and then deep compare the pointers of the logentries */
    for (; it1 != m_entries.end(); it1++, it2++) {
        if (it1->first != it2->first)
            return false;
        const auto& vec1 = it1->second;
        const auto& vec2 = it2->second;
        if (vec1.size() != vec2.size())
            return false;
        for (unsigned i = 0; i < vec2.size(); i++)
            if (*vec1[i] != *vec2[i])
                return false;
    }
    return true;
}

bool PTrial::operator!=(const PTrial& other) const 
{
    return ! (*this == other);
}

String PTrial::getIdentifier() const
{
    return m_entry.getIdentifier();
}

String PTrial::getGroup() const
{
    return m_entry.getGroup();
}

/* **** implementation of PExperiment **** */

PExperiment::PExperiment()
    :
        m_metadata()
{
}

PExperiment::PExperiment(const PExperiment& rhs)
{
    m_metadata = copyPEntryVec(rhs.m_metadata);
    m_trials = rhs.m_trials;
}

PExperiment::~PExperiment()
{
    destroyPEntyVec(m_metadata);
}

PExperiment::PExperiment(const PEntryVec& entries)
{
    initFromEntryVec(entries);
}

PExperiment::PExperiment(const PEyeLog& log)
{
    initFromEntryVec(log.getEntries());
}

void PExperiment::initFromEntryVec(const PEntryVec& entries)
{
    // Used to indicate trial end.
    bool end(false);

    // The next loop adds all entries prior to the first trial
    // to m_metadata, and subsequently it creates more trials
    // and pushes the new entries to those trials.
    for (const auto& e : entries) {
        if (m_trials.empty() && e->getEntryType() != TRIAL) {
            m_metadata.push_back(e->clone());
            continue;
        }
        switch(e->getEntryType()) {
            case TRIAL:
                end = false;
                m_trials.push_back(PTrial(static_cast<PTrialEntry*>(e)));
                break;
            case TRIALSTART:
                m_trials[m_trials.size() - 1].clear();
                break;
            case TRIALEND:
                end = true;
                break;
            default:
                if(!end)
                    m_trials[m_trials.size()-1].addEntry(e);
        }
    }
}

bool PExperiment::operator==(const PExperiment& rhs) const
{
    if (m_metadata.size() != rhs.m_metadata.size())
        return false;
    for (unsigned i = 0; i < m_metadata.size(); i++)
        if (*m_metadata[i] != *rhs.m_metadata[i])
            return false;
    return m_trials == rhs.m_trials;
}

bool PExperiment::operator!=(const PExperiment& rhs) const
{
    return !(*this == rhs);
}

unsigned PExperiment::nTrials()const
{
    return m_trials.size();
}

void PExperiment::getLog(PEyeLog& log, bool append)const
{
    log.setEntries(m_metadata, !append);
    for (const auto& trial : m_trials)
        log.setEntries(copyPEntryVec(trial.getEntries()), false);
}
