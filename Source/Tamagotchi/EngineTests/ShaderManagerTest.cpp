#include <cppunit/config/SourcePrefix.h>
#include "ShaderManagerTest.h"


// We need to access shader and program GL handles.
#define private public
#define protected public
#include "Rendering/ShaderManager.h"
#undef protected
#undef private

CPPUNIT_TEST_SUITE_REGISTRATION(ShaderManagerTest);

void ShaderManagerTest::setUp()
{
    this->shaderMgr = new ShaderManager();
}

void ShaderManagerTest::tearDown()
{
    delete this->shaderMgr;
}

void ShaderManagerTest::GetShaderTest()
{
    std::shared_ptr<Shader> shader0(this->shaderMgr->GetShader("UnexistingShader"));
    CPPUNIT_ASSERT(shader0 == NULL);

    std::shared_ptr<Shader> shader1(this->shaderMgr->GetShader("SpriteShader"));
    CPPUNIT_ASSERT(shader1 != NULL);
    CPPUNIT_ASSERT(shader1->vertexShader > 0);
    CPPUNIT_ASSERT(shader1->fragmentShader > 0);
    CPPUNIT_ASSERT(shader1->program > 0);
}