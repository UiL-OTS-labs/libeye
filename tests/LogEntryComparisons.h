#include <cxxtest/TestSuite.h>
#include "../eyelog/EyeLog.h"

class LogEntryComparisonSuite: public CxxTest::TestSuite
{
public:

    typedef std::less<int>      less;
    typedef std::greater<int>   greater;

    void testGazeEntry(void)
    {
        TS_TRACE("Testing gazeentries");
        PGazeEntry gaze1(LGAZE, 0.0, 10, 10, 10);
        PGazeEntry gaze2(LGAZE, 0.0, 10, 10, 10);
        PGazeEntry gaze3(LGAZE, 1.0, 10, 10, 10);
        PGazeEntry gaze4(LGAZE, 0.0,  0, 10, 10);
        PGazeEntry gaze5(RGAZE, 0.0, 10, 10, 10);

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
        PGazeEntry gaze1(LGAZE, 0.0, 10, 10, 10);
        PGazeEntry gaze2(LGAZE, 0.0, 10, 10, 10);
        PGazeEntry gaze3(LGAZE, 1.0, 10, 10, 10);
        PGazeEntry gaze4(LGAZE, 0.0,  0, 10, 10);
        PGazeEntry gaze5(RGAZE, 0.0, 10, 10, 10);

        TS_ASSERT_EQUALS(gaze1,gaze2);
        TS_ASSERT_LESS_THAN(gaze1, gaze3);
        TS_ASSERT_LESS_THAN(gaze4, gaze1);
        TS_ASSERT_LESS_THAN(gaze1, gaze5);
        TS_TRACE("Finished testing gazeentries compare");
    }

    void testFixationCompare()
    {
        TS_TRACE ("Testing fixationentries compare");
        PFixationEntry fix1(LFIX, 0.0 , 10, 10, 10);
        PFixationEntry fix2(LFIX, 10.0, 10, 10, 10);
        PFixationEntry fix3(RFIX, 0.0 , 10, 10, 10);

        TS_ASSERT_EQUALS(fix1, fix1);
        TS_ASSERT_LESS_THAN( fix1, fix2);
        TS_ASSERT_LESS_THAN( fix1, fix3);
        TS_ASSERT_LESS_THAN( fix3, fix2);

        TS_TRACE ("Finished Testing fixationentries compare");
    }
};
