#ifndef __ZIPFILETEST_H__
#define __ZIPFILETEST_H__

#include <cppunit/extensions/HelperMacros.h>

class ZipFile;

class ZipFileTest : public CPPUNIT_NS::TestFixture
{
    CPPUNIT_TEST_SUITE(ZipFileTest);
    CPPUNIT_TEST(GetNumFilesTest);
    CPPUNIT_TEST(GetFilenameTest);
    CPPUNIT_TEST(GetFileLenTest);
    CPPUNIT_TEST(ReadFileTest);
    CPPUNIT_TEST(FindTest);
    CPPUNIT_TEST_SUITE_END();

public:
    void setUp();
    void tearDown();

    void GetNumFilesTest();
    void GetFilenameTest();
    void GetFileLenTest();
    void ReadFileTest();
    void FindTest();

protected:
    ZipFile *zipFile;
};

#endif // __ZIPFILETEST_H__