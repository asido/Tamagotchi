#include "Shader.h"
#include "Logger.h"

//-----------------------------------------------------------------------------------------------------------
//  class Shader
//-----------------------------------------------------------------------------------------------------------

//-----------------------------------------------
// Public
//-----------------------------------------------

Shader::Shader()
    : program(0)
{

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

    return true;
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
        glDetachShader(this->program, fragmentShader);
        glDeleteProgram(this->program);
        this->program = 0;

        LogError("Failure linking shader program.");
        return false;
    }

    if (!ValidateProgram())
    {
        glDetachShader(this->program, vertexShader);
        glDetachShader(this->program, fragmentShader);
        glDeleteProgram(this->program);
        this->program = 0;

        LogError("Failure validating shader program.");
        return false;
    }

    return true;
}

bool DefaultShader::PrepareToRender()
{
    LogError("Implement me!");
    return false;
}