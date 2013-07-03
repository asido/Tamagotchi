#include <cppunit/config/SourcePrefix.h>
#include "ResourceMgrTest.h"
#include "ResourceManager/ResourceManager.h"
#include "ResourceManager/ZipFile.h"
#include "ResourceManager/TextureResource.h"
#include "ResourceManager/XMLResource.h"
#include "ResourceManager/ShaderResource.h"
#include "ResourceManager/FontResource.h"
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
    const Resource r1("test.txt");
    std::shared_ptr<ResourceHandle> handle = this->resourceMgr->GetHandle(r1);
    CPPUNIT_ASSERT(!handle); // We don't have .txt loader.

    const Resource r2("stdafx.xml");
    handle = this->resourceMgr->GetHandle(r2);
    CPPUNIT_ASSERT(handle);

    const Resource r3("unexisting.file");
    handle = this->resourceMgr->GetHandle(r3);
    CPPUNIT_ASSERT(!handle);

    const Resource r4("1.png");
    handle = this->resourceMgr->GetHandle(r4);
    CPPUNIT_ASSERT(handle);
    std::shared_ptr<GLESTextureResourceExtraData> textureExtra = std::static_pointer_cast<GLESTextureResourceExtraData>(handle->GetExtra());
    CPPUNIT_ASSERT(textureExtra->GetTexture() > 0);

    const Resource r5("TestFolder/TestFile.xml");
    handle = this->resourceMgr->GetHandle(r5);
    CPPUNIT_ASSERT(handle);
    std::shared_ptr<XMLResourceExtraData> xmlExtra = std::static_pointer_cast<XMLResourceExtraData>(handle->GetExtra());
    tinyxml2::XMLElement *root = xmlExtra->GetRoot();
    CPPUNIT_ASSERT(root->Value() == std::string("TestElement"));
    CPPUNIT_ASSERT(root->Attribute("testattribute") == std::string("testvalue"));

    const Resource r6("shader.vsh");
    handle = this->resourceMgr->GetHandle(r6);
    CPPUNIT_ASSERT(handle);
    std::shared_ptr<ShaderResourceExtraData> vertexExtra = std::static_pointer_cast<ShaderResourceExtraData>(handle->GetExtra());
    CPPUNIT_ASSERT(vertexExtra->GetGlShader() > 0);

    const Resource r7("shader.fsh");
    handle = this->resourceMgr->GetHandle(r7);
    CPPUNIT_ASSERT(handle);
    std::shared_ptr<ShaderResourceExtraData> fragmentExtra = std::static_pointer_cast<ShaderResourceExtraData>(handle->GetExtra());
    CPPUNIT_ASSERT(fragmentExtra->GetGlShader() > 0);

    const Resource r8("Fonts/Arial.ttf");
    handle = this->resourceMgr->GetHandle(r8);
    CPPUNIT_ASSERT(handle);
    std::shared_ptr<FontResourceExtraData> fontExtra = std::static_pointer_cast<FontResourceExtraData>(handle->GetExtra());
    CPPUNIT_ASSERT(fontExtra->GetFtFace() != NULL);
}