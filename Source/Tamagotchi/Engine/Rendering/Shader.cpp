#include "Shader.h"
#include "Logger.h"
#include "TamagotchiEngine.h"

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
        LogSpam("Shader program destroyed: %d", this->program);
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


//-----------------------------------------------------------------------------------------------------------
//  class SpriteShader
//-----------------------------------------------------------------------------------------------------------

const std::string SpriteShader::name("SpriteShader");

//-----------------------------------------------
// Public
//-----------------------------------------------

ShaderId SpriteShader::GetIdStatic()
{
    static ShaderId id = INVALID_SHADER_ID;

    if (id == INVALID_SHADER_ID)
    {
        id = StringUtilities::Hash(SpriteShader::name);
    }

    return id;
}

bool SpriteShader::Init(GLuint vertexShader, GLuint fragmentShader)
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
    LogSpam("Shader program created: %d", this->program);

    glAttachShader(this->program, vertexShader);
    glAttachShader(this->program, fragmentShader);

    // Bind shader attribute locations.
    glBindAttribLocation(this->program, DEFAULT_VERTEX_ATTRIB_POSITION, "a_position");
    glBindAttribLocation(this->program, DEFAULT_VERTEX_ATTRIB_TEXTURE, "a_textureCoord");

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

    LoadUniformLocation(DEFAULT_SHADER_UNIFORM_MVP, "u_mvp");
    LoadUniformLocation(DEFAULT_SHADER_UNIFORM_TEXTURE, "u_sampler");

    GL_CHECK_ERROR();

    return true;
}

bool SpriteShader::PrepareToRender()
{
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, this->glTexture);

    glUseProgram(this->program);
    glUniformMatrix4fv(this->uniforms[DEFAULT_SHADER_UNIFORM_MVP], 1, GL_FALSE, this->mvpMatrix.data());
    glUniform1i(this->uniforms[DEFAULT_SHADER_UNIFORM_TEXTURE], 0);

    GL_CHECK_ERROR();

    return true;
}

//-----------------------------------------------
// Private
//-----------------------------------------------

bool SpriteShader::LoadUniformLocation(GLenum uniform, const std::string &uniformVar)
{
    LogAssert(uniform < DEFAULT_SHADER_UNIFORM_COUNT && "Invalid uniform.");
    this->uniforms[uniform] = glGetUniformLocation(this->program, uniformVar.c_str());

    if (this->uniforms[uniform] == -1)
    {
        LogWarning("glGetUniformLocation() for uniform '%s' has failed.", uniformVar.c_str());
        return false;
    }
    else
    {
        return true;
    }
}