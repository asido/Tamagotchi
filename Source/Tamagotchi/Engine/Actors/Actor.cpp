#include "Actor.h"
#include "Logger.h"

Actor::Actor(ActorId id)
    : id(id), components()
{
    LogSpam("Actor created: %d", this->id);
}

Actor::~Actor()
{
    LogSpam("Actor destroyed: %d", this->id);
    this->components.clear();
}

bool Actor::Init()
{
    LogSpam("Actor initialising: %d", this->id);
    return true;
}

void Actor::PostInit()
{
    LogSpam("Actor post init: %d", this->id);

    for (ActorComponents::iterator it = this->components.begin(); it != this->components.end(); ++it)
    {
        it->second->PostInit();
    }
}

void Actor::OnUpdate(float delta)
{
    for (ActorComponents::iterator it = this->components.begin(); it != this->components.end(); ++it)
    {
        std::shared_ptr<ActorComponent> component = it->second;
        component->Update(delta);
    }
}

bool Actor::AddComponent(std::shared_ptr<ActorComponent> component)
{
    std::pair<ActorComponents::iterator, bool> success = this->components.insert(std::make_pair(component->GetId(), component));
    return success.second;
}