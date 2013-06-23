#ifndef __RENDERCOMPONENT_H__
#define __RENDERCOMPONENT_H__

#include <memory>
#include <tinyxml2.h>
#include "ActorComponent.h"

class SceneNode;

//-----------------------------------------------------------------------------------------------------------
//  class RenderComponent
//-----------------------------------------------------------------------------------------------------------

class RenderComponent : public ActorComponent
{
public:
    virtual bool Init(tinyxml2::XMLElement *data) override;
    virtual void PostInit() override;

protected:
    virtual std::shared_ptr<SceneNode> GetSceneNode() = 0;
};

#endif // __RENDERCOMPONENT_H__