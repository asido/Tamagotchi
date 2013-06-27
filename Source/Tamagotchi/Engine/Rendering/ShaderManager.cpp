#include "ShaderManager.h"
#include "Shader.h"
#include "TamagotchiEngine.h"

//-----------------------------------------------------------------------------------------------------------
//  class ShaderManager
//-----------------------------------------------------------------------------------------------------------

//-----------------------------------------------
// Public
//-----------------------------------------------

std::shared_ptr<Shader> ShaderManager::GetShader(const std::string &shaderName)
{
    std::shared_ptr<Shader> shader;
    ShaderMap::iterator it = this->shaders.find(shaderName);

    if (it == this->shaders.end())
    {
        shader = LoadShader(shaderName);
        
        if (!shader)
        {
            LogError("shader == NULL");
            return std::shared_ptr<Shader>();
        }
    }

    return shader;
}

//-----------------------------------------------
// Private
//-----------------------------------------------

std::shared_ptr<Shader> ShaderManager::LoadShader(const std::string &shaderName)
{
    std::shared_ptr<ResourceHandle> vShaderHandle = g_engine->GetResourceManager()->GetHandle(shaderName + ".vsh");
    std::shared_ptr<ResourceHandle> fShaderHandle = g_engine->GetResourceManager()->GetHandle(shaderName + ".fsh");

    if (!vShaderHandle || !fShaderHandle)
    {
        LogError("Couldn't load shader resource handles.");
        return std::shared_ptr<Shader>();
    }

    std::shared_ptr<ShaderResourceExtraData> vertexExtra = std::static_pointer_cast<ShaderResourceExtraData>(vShaderHandle->GetExtra());
    std::shared_ptr<ShaderResourceExtraData> fragmentExtra = std::static_pointer_cast<ShaderResourceExtraData>(fShaderHandle->GetExtra());

    std::shared_ptr<Shader> shader = std::shared_ptr<Shader>(TG_NEW Shader());

    if (!shader->Init(vertexExtra->GetGlShader(), fragmentExtra->GetGlShader()))
    {
        LogError("Shader '%s' initialization has failed.", shaderName.c_str());
        return std::shared_ptr<Shader>();
    }

    this->shaders[shaderName] = shader;

    return shader;
}