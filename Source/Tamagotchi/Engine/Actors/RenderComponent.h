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
    virtual bool                Init(tinyxml2::XMLElement *data) override;
    virtual void                PostInit() override;

    virtual void                Update(float delta) {}

    virtual ComponentId         GetId() const = 0;
    virtual const std::string&  GetName() const = 0;

    const std::string&          GetShaderName() const { return this->shaderName; }

protected:
    virtual std::shared_ptr<SceneNode> CreateSceneNode() = 0;

private:
    std::string shaderName;
};


//-----------------------------------------------------------------------------------------------------------
//  class SpriteRenderComponent
//-----------------------------------------------------------------------------------------------------------

class SpriteRenderComponent : public RenderComponent
{
public:
    static ComponentId          GetIdStatic();

    SpriteRenderComponent() {}
    ~SpriteRenderComponent() {}

    virtual bool                Init(tinyxml2::XMLElement *data) override;
    virtual ComponentId         GetId() const override { return SpriteRenderComponent::GetIdStatic(); }
    virtual const std::string&  GetName() const override { return SpriteRenderComponent::name; }

    const std::string&          GetTextureFilename() const { return this->textureFilename; }
    
protected:
    virtual std::shared_ptr<SceneNode> CreateSceneNode() override;

private:
    static const std::string name;

    std::string textureFilename;
};

#endif // __RENDERCOMPONENT_H__