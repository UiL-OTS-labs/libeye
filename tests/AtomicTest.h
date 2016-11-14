

#include <cxxtest/TestSuite.h>

#include <thread>
#include <vector>
#include <functional>

#include "../baselib/Atomic.h"

using namespace eye;

template<class T>
class Incrementer
{
    private:
        T*  m_pcounter;
        int m_incs;

    public:

        Incrementer(T* cntr, int n) : m_pcounter(cntr), m_incs(n) {}

        void operator()()
        {
            for (int i = 0; i < m_incs; i++)
                m_pcounter->increment();

            for (int i = 0; i < m_incs; i++)
                m_pcounter->decrement();
        }
};

class AtomicSuite : public CxxTest::TestSuite {

    private:
        
        const int nincs = 10000;    // number of increments (and decrements)
                                    // per thread.
        const int nt    = 1000;      // number of threads to use.

    public:

        void testAtomicity()
        {
            using namespace std;
            TS_TRACE("Testing Atomic counter");
            AtomicCounter cnt;
            std::vector<thread> threads;
            Incrementer<AtomicCounter> counter(&cnt, nincs);
            for (int i = 0; i < nt; ++i)
                threads.push_back(thread(counter));
            
            for (int i = 0; i < nt; i++)
                threads[i].join();

            TS_ASSERT_EQUALS(cnt.count(), 0);
        }
        
        void testAtomicityII()
        {
            using namespace std;
            TS_TRACE("Testing Atomic counter with non default constructor");
            AtomicCounter cnt(2);
            std::vector<thread> threads;
            Incrementer<AtomicCounter> counter(&cnt, nincs);
            for (int i = 0; i < nt; ++i)
                threads.push_back(thread(counter));
            
            for (int i = 0; i < nt; i++)
                threads[i].join();

            TS_ASSERT_EQUALS(cnt.count(), 2);
        }
        

};
