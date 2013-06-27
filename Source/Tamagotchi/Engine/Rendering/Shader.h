#ifndef __SHADER_H__
#define __SHADER_H__

#include <memory>
#include <string>
#include "Rendering/GLES.h"

typedef unsigned int ShaderId;

//-----------------------------------------------------------------------------------------------------------
//  class Shader
//-----------------------------------------------------------------------------------------------------------

class Shader
{
public:
    Shader();
    ~Shader();

    virtual bool    Init(GLuint vertexShader, GLuint fragmentShader) = 0;
    virtual bool    PrepareToRender() = 0;

protected:
    bool            LinkProgram();
    bool            ValidateProgram();

    std::shared_ptr<RenderComponent> GetRenderComponent() const;

protected:
    GLuint program;
    GLuint vertexShader, GLuint fragmentShader;

private:
    std::weak_ptr<RenderComponent>  renderComponent;
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