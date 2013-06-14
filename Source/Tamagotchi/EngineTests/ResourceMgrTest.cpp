#include <cppunit/config/SourcePrefix.h>
#include "ResourceMgrTest.h"
#include "ResourceMgr/ResourceManager.h"
#include "ResourceMgr/ZipFile.h"
#include "ResourceMgr/TextureResource.h"
#include "defines.h"
#include "TestMain.h"

CPPUNIT_TEST_SUITE_REGISTRATION(ResourceMgrTest);

void ResourceMgrTest::setUp()
{
	std::shared_ptr<IResourceFile> resourceFile = std::shared_ptr<IResourceFile>(new ResourceZipFile("ZipFileTest.zip"));
	this->resourceMgr = new ResourceManager(static_cast<unsigned int>(MB_TO_B(50.0f)), resourceFile);
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

	Resource r4("1.png");
	handle = this->resourceMgr->GetHandle(r4);
	CPPUNIT_ASSERT(handle);
	std::shared_ptr<GLESTextureResourceExtraData> textureExtra = std::static_pointer_cast<GLESTextureResourceExtraData>(handle->GetExtra());
	CPPUNIT_ASSERT(textureExtra->GetTexture() > 0);
}