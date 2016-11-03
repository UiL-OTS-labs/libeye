#include <cxxtest/TestSuite.h>
#include "../eyelog/EyeLog.h"
#include <cstring>

using namespace std;


class StringTestSuite: public CxxTest::TestSuite
{
public:

    typedef std::less<int>      less;
    typedef std::greater<int>   greater;

    void testStringConstructors(void)
    {
        TS_TRACE("Testing string constructors");

        const char* result              = "HelloWorld";
        char*       non_const_result    = "HelloWorld";

        String str0;
        String str1(result);
        String str2(str1.begin(), str1.end());
        String str3(non_const_result);
        String str4(str1);
        String moved("moved");
        String str5(std::move(moved));
        String str6('\n');

        TS_ASSERT_EQUALS(str0, "");
        TS_ASSERT_EQUALS(str1, result);
        TS_ASSERT_EQUALS(str2, result);
        TS_ASSERT_EQUALS(str3, result);
        TS_ASSERT_EQUALS(str4, result);
        TS_ASSERT_EQUALS(str5, "moved");
        TS_ASSERT_EQUALS(str6, "\n");

        TS_TRACE("Finished testing constructors");
    }

    void testStringAssignment(void)
    {
        TS_TRACE("Testing string assignment");

        const char* result              = "HelloWorld";
        String p1("Hello");
        String p2("World");

        String str0 = result;
        String str1 = str0;
        String temp(result);
        String str2 = std::move(temp);
        String str3 (p1);
        str3 += p2;
        
        TS_ASSERT_EQUALS(str0, result);
        TS_ASSERT_EQUALS(str1, result);
        TS_ASSERT_EQUALS(str2, result);
        TS_ASSERT_EQUALS(str3, result);

        TS_TRACE("Finished testing assignment");
    }

    void testStringModifications()
    {
        TS_TRACE("Testing string assignment");
        String h = "h";
        String w = "elloworld";
        String result = "helloworld";

        String s1 = h;
        for (const auto& c : w)
            s1.push_back(c);

        String s2 = result;
        for (auto i = 0u; i < w.size(); ++i)
            s2.pop_back();
        
        TS_ASSERT_EQUALS(s1, result);
        TS_ASSERT_EQUALS(s2, h);
        // String should always represent a null terminated string.
        TS_ASSERT_EQUALS(s2[s2.size()], '\0');
    }

    void testStringErasing()
    {
        TS_TRACE("Testing string erasing");
        //                 0123456789
        String sentence = "Erase a bit.";
        String erase    = sentence;
        String::iterator it = erase.erase(erase.begin() + 5, erase.end());
        TS_ASSERT_EQUALS(erase, "Erase");
        TS_ASSERT_EQUALS(it, erase.end());
        TS_ASSERT_EQUALS(*it, '\0');
        TS_ASSERT_EQUALS(erase.length(), String(erase).size() );

        erase = sentence;
        it    = erase.erase(erase.begin() + 6, erase.begin() + 8);
        TS_ASSERT_EQUALS(erase, "Erase bit.");
        TS_ASSERT_EQUALS(it, erase.begin() + 6);
        TS_ASSERT_EQUALS(*it, 'b');
        TS_ASSERT_EQUALS(erase.length(), sentence.size() - 2 );
        
        erase = sentence;
        it    = erase.erase(erase.begin() + 6, erase.begin() + 8);
        TS_ASSERT_EQUALS(erase, "Erase bit.");
        TS_ASSERT_EQUALS(it, erase.begin() + 6);
        TS_ASSERT_EQUALS(*it, 'b');

        erase = sentence;
        it    = erase.erase(erase.end(), erase.end());
        TS_ASSERT_EQUALS(erase, sentence);
        TS_ASSERT_EQUALS(it, sentence.end());
        TS_ASSERT_EQUALS(*it, '\0');

        // The other overloads of erase use the upper one internally, thus they
        // should succeed if this succeed.
    }

    void testStringClear()
    {
        String clear = "bla blah.";
        clear.clear();
        TS_ASSERT_EQUALS(clear, String(""));
        TS_ASSERT_EQUALS(*clear.end(), '\0');
        TS_ASSERT_EQUALS(clear.size(), 0);
    }

