#ifndef __SHADERMANAGER_H__
#define __SHADERMANAGER_H__

#include <string>
#include <memory>
#include <map>
#include "ObjectFactory.h"
#include "Shader.h"

//-----------------------------------------------------------------------------------------------------------
//  class ShaderManager
//-----------------------------------------------------------------------------------------------------------

typedef std::map< std::string, std::shared_ptr<Shader> > ShaderMap;

class ShaderManager
{
public:
    std::shared_ptr<Shader> GetShader(const std::string &shaderName);

private:
    std::shared_ptr<Shader> LoadShader(const std::string &shaderName);

private:
    GenericObjectFactory<Shader, ShaderId> shaderFactory;
    ShaderMap shaders;
};

#endif // __SHADERMANAGER_H__