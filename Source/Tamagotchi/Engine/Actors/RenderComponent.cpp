#include "RenderComponent.h"
#include "EventManager/EventManager.h"

//-----------------------------------------------------------------------------------------------------------
//  class RenderComponent
//-----------------------------------------------------------------------------------------------------------

bool RenderComponent::Init(tinyxml2::XMLElement *data)
{
    return false;
}

void RenderComponent::PostInit()
{
    std::shared_ptr<SceneNode> sceneNode(CreateSceneNode());
    std::shared_ptr<Event_NewRenderComponent> event = std::shared_ptr<Event_NewRenderComponent>(TG_NEW Event_NewRenderComponent(this->GetOwner()->GetId(), sceneNode));
    EventManager::Get().TriggerEvent(event);
}