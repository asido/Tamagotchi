#include "Shader.h"
#include "Logger.h"

//-----------------------------------------------------------------------------------------------------------
//  class Shader
//-----------------------------------------------------------------------------------------------------------

//-----------------------------------------------
// Public
//-----------------------------------------------

Shader::Shader()
    : program(0), vertexShader(0), fragmentShader(0)
{

}

Shader::~Shader()
{
    if (this->vertexShader)
    {
        if (this->program)
        {
            glDetachShader(this->program, this->vertexShader);
        }
        glDeleteShader(this->vertexShader);
    }
    if (this->fragmentShader)
    {
        if (this->program)
        {
            glDetachShader(this->program, this->fragmentShader);
        }
        glDeleteShader(this->fragmentShader);
    }
    if (this->program)
    {
        glDeleteProgram(this->program);
    }

    GL_CHECK_ERROR();
}

//-----------------------------------------------
// Protected
//-----------------------------------------------

bool Shader::LinkProgram()
{
    if (this->program == 0)
    {
        LogError("Trying to link uninitialized program - skipping");
        return false;
    }

    glLinkProgram(this->program);

    GLint linked;
    glGetProgramiv(this->program, GL_LINK_STATUS, &linked);
    if (!linked)
    {
        GLint infoLength;
        glGetProgramiv(this->program, GL_INFO_LOG_LENGTH, &infoLength);

        if (infoLength > 0)
        {
            char *infoBuf = new char[infoLength];
            glGetProgramInfoLog(this->program, infoLength, NULL, infoBuf);
            LogError("Shader link info: %s", infoBuf);
            delete[] infoBuf;
        }

        return false;
    }

    GL_CHECK_ERROR();

    return true;
}

bool Shader::ValidateProgram()
{
    if (this->program == 0)
    {
        LogError("Trying to validate uninitialized program - skipping.");
        return false;
    }

    glValidateProgram(this->program);

    GLint valid;
    glGetProgramiv(this->program, GL_VALIDATE_STATUS, &valid);
    if (!valid)
    {
        GLint logLength;
        glGetProgramiv(this->program, GL_INFO_LOG_LENGTH, &logLength);

        if (logLength > 0)
        {
            char *infoBuf = new char[logLength];
            glGetProgramInfoLog(this->program, logLength, NULL, infoBuf);
            LogError("Shader validation info: %s", infoBuf);
            delete[] infoBuf;
        }

        return false;
    }

    GL_CHECK_ERROR();

    return true;
}

std::shared_ptr<RenderComponent> Shader::GetRenderComponent() const
{
    std::shared_ptr<RenderComponent> r = this->renderComponent.lock();
    if (!r)
    {
        LogWarning("Shader::renderComponent == NULL.");
    }
    return r;
}


//-----------------------------------------------------------------------------------------------------------
//  class DefaultShader
//-----------------------------------------------------------------------------------------------------------

bool DefaultShader::Init(GLuint vertexShader, GLuint fragmentShader)
{
    if (vertexShader == 0)
    {
        LogError("Invalid vertex shader.");
        return false;
    }
    if (fragmentShader == 0)
    {
        LogError("Invalid fragment shader.");
        return false;
    }

    this->program = glCreateProgram();
    if (!this->program)
    {
        LogError("glCreateProgram() has failed.");
        return false;
    }

    glAttachShader(this->program, vertexShader);
    glAttachShader(this->program, fragmentShader);

    if (!LinkProgram())
    {
        glDetachShader(this->program, vertexShader);
        glDeleteShader(vertexShader);
        glDetachShader(this->program, fragmentShader);
        glDeleteShader(fragmentShader);
        glDeleteProgram(this->program);
        this->program = 0;

        LogError("Failure linking shader program.");
        return false;
    }

    if (!ValidateProgram())
    {
        glDetachShader(this->program, vertexShader);
        glDeleteShader(vertexShader);
        glDetachShader(this->program, fragmentShader);
        glDeleteShader(fragmentShader);
        glDeleteProgram(this->program);
        this->program = 0;

        LogError("Failure validating shader program.");
        return false;
    }

    GL_CHECK_ERROR();

    return true;
}

bool DefaultShader::PrepareToRender()
{
    std::shared_ptr<RenderComponent> renderComponent = GetRenderComponent();
    std::shared_ptr<ResourceHandle> textureHandle = ResourceManager::GetHandle()
    return false;
}