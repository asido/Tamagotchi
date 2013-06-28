#include "Event.h"

//-----------------------------------------------------------------------------------------------------------
//  Event_Test
//-----------------------------------------------------------------------------------------------------------

const EventType Event_Test::Type(0x9f85ee47);

Event_Test::Event_Test(int value)
    : testValue(value)
{

}

EventType Event_Test::GetEventType() const
{
    return this->Type;
}

const std::string& Event_Test::GetName() const
{
    static const std::string name("Event_Test");
    return name;
}


//-----------------------------------------------------------------------------------------------------------
//  Event_NewRenderComponent
//-----------------------------------------------------------------------------------------------------------

const EventType Event_NewRenderComponent::Type(0xe4403d5e);

Event_NewRenderComponent::Event_NewRenderComponent(std::shared_ptr<SceneNode> node)
    : sceneNode(node)
{

}

EventType Event_NewRenderComponent::GetEventType() const
{
    return this->Type;
}

const std::string& Event_NewRenderComponent::GetName() const
{
    static const std::string name("Event_NewRenderComponent");
    return name;
}