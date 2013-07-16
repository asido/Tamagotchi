#ifndef __FONT_MANAGER_TEST_H__
#define __FONT_MANAGER_TEST_H__

#include <cppunit/extensions/HelperMacros.h>

class FontManager;

//-----------------------------------------------------------------------------------------------------------
//  class FontManagerTest
//-----------------------------------------------------------------------------------------------------------

class FontManagerTest : public CPPUNIT_NS::TestFixture
{
    CPPUNIT_TEST_SUITE(FontManagerTest);
    CPPUNIT_TEST(FindFontTest);
    CPPUNIT_TEST_SUITE_END();

public:
    void setUp();
    void tearDown();

    void FindFontTest();

private:
    FontManager *fontMgr;
};

#endif // __FONT_MANAGER_TEST_H__