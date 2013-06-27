#ifndef __RENDERCOMPONENT_H__
#define __RENDERCOMPONENT_H__

#include <memory>
#include <tinyxml2.h>
#include "ActorComponent.h"
#include "Rendering/GLES.h"

class SceneNode;
class Resource;
class Shader;

//-----------------------------------------------------------------------------------------------------------
//  class RenderComponent
//-----------------------------------------------------------------------------------------------------------

class RenderComponent : public ActorComponent
{
public:
    virtual bool Init(tinyxml2::XMLElement *data) = 0;
    virtual void PostInit() override;

protected:
    virtual std::shared_ptr<SceneNode> CreateSceneNode() = 0;
};


//-----------------------------------------------------------------------------------------------------------
//  class SpriteRenderComponent
//-----------------------------------------------------------------------------------------------------------

class SpriteRenderComponent : public RenderComponent
{
public:
    SpriteRenderComponent();

    virtual bool        Init(tinyxml2::XMLElement *data) override;

    const std::string&  GetTextureFilename() const { return this->textureFilename; }
    const std::string&  GetShaderName() const { return this->shaderName; }

protected:
    virtual std::shared_ptr<SceneNode> CreateSceneNode() override;

private:
    std::string textureFilename;
    std::string shaderName;
};

#endif // __RENDERCOMPONENT_H__