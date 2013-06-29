#ifndef __SCENE_H__
#define __SCENE_H__

#include <map>
#include <memory>

#include <Eigen/Dense>
using namespace Eigen;

#include "Actors/Actor.h"

class SceneNode;
class CameraSceneNode;
class IRenderer;
class IEvent;

//-----------------------------------------------------------------------------------------------------------
//  class Scene
//-----------------------------------------------------------------------------------------------------------

typedef std::map< ActorId, std::shared_ptr<SceneNode> > SceneActorMap;

class Scene
{
public:
    Scene();
    ~Scene();

    bool                        AddChild(std::shared_ptr<SceneNode> node);
    bool                        RemoveChild(ActorId actorId);
    std::shared_ptr<SceneNode>  FindChild(ActorId actorId);

    void                        PushAndSetMatrix(const Matrix4f &matrix);

    virtual void                OnUpdate(float delta);
    virtual void                OnRender();

    // Event delegates.
    void                        NewRenderComponentDelegate(std::shared_ptr<IEvent> e);

private:
    std::shared_ptr<SceneNode>          rootNode;
    std::shared_ptr<CameraSceneNode>    cameraNode;
    SceneActorMap                       actorMap;
};

#endif // __SCENE_H__