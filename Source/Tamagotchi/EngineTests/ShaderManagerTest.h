#ifndef __SHADERMANAGER_TEST_H__
#define __SHADERMANAGER_TEST_H__

#include <cppunit/extensions/HelperMacros.h>

class ShaderManager;

class ShaderManagerTest : public CPPUNIT_NS::TestFixture
{
    CPPUNIT_TEST_SUITE(ShaderManagerTest);
    CPPUNIT_TEST(GetShaderTest);
    CPPUNIT_TEST_SUITE_END();

public:
    void setUp();
    void tearDown();

    void GetShaderTest();

private:
    ShaderManager *shaderMgr;
};

#endif // __SHADERMANAGER_TEST_H__