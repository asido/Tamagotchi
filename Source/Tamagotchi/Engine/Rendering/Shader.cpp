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
//  class DefaultShader
//-----------------------------------------------------------------------------------------------------------

const std::string DefaultShader::name("DefaultShader");

ShaderId DefaultShader::GetIdStatic()
{
    static ShaderId id = INVALID_SHADER_ID;

    if (id == INVALID_SHADER_ID)
    {
        id = StringUtilities::Hash(DefaultShader::name);
    }

    return id;
}

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

    this->uniforms[DEFAULT_SHADER_UNIFORM_TEXTURE] = glGetUniformLocation(this->program, "u_sampler");
    if (this->uniforms[DEFAULT_SHADER_UNIFORM_TEXTURE] == -1)
    {
        LogWarning("glGetUniformLocation() for 'u_sampler' failed.");
    }

    GL_CHECK_ERROR();

    return true;
}

bool DefaultShader::PrepareToRender()
{
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, this->glTexture);

    glUseProgram(this->program);
    glUniform1i(this->uniforms[DEFAULT_SHADER_UNIFORM_TEXTURE], 0);

    GL_CHECK_ERROR();

    return true;
}