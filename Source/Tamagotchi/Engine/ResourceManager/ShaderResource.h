#ifndef __SHADERRESOURCE_H__
#define __SHADERRESOURCE_H__

#include <memory>
#include <string>
#include "ResourceManager.h"
#include "Rendering/GLES.h"

//-----------------------------------------------------------------------------------------------------------
//  class ShaderResourceExtraData
//-----------------------------------------------------------------------------------------------------------

class ShaderResourceLoader;

class ShaderResourceExtraData : public IResourceExtraData
{
    friend class ShaderResourceLoader;

public:
    GLuint  GetGlShader() const { return this->glShader; }
    
private:
    void    SetGlShader(GLuint shader) { this->glShader = shader; }

private:
    GLuint glShader;
};


//-----------------------------------------------------------------------------------------------------------
//  class ShaderResourceLoader
//  NOTE: This is abstract class and should not be used directly.
//-----------------------------------------------------------------------------------------------------------

class ResourceHandle;

class ShaderResourceLoader : public IResourceLoader
{
public:
    virtual const std::string&  GetPattern() const = 0;
    virtual bool                UseRawFile() const override { return false; }
    virtual bool                DiscardRawBufferAfterLoad() const override { return true; }
    virtual bool                AddNullZero() const override { return true; }
    virtual unsigned int        GetLoadedResourceSize(char *rawBuffer, unsigned int rawSize) override;
    virtual bool                LoadResource(char *rawBuffer, unsigned int rawSize, std::shared_ptr<ResourceHandle> handle) override;

protected:
    virtual GLenum              GetShaderType() const = 0;

private:
    GLuint                      CompileShader(const char *source) const;
};

//-----------------------------------------------------------------------------------------------------------
//  class VertexShaderResourceLoader
//-----------------------------------------------------------------------------------------------------------

class VertexShaderResourceLoader : public ShaderResourceLoader
{
public:
    virtual const std::string&  GetPattern() const override;

protected:
    virtual GLenum              GetShaderType() const override { return GL_VERTEX_SHADER; }
};

//-----------------------------------------------------------------------------------------------------------
//  class FragmentShaderResourceLoader
//-----------------------------------------------------------------------------------------------------------

class FragmentShaderResourceLoader : public ShaderResourceLoader
{
public:
    virtual const std::string&  GetPattern() const override;

protected:
    virtual GLenum              GetShaderType() const override { return GL_FRAGMENT_SHADER; }
};

#endif // __SHADERRESOURCE_H__