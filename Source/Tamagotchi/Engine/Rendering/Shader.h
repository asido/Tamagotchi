#ifndef __SHADER_H__
#define __SHADER_H__

#include <memory>
#include <string>
#include "Rendering/GLES.h"
#include "StringUtilities.h"

typedef unsigned int ShaderId;
const ShaderId INVALID_SHADER_ID = 0;

//-----------------------------------------------------------------------------------------------------------
//  class Shader
//-----------------------------------------------------------------------------------------------------------

class RenderComponent;

class Shader
{
public:
    static ShaderId GetIdFromName(const std::string &name) { return StringUtilities::Hash(name); }

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
    GLuint vertexShader, fragmentShader;

private:
    std::weak_ptr<RenderComponent>  renderComponent;
};


//-----------------------------------------------------------------------------------------------------------
//  class DefaultShader
//-----------------------------------------------------------------------------------------------------------

class DefaultShader : public Shader
{
public:
    static ShaderId GetIdStatic();

    virtual bool Init(GLuint vertexShader, GLuint fragmentShader) override;
    virtual bool PrepareToRender() override;

private:
    static const std::string name;
};

#endif // __SHADER_H__