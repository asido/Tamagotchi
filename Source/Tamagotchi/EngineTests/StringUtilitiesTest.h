#ifndef __STRINGUTILITIESTEST_H__
#define __STRINGUTILITIESTEST_H__

#include <cppunit/extensions/HelperMacros.h>

class StringUtilitiesTest : public CPPUNIT_NS::TestFixture
{
	CPPUNIT_TEST_SUITE(StringUtilitiesTest);
	CPPUNIT_TEST(WildcardMatchTest);
	CPPUNIT_TEST_SUITE_END();

public:
	void WildcardMatchTest();
};

#endif // __STRINGUTILITIESTEST_H__