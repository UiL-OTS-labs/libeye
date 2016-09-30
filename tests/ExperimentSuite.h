#include <cxxtest/TestSuite.h>
#include "../log/EyeLog.h"

class ExperimentSuite: public CxxTest::TestSuite
{
public:

    bool entryVecsAreEqual(const PEntryVec& v1, const PEntryVec& v2)
    {
        if (v1.size() != v2.size())
            return false;
        for (auto i = PEntryVec::size_type(0); i < v1.size(); i++) {
            if (v1[i]->compare(*v2[i]) != 0)
                return false;
        }
        return true;
    }

    void testMetaComparison()
    {
        TS_TRACE("Testing PExperiment comparisons");
        PEntryVec meta;
        PEntryVec metacp;
        meta.push_back(new PGazeEntry(LGAZE, 0, 10, 10, 0));
        meta.push_back(new PGazeEntry(RGAZE, 0, 10, 10, 0));
        meta.push_back(new PGazeEntry(LGAZE, 1, 10, 10, 0));
        meta.push_back(new PGazeEntry(RGAZE, 1, 10, 10, 0));
        meta.push_back(new PMessageEntry(2, "Hi"));

        metacp = copyPEntryVec(meta);

        PExperiment expmeta (meta);
        PExperiment expmetacp(metacp);

        // Test whether two identical are identical
        TS_ASSERT_EQUALS(expmeta, expmetacp);
        metacp.push_back(new PTrialEntry());

        // Test whether a length difference compares as unequal
        TS_ASSERT_DIFFERS(expmeta, PExperiment(metacp));

        destroyPEntyVec(metacp);
        metacp = copyPEntryVec(meta);
        delete metacp[3];
        // note that in the next the pupil size if different
        metacp[3] = new PGazeEntry(RGAZE, 1, 10, 10, 1); 
        TS_ASSERT_DIFFERS(expmeta, PExperiment(metacp));
        destroyPEntyVec(metacp);
        destroyPEntyVec(meta);
    }

};


