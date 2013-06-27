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

    virtual bool    Init(tinyxml2::XMLElement *data) override;

    GLuint          GetGlVertexArray() const { return this->glVertexArray; }

protected:
    virtual std::shared_ptr<SceneNode> CreateSceneNode() override;

private:
    std::shared_ptr<Resource>   textureResource;
    std::shared_ptr<Shader>     shader;

    GLuint          glVertexArray;
    GLuint          glbuffer;
    unsigned int    vertexCount;
};

#endif // __RENDERCOMPONENT_H__