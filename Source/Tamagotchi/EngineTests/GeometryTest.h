#ifndef __GEOMETRY_TEST_H__
#define __GEOMETRY_TEST_H__

#include <cppunit/extensions/HelperMacros.h>

//-----------------------------------------------------------------------------------------------------------
//  class GeometryTest
//-----------------------------------------------------------------------------------------------------------

class GeometryTest : public CPPUNIT_NS::TestFixture
{
    CPPUNIT_TEST_SUITE(GeometryTest);
    CPPUNIT_TEST(MatrixStackTest);
    CPPUNIT_TEST_SUITE_END();

public:
    void MatrixStackTest();
};

#endif // __GEOMETRY_TEST_H__