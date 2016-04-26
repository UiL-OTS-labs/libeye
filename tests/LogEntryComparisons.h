#include <cxxtest/TestSuite.h>
#include "../log/EyeLog.h"

class LogEntryComparisonSuite: public CxxTest::TestSuite
{
public:

    typedef std::less<int>      less;
    typedef std::greater<int>   greater;

    void testGazeEntry(void)
    {
        TS_TRACE("Testing gazeentries");
        GazeEntry gaze1(LGAZE, 0.0, 10, 10, 10);
        GazeEntry gaze2(LGAZE, 0.0, 10, 10, 10);
        GazeEntry gaze3(LGAZE, 1.0, 10, 10, 10);
        GazeEntry gaze4(LGAZE, 0.0,  0, 10, 10);
        GazeEntry gaze5(RGAZE, 0.0, 10, 10, 10);

        TS_ASSERT_EQUALS(gaze1, gaze2);
        TS_ASSERT_DIFFERS(gaze1, gaze3);
        TS_ASSERT_LESS_THAN(gaze1, gaze3);
        TS_ASSERT_LESS_THAN(gaze4, gaze1);
        TS_ASSERT_LESS_THAN(gaze1, gaze5);
        TS_TRACE("Finished testing gazeentries");
    }

    void testGazeCompare()
    {
        TS_TRACE("Testing gazeentries compare");
        GazeEntry gaze1(LGAZE, 0.0, 10, 10, 10);
        GazeEntry gaze2(LGAZE, 0.0, 10, 10, 10);
        GazeEntry gaze3(LGAZE, 1.0, 10, 10, 10);
        GazeEntry gaze4(LGAZE, 0.0,  0, 10, 10);
        GazeEntry gaze5(RGAZE, 0.0, 10, 10, 10);

        TS_ASSERT_EQUALS(gaze1.compare(gaze2), 0);
        TS_ASSERT_RELATION( less, gaze1.compare(gaze3), 0);
        TS_ASSERT_RELATION( less, gaze4.compare(gaze1), 0);
        TS_ASSERT_RELATION( less, gaze1.compare(gaze5), 0);
        TS_TRACE("Finished testing gazeentries compare");
    }

    void testFixationCompare()
    {
        TS_TRACE ("Testing fixationentries compare");
        FixationEntry fix1(LFIX, 0.0 , 10, 10, 10);
        FixationEntry fix2(LFIX, 10.0, 10, 10, 10);
        FixationEntry fix3(RFIX, 0.0 , 10, 10, 10);

        TS_ASSERT_EQUALS(fix1.compare(fix1), 0);
        TS_ASSERT_RELATION( less   , fix1.compare(fix2), 0);
        TS_ASSERT_RELATION( greater, fix2.compare(fix1), 0);
        TS_ASSERT_RELATION( less   , fix1.compare(fix3), 0);
        TS_ASSERT_RELATION( greater, fix3.compare(fix1), 0);

        TS_TRACE ("Finished Testing fixationentries compare");
    }
};
