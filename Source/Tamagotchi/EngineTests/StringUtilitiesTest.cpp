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

void StringUtilitiesTest::ToLowerTest()
{
	CPPUNIT_ASSERT(StringUtilities::ToLower(std::string("ABCdefGHIj")) == "abcdefghij");
}

void StringUtilitiesTest::FormatTest()
{
	CPPUNIT_ASSERT(StringUtilities::Format("Test 1: 0x%X", 0x567a) == std::string("Test 1: 0x567A"));
	CPPUNIT_ASSERT(StringUtilities::Format("%d %c Test 2 %d", 1, 0x41, 10) == std::string("1 A Test 2 10"));
}