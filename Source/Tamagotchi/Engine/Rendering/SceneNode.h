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

class SceneNode : public std::enable_shared_from_this<SceneNode>
{
public:
#if !defined(DEBUG) && !defined(_DEBUG)
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW
#endif

    SceneNode();
    SceneNode(ActorId actor, std::weak_ptr<RenderComponent> renderComp);
    virtual ~SceneNode();

    bool            AddChild(std::shared_ptr<SceneNode> kid);
    bool            RemoveChild(ActorId id);

    bool            IsVisible() const;

    virtual bool    Init() = 0;
    virtual void    OnUpdate(const Scene &scene, float delta);
    virtual bool    OnPreRender(Scene &scene);
    virtual void    OnRender(Scene &scene) = 0;
    virtual void    OnRenderChildren(Scene &scene);
    virtual void    OnPostRender(Scene &scene);

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
//  class RootSceneNode
//-----------------------------------------------------------------------------------------------------------

class RootSceneNode : public SceneNode
{
public:
    RootSceneNode();
    ~RootSceneNode();

    virtual bool Init() override { return true; }
    virtual void OnRender(Scene &scene) override { }
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
    ~SpriteSceneNode();

    virtual bool    Init() override;
    virtual void    OnRender(Scene &scene) override;

private:
    static unsigned int             VertexCount;

    std::shared_ptr<Shader>         shader;
    std::shared_ptr<Resource>       textureResource;

    GLuint          glVertexArray;
    GLuint          glBuffer;
};

#endif // __SCENENODE_H__