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

PTrial::~PTrial()
{
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

void PTrial::clear() 
{
    for(auto& pair: m_entries) {
        auto& vec = pair.second;
        destroyPEntyVec(vec);
    }
    m_entries.clear();
}

bool PTrial::operator==(const PTrial& other) const
{
    if (m_entries.size() != other.m_entries.size())
        return false;

    auto it1 = m_entries.cbegin();
    auto it2 = other.m_entries.cbegin();
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

std::string PTrial::getIdentifier() const
{
    return m_entry.getIdentifier();
}

std::string PTrial::getGroup() const
{
    return m_entry.getGroup();
}

PExperiment::PExperiment()
{
}

PExperiment::PExperiment(const std::vector<PEyeLogEntry*>& entries)
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
    if (m_metadata.size() == rhs.m_metadata.size())
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

PEyeLog PExperiment::getLog()const
{
    //TODO
    assert(false);
}
