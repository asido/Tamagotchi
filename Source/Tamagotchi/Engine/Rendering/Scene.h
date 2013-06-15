#ifndef __SCENE_H__
#define __SCENE_H__

#include <map>
#include <memory>
#include "Actors/Actor.h"

class SceneNode;
class CameraSceneNode;
class IRenderer;

//-----------------------------------------------------------------------------------------------------------
//  class Scene
//-----------------------------------------------------------------------------------------------------------

typedef std::map< ActorId, std::shared_ptr<SceneNode> > SceneActorMap;

class Scene
{
public:
    Scene(std::shared_ptr<IRenderer> renderer);

    virtual void OnUpdate(float delta);
    virtual void OnRender();

private:
    std::shared_ptr<SceneNode>          rootNode;
    std::shared_ptr<CameraSceneNode>    cameraNode;
    SceneActorMap                       actorMap;

    std::shared_ptr<IRenderer>          renderer;
};

#endif // __SCENE_H__