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

class SceneNode;
typedef std::vector< std::shared_ptr<SceneNode> > SceneNodeList;

class SceneNode
{
public:
    SceneNode(ActorId actor);

    bool            AddChild(std::shared_ptr<SceneNode> kid);
    bool            RemoveChild(ActorId id);

    bool            IsVisible() const;

    virtual void    OnUpdate(float delta);
    virtual void    OnPreRender();
    virtual void    OnRender() = 0;
    virtual void    OnRenderChildren();
    virtual void    OnPostRender();

    ActorId         GetActorId() const { return this->actorId; }
    void            SetParent(std::weak_ptr<SceneNode> parent) { this->parent = parent; }
    void            SetTransform(const Matrix4f &transform) { this->toWorld = transform; }

private:
    std::shared_ptr<SceneNode>  GetParent() const;

private:
    SceneNodeList               childNodes;

    ActorId                     actorId;
    std::weak_ptr<SceneNode>    parent;
    Matrix4f                    toWorld;
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

class SpriteSceneNode : public SceneNode
{
public:
    SpriteSceneNode(ActorId actorId);

    virtual void    OnRender() override;
};

#endif // __SCENENODE_H__