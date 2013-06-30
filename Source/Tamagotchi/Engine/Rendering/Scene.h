#ifndef __SCENE_H__
#define __SCENE_H__

#include <map>
#include <memory>

#include <Eigen/Dense>
using namespace Eigen;

#include "Actors/Actor.h"

class SceneNode;
class IRenderer;
class IEvent;
class Camera;
class MatrixStack;

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

    void                        PushMatrix(const Matrix4f &matrix);
    void                        PopMatrix();
    Matrix4f                    GetTopMatrix() const;

    virtual void                OnUpdate(float delta);
    virtual void                OnRender();

    // Event delegates.
    void                        NewRenderComponentDelegate(std::shared_ptr<IEvent> e);

    std::shared_ptr<Camera>     GetCamera() const { return this->camera; }

private:
    std::shared_ptr<SceneNode>          rootNode;
    SceneActorMap                       actorMap;
    std::shared_ptr<Camera>             camera;
    std::shared_ptr<MatrixStack>        matrixStack;
};

#endif // __SCENE_H__