    void testStringReserve()
    {
        // tests capacity and whether we can insert without reallocation occur.
        String fillme("A bit of content.");
        const String fillme2("A bit of content.");
        const int cap = 100;

        String::iterator it1 = fillme.begin();
        fillme.reserve(cap);
        String::iterator it2 = fillme.begin();
        TS_ASSERT_DIFFERS(it1, it2); // the buffer should be reallocated.
        TS_ASSERT_DIFFERS(fillme.capacity(), fillme2.capacity());
        TS_ASSERT_EQUALS(fillme.capacity(), 100);
        TS_ASSERT_EQUALS(fillme, fillme2);
        TS_ASSERT_EQUALS(fillme.size(), fillme2.size());

        for (int i = fillme.size(); i < cap; i++)
            fillme.push_back('A');
        String::iterator it3 = fillme.begin();
        // now the buffer should not be reallocated.
        TS_ASSERT_EQUALS(it3, it2); // the buffer should NOT be reallocated.
        TS_ASSERT_EQUALS(fillme.capacity(), 100);
    }

    void testStringResize()
    {
        String dentist = "";
        dentist.resize(5, 'A');
        TS_ASSERT_EQUALS(dentist, String("AAAAA"));
        TS_ASSERT_EQUALS(dentist.size(), 5);
        TS_ASSERT_EQUALS(*dentist.end(), '\0');

        // this should more or leave an emtpy string
        dentist.resize(0);
        TS_ASSERT_EQUALS(dentist, String(""));
        TS_ASSERT_EQUALS(dentist.size(), 0);
        TS_ASSERT_EQUALS(*dentist.end(), '\0');

        // appends nullbytes.
        dentist.clear();
        dentist.resize(5);
        String empty;
        //dentist is not empty.
        TS_ASSERT_DIFFERS(dentist, String(""));
        // however compared as C string it should be regarded
        // empty.
        TS_ASSERT_EQUALS(
                strcmp(dentist.c_str(),
                       empty.c_str()
                    ),
                0
                );
        TS_ASSERT_EQUALS(dentist.size(), 5);
        TS_ASSERT_EQUALS(*dentist.end(), '\0');
    }

    void testStringAddition()
    {
        TS_TRACE("Testing string addition/concatenation");
        const String result("Hello, World!");

        const String cp1 = "Hello";
        const String cp2 = ", ";
        const String cp3 = "World!";
        String res = cp1 + cp2;
        res = res + cp3;

        TS_ASSERT_EQUALS(res, result);
        TS_ASSERT_EQUALS(res[res.size()], '\0');
        
        String mp1 = "Hello, ";
        String mp2 = "World!";
        res = std::move(mp1) + std::move(mp2);
        TS_ASSERT_EQUALS(res, result);
        TS_ASSERT_EQUALS(res[res.size()], '\0');

        mp1 = "Hello, ";
        mp2 = "World!";
        res = mp1 + std::move(mp2);
        TS_ASSERT_EQUALS(res, result);
        TS_ASSERT_EQUALS(mp1.size(), 7);
        TS_ASSERT_EQUALS(res[res.size()], '\0');

        res = cp1 + cp2 + cp3;
        res.pop_back();
        res = res + '!';
        TS_ASSERT_EQUALS(res, result);
        TS_ASSERT_EQUALS(res[res.size()], '\0');
        TS_TRACE("Finshed testing string addition/concatenation");
    }

    void testStringInsert()
    {
        TS_TRACE("Testing string insertion");

        //               01234567890123
        String result = "Hallo, Wereld!";
        String hallo  = "HalloWereld";
        const String c= "HalloWereld";
        String punct  = ", ";
        String exclam = "!";

        hallo.insert(hallo.begin() + 5, punct.begin(), punct.end());
        TS_ASSERT_EQUALS(hallo, "Hallo, Wereld");
        TS_ASSERT_EQUALS(hallo.size(), c.size() + 2 );
        TS_ASSERT_EQUALS(*hallo.end(), '\0');

        hallo.insert(hallo.end(), exclam.begin(), exclam.end());
        TS_ASSERT_EQUALS(hallo, result);
        TS_ASSERT_EQUALS(hallo.size(), result.size());
        TS_ASSERT_EQUALS(*hallo.end(), '\0');
        
        TS_TRACE("Finished Testing string insertion");
    }

};
