#include <cppunit/config/SourcePrefix.h>
#include "StringUtilitiesTest.h"
#include "StringUtilities.h"

CPPUNIT_TEST_SUITE_REGISTRATION(StringUtilitiesTest);

void StringUtilitiesTest::WildcardMatchTest()
{
	CPPUNIT_ASSERT(StringUtilities::WildcardMatch("*.xml", "File.xml"));
	CPPUNIT_ASSERT(!StringUtilities::WildcardMatch("*.xml", "File.jpg"));

	CPPUNIT_ASSERT(!StringUtilities::WildcardMatch("*.png", "File.png.jpg"));
	CPPUNIT_ASSERT(StringUtilities::WildcardMatch("File.png", "File.png"));
	CPPUNIT_ASSERT(StringUtilities::WildcardMatch("*File.png", "ElifFile.png"));
}