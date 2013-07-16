#include <cppunit/config/SourcePrefix.h>
#include <Eigen/Dense>
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
    std::string str("ABCdefGHIj");
    CPPUNIT_ASSERT(StringUtilities::ToLower(str) == std::string("abcdefghij"));
}

void StringUtilitiesTest::FormatTest()
{
    CPPUNIT_ASSERT(StringUtilities::Format("Test 1: 0x%X", 0x567a) == std::string("Test 1: 0x567A"));
    CPPUNIT_ASSERT(StringUtilities::Format("%d %c Test 2 %d", 1, 0x41, 10) == std::string("1 A Test 2 10"));
}

void StringUtilitiesTest::HashTest()
{
    unsigned int expected = StringUtilities::Hash("abcdef");
    CPPUNIT_ASSERT(StringUtilities::Hash("abcdef") == expected);
    CPPUNIT_ASSERT(StringUtilities::Hash("ABCDEF") == expected);
    CPPUNIT_ASSERT(StringUtilities::Hash("fedcba") != expected);
}