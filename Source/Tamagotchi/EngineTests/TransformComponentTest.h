#ifndef __TRANSFORMCOMPONENTTEST_H__
#define __TRANSFORMCOMPONENTTEST_H__

#include <cppunit/extensions/HelperMacros.h>

class TransformComponent;

class TransformComponentTest : public CPPUNIT_NS::TestFixture
{
    CPPUNIT_TEST_SUITE(TransformComponentTest);
    CPPUNIT_TEST(SetTransformTest);
    CPPUNIT_TEST(SetPositionTest);
    CPPUNIT_TEST_SUITE_END();

public:
    void setUp();
    void tearDown();

    void SetTransformTest();
    void SetPositionTest();

protected:
    TransformComponent *transformComponent;
};

#endif // __TRANSFORMCOMPONENTTEST_H__