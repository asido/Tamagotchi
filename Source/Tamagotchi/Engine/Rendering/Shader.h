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
    virtual bool    Init() = 0;
    virtual bool    PrepareToRender() = 0;

protected:
    GLuint          GetProgram() const { return this->program; }

protected:
    bool            LinkProgram(GLuint program);
    bool            ValidateProgram(GLuint program);

private:
    GLuint program;
};


//-----------------------------------------------------------------------------------------------------------
//  class IShader
//-----------------------------------------------------------------------------------------------------------

class DefaultShader : public Shader
{
public:
    virtual bool Init() override;
    virtual bool PrepareToRender() override;
};

#endif // __SHADER_H__