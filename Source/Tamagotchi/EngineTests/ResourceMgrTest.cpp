#include <cppunit/config/SourcePrefix.h>
#include "ResourceMgrTest.h"
#include "ResourceMgr/ResourceMgr.h"
#include "ResourceMgr/ZipFile.h"

#include "TestMain.h"

CPPUNIT_TEST_SUITE_REGISTRATION(ResourceMgrTest);

void ResourceMgrTest::setUp()
{
	std::shared_ptr<IResourceFile> resourceFile = std::shared_ptr<IResourceFile>(new ResourceZipFile("ZipFileTest.zip"));
	this->resourceMgr = new ResourceMgr(static_cast<unsigned int>(MB_TO_B(50)), resourceFile);
	CPPUNIT_ASSERT(this->resourceMgr->Init());
}

void ResourceMgrTest::tearDown()
{
	delete this->resourceMgr;
}

void ResourceMgrTest::GetHandleTest()
{
	Resource r1("test.txt");
	std::shared_ptr<ResourceHandle> handle = this->resourceMgr->GetHandle(r1);
	CPPUNIT_ASSERT(!handle); // We don't have .txt loader.

	Resource r2("stdafx.xml");
	handle = this->resourceMgr->GetHandle(r2);
	CPPUNIT_ASSERT(handle);

	Resource r3("unexisting.file");
	handle = this->resourceMgr->GetHandle(r3);
	CPPUNIT_ASSERT(!handle);
}