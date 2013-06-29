#ifndef __SCENENODE_H__
#define __SCENENODE_H__

#include <vector>
#include <memory>

#include <Eigen/Dense>
using namespace Eigen;

#include "Actors/Actor.h"
#include "Rendering/GLES.h"

//-----------------------------------------------------------------------------------------------------------
//  class SceneNode
//-----------------------------------------------------------------------------------------------------------

class Scene;
class SceneNode;
class RenderComponent;

typedef std::vector< std::shared_ptr<SceneNode> > SceneNodeList;

class SceneNode
{
public:
#if !defined(DEBUG) && !defined(_DEBUG)
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW
#endif

    SceneNode(ActorId actor, std::weak_ptr<RenderComponent> renderComp);
    virtual ~SceneNode();

    bool            AddChild(std::shared_ptr<SceneNode> kid);
    bool            RemoveChild(ActorId id);

    bool            IsVisible() const;

    virtual bool    Init() = 0;
    virtual void    OnUpdate(const Scene &scene, float delta);
    virtual void    OnPreRender(const Scene &scene);
    virtual void    OnRender(const Scene &scene) = 0;
    virtual void    OnRenderChildren(const Scene &scene);
    virtual void    OnPostRender(const Scene &scene);

    ActorId         GetActorId() const { return this->actorId; }
    void            SetParent(std::weak_ptr<SceneNode> parent) { this->parent = parent; }
    void            SetTransform(const Matrix4f &transform) { this->toWorld = transform; }

protected:
    std::shared_ptr<SceneNode>       GetParent() const;
    std::shared_ptr<RenderComponent> GetRenderComponent() const;

private:
    SceneNodeList                   childNodes;
    ActorId                         actorId;
    std::weak_ptr<SceneNode>        parent;
    std::weak_ptr<RenderComponent>  renderComponent;
    Matrix4f                        toWorld;
};


//-----------------------------------------------------------------------------------------------------------
//  class CameraSceneNode
//-----------------------------------------------------------------------------------------------------------

class CameraSceneNode : public SceneNode
{
private:
    std::shared_ptr<SceneNode> target;
};


//-----------------------------------------------------------------------------------------------------------
//  class SpriteSceneNode
//-----------------------------------------------------------------------------------------------------------

class Shader;
class Resource;

class SpriteSceneNode : public SceneNode
{
public:
    SpriteSceneNode(ActorId actorId, std::weak_ptr<RenderComponent> renderComp);

    virtual bool    Init() override;
    virtual void    OnRender(const Scene &scene) override;

private:
    std::shared_ptr<Shader>         shader;
    std::shared_ptr<Resource>       textureResource;

    GLuint          glVertexArray;
    GLuint          glBuffer;
    unsigned int    vertexCount;
};

#endif // __SCENENODE_H__