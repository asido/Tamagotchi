#ifndef __SHADER_H__
#define __SHADER_H__

#include <string>
#include "Rendering/GLES.h"

//-----------------------------------------------------------------------------------------------------------
//  class Shader
//-----------------------------------------------------------------------------------------------------------

class Shader
{
public:
    Shader();

    virtual bool    Init(GLuint vertexShader, GLuint fragmentShader) = 0;
    virtual bool    PrepareToRender() = 0;

protected:
    bool            LinkProgram();
    bool            ValidateProgram();

protected:
    GLuint program;
};


//-----------------------------------------------------------------------------------------------------------
//  class IShader
//-----------------------------------------------------------------------------------------------------------

class DefaultShader : public Shader
{
public:
    virtual bool Init(GLuint vertexShader, GLuint fragmentShader) override;
    virtual bool PrepareToRender() override;
};

#endif // __SHADER_H__