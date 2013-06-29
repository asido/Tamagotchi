#include <memory>
#include "ShaderResource.h"
#include "defines.h"
#include "Logger.h"
#include "ResourceManager/ResourceManager.h"

//-----------------------------------------------------------------------------------------------------------
//  class VertexShaderResourceLoader
//-----------------------------------------------------------------------------------------------------------

const std::string& VertexShaderResourceLoader::GetPattern() const
{
    static const std::string pattern("*.vsh");
    return pattern;
}


//-----------------------------------------------------------------------------------------------------------
//  class FragmentShaderResourceLoader
//-----------------------------------------------------------------------------------------------------------

const std::string& FragmentShaderResourceLoader::GetPattern() const
{
    static const std::string pattern("*.fsh");
    return pattern;
}


//-----------------------------------------------------------------------------------------------------------
//  class ShaderResourceLoader
//-----------------------------------------------------------------------------------------------------------

unsigned int ShaderResourceLoader::GetLoadedResourceSize(char *rawBuffer, unsigned int rawSize)
{
    // Keep the resource cache from allocating memory for the shader, so OpenGL can manage it on it's own.
    return 0;
}

bool ShaderResourceLoader::LoadResource(char *rawBuffer, unsigned int rawSize, std::shared_ptr<ResourceHandle> handle)
{
    GLuint shader = CompileShader(rawBuffer);

    if (shader)
    {
        std::shared_ptr<ShaderResourceExtraData> extra = std::shared_ptr<ShaderResourceExtraData>(TG_NEW ShaderResourceExtraData);
        extra->SetGlShader(shader);
        handle->SetExtra(extra);
        return true;
    }
    else
    {
        return false;
    }
}

//-----------------------------------------------
// Private
//-----------------------------------------------

GLuint ShaderResourceLoader::CompileShader(const char *source) const
{
    GLuint s = glCreateShader(this->GetShaderType());
    if (!s)
    {
        GLenum err = glGetError();
        LogError("glCreateShader() has failed.");
        return s;
    }

    glShaderSource(s, 1, &source, NULL);
    GLint e = glGetError();
    glCompileShader(s);

    GLint compiled;
    glGetShaderiv(s, GL_COMPILE_STATUS, &compiled);
    if (!compiled)
    {
        GLint logLength;
        glGetShaderiv(s, GL_INFO_LOG_LENGTH, &logLength);
        if (logLength > 0)
        {
            char *logBuffer = new char[logLength];
            glGetShaderInfoLog(s, logLength, NULL, logBuffer);
            LogError("Shader compile error: %s", logBuffer);
            delete[] logBuffer;
        }

        glDeleteShader(s);
        return s;
    }

    return s;
}