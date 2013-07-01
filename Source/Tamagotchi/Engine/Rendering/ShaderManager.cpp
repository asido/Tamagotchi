#include "ShaderManager.h"
#include "Shader.h"
#include "TamagotchiEngine.h"
#include "EngineConfig.h"
#include "ResourceManager/ResourceManager.h"
#include "ResourceManager/ShaderResource.h"

//-----------------------------------------------------------------------------------------------------------
//  class ShaderManager
//-----------------------------------------------------------------------------------------------------------

//-----------------------------------------------
// Public
//-----------------------------------------------

ShaderManager::ShaderManager()
{
    this->shaderFactory.Register<DefaultShader>(DefaultShader::GetIdStatic());
}

std::shared_ptr<Shader> ShaderManager::GetShader(const std::string &shaderName)
{
    std::shared_ptr<Shader> shader;
    ShaderMap::iterator it = this->shaders.find(shaderName);

    if (it == this->shaders.end())
    {
        shader = LoadShader(shaderName);
        
        if (!shader)
        {
            LogWarning("Shader was not loaded: %s", shaderName.c_str());
            return std::shared_ptr<Shader>();
        }
    }
    else
    {
        shader = it->second;
    }

    return shader;
}

//-----------------------------------------------
// Private
//-----------------------------------------------

std::shared_ptr<Shader> ShaderManager::LoadShader(const std::string &shaderName)
{
    const std::string shadersPath = g_engine->GetEngineConfig()->GetAssetPathShaders();
    std::shared_ptr<ResourceHandle> vShaderHandle = g_engine->GetResourceManager()->GetHandle(shadersPath + shaderName + ".vsh");
    std::shared_ptr<ResourceHandle> fShaderHandle = g_engine->GetResourceManager()->GetHandle(shadersPath + shaderName + ".fsh");

    if (!vShaderHandle || !fShaderHandle)
    {
        LogWarning("Couldn't load resource handles of shader: %s", shaderName.c_str());
        return std::shared_ptr<Shader>();
    }

    std::shared_ptr<ShaderResourceExtraData> vertexExtra = std::static_pointer_cast<ShaderResourceExtraData>(vShaderHandle->GetExtra());
    std::shared_ptr<ShaderResourceExtraData> fragmentExtra = std::static_pointer_cast<ShaderResourceExtraData>(fShaderHandle->GetExtra());

    std::shared_ptr<Shader> shader(this->shaderFactory.Create(Shader::GetIdFromName(shaderName)));

    if (!shader->Init(vertexExtra->GetGlShader(), fragmentExtra->GetGlShader()))
    {
        LogError("Shader '%s' initialization has failed.", shaderName.c_str());
        return std::shared_ptr<Shader>();
    }

    this->shaders[shaderName] = shader;

    return shader;
}