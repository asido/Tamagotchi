#ifndef __EVENT_H__
#define __EVENT_H__

#include <string>
#include "Actors/Actor.h"

typedef unsigned long EventType;

//-----------------------------------------------------------------------------------------------------------
//  class IEvent
//-----------------------------------------------------------------------------------------------------------

class IEvent
{
public:
    virtual EventType           GetEventType() const = 0;
    virtual const std::string&  GetName() const = 0;
};


//-----------------------------------------------------------------------------------------------------------
//  Events
//-----------------------------------------------------------------------------------------------------------

class Event_Test : public IEvent
{
public:
    static const EventType Type;

    Event_Test(int value);

    virtual EventType           GetEventType() const override;
    virtual const std::string&  GetName() const override;

    int                         GetTestValue() const { return testValue; }

private:
    int testValue;
};

//-----------------------------------------------
// Event_NewRenderComponent
//-----------------------------------------------

class SceneNode;

class Event_NewRenderComponent : public IEvent
{
public:
    static const EventType Type;

    Event_NewRenderComponent(ActorId id, std::shared_ptr<SceneNode> node);

    virtual EventType           GetEventType() const override;
    virtual const std::string&  GetName() const override;

    ActorId                     GetActorId() const { return this->actorId; }
    std::shared_ptr<SceneNode>  GetSceneNode() const { return this->sceneNode; }

private:
    ActorId                     actorId;
    std::shared_ptr<SceneNode>  sceneNode;
};

#endif // __EVENT_H__