#include <memory>
#include <cppunit/config/SourcePrefix.h>
#include "FontManagerTest.h"
#include "Rendering/FontManager.h"
#include "Rendering/Font.h"

CPPUNIT_TEST_SUITE_REGISTRATION(FontManagerTest);

void FontManagerTest::setUp()
{
    this->fontMgr = new FontManager;
}

void FontManagerTest::tearDown()
{
    delete this->fontMgr;
}

void FontManagerTest::FindFontTest()
{
    std::shared_ptr<Font> font = this->fontMgr->FindFont("Arial", 18);
    CPPUNIT_ASSERT(font);
}