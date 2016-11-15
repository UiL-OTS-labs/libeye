

#include <cxxtest/TestSuite.h>

#include <vector>
#include <thread>

#include "../baselib/SharedPtr.h"
#include "../eyelog/PEyeLogEntry.h"

using namespace eye;

extern template class SharedPtr<int>;


void reffunction(int n, SharedPtr<int> ptr)
{
    std::vector<SharedPtr<int> > refs;
    for (int i = 0; i < n; i++)
        refs.push_back(ptr);
}

class SharedPtrSuite : public CxxTest::TestSuite
{

    class RefIt {
        public:
            void operator()(int n, SharedPtr<int> ptr)
            {
                std::vector<SharedPtr<int> > refs;
                for (int i = 0; i < n; i++)
                    refs.push_back(ptr);
            }
    };

public:
    void testSharedPtr()
    {
        TS_TRACE("Testing the shared pointer.");
        
        int* pint = new int(3);
        SharedPtr<int> sptr(pint);
        TS_ASSERT_EQUALS(*sptr, 3)
        TS_ASSERT_EQUALS(sptr.get(), pint);
    }

    void testSelfAsignment()
    {
        int* pint = new int(5);
        SharedPtr<int> sp1 (pint);
        sp1 = sp1;
        TS_ASSERT_EQUALS(sp1.get(), pint);
        TS_ASSERT_EQUALS(sp1.refcount(), 1);

        SharedPtr<int> sp2 (sp1);
        TS_ASSERT_EQUALS(sp1.refcount(), 2);
        TS_ASSERT_EQUALS(sp2.refcount(), 2);
        sp1 = sp2;
        TS_ASSERT_EQUALS(sp1.refcount(), 2);
        TS_ASSERT_EQUALS(sp2.refcount(), 2);
        TS_ASSERT_EQUALS(sp2.get(), pint);
        // Hopefully it doesn't crash here....
    }

    void testNullPtrInit()
    {
        TS_TRACE("Testing the nullptr init.");
        SharedPtr<int> p1(nullptr);
        SharedPtr<int> p2(p1);
        SharedPtr<int> p3 = p2;
    }
    
    void testAtomicity()
    {
        TS_TRACE("Testing whether ref counting is indeed atomic.");

        const int nthreads = 100;
        const int nrefs    = 10000;

        SharedPtr<int> pint(new int(300));
        std::vector<std::thread> threads;
        for (int i = 0; i < nthreads; i++)
            threads.push_back(std::thread(reffunction, nrefs, pint));

        for (int i = 0; i < nthreads; i++)
            threads[i].join();
        
        TS_ASSERT_EQUALS(pint.refcount(), 1);
    }

    void testCopyConstructor()
    {
        TS_TRACE("Testing copy constructor.");
        SharedPtr<int> p1( new int(18));
        {
            SharedPtr<int> p2(p1);
            TS_ASSERT_EQUALS(*p1, *p2);
            TS_ASSERT_EQUALS(p1.get(), p2.get());
        }
        TS_ASSERT_EQUALS(1, p1.refcount());
    }
    
    void testAssignment()
    {
        TS_TRACE("Testing assignment.");
        SharedPtr<int> p1(new int(18));
        {
            SharedPtr<int> p2;
            p2 = p1;
            TS_ASSERT_EQUALS(*p1, *p2);
            TS_ASSERT_EQUALS(p1.get(), p2.get());
            TS_ASSERT_EQUALS(p2.refcount(), 2);
            TS_ASSERT_EQUALS(p2.refcount(), p1.refcount());

            // check whether an already initialized version works
            SharedPtr<int> p3(new int(4));
            TS_ASSERT_DIFFERS(*p3, *p1);
            TS_ASSERT_DIFFERS(p3.get(), p1.get());
            p3 = p1;
            TS_ASSERT_EQUALS(*p1, *p3);
            TS_ASSERT_EQUALS(p1.get(), p3.get());
        }
        TS_ASSERT_EQUALS(1, p1.refcount());
    }

    void testComparisons()
    {
        SharedPtr<int>p1(new int(1));
        SharedPtr<int>p2(new int(2));

        TS_ASSERT_EQUALS(p1 < p2, true);
        TS_ASSERT_EQUALS(p2 > p1, true);

        SharedPtr<int>p3(new int(2));
        SharedPtr<int>p4;

        bool b3;
        bool b4;

        if (p3)
            b3 = true;
        else
            b3 = false;

        if (p4)
            b4 = true;
        else
            b4 = false;

        TS_ASSERT_EQUALS(bool(p3), true)
        TS_ASSERT_EQUALS(bool(p4), false)
        TS_ASSERT_EQUALS(b3, true);
        TS_ASSERT_EQUALS(b4, false);
        TS_ASSERT_EQUALS(p3 != nullptr, true);
        TS_ASSERT_EQUALS(p4 , nullptr);
        TS_ASSERT_EQUALS(p2, p3);
    }
};
