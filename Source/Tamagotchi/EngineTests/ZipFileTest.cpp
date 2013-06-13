#include <cppunit/config/SourcePrefix.h>
#include "ZipFileTest.h"
#include "ResourceMgr/ZipFile.h"
#include "TestMain.h"

CPPUNIT_TEST_SUITE_REGISTRATION(ZipFileTest);

void ZipFileTest::setUp()
{
	this->zipFile = new ZipFile();
    CPPUNIT_ASSERT(this->zipFile->Init("ZipFileTest.zip"));
}

void ZipFileTest::tearDown()
{
	delete this->zipFile;
}

void ZipFileTest::GetNumFilesTest()
{
	CPPUNIT_ASSERT(this->zipFile->GetNumFiles() == 7);
}

void ZipFileTest::GetFilenameTest()
{
	int fileIndex = this->zipFile->Find("test.txt");
	std::string filename = this->zipFile->GetFilename(fileIndex);
	CPPUNIT_ASSERT(filename == "test.txt");
}

void ZipFileTest::GetFileLenTest()
{
	int fileIndex = this->zipFile->Find("test.txt");
	int size = this->zipFile->GetFileLen(fileIndex);
	CPPUNIT_ASSERT(size == 9);
}

void ZipFileTest::ReadFileTest()
{
	int fileIndex = this->zipFile->Find("test.txt");
	int size = this->zipFile->GetFileLen(fileIndex);
	CPPUNIT_ASSERT(size != -1);
	char *buf = new char[size+1];
	memset(buf, 0, size+1);

	CPPUNIT_ASSERT(this->zipFile->ReadFile(fileIndex, buf));
	CPPUNIT_ASSERT(strlen(buf) == 9);
	std::string text(buf);
	CPPUNIT_ASSERT(text == "It works!");

	delete[] buf;
}

void ZipFileTest::FindTest()
{

}