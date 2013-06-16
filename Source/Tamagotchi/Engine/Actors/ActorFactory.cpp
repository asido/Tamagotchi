#include "ActorFactory.h"
#include "TamagotchiEngine.h"
#include "Logger.h"
#include "ResourceManager/ResourceManager.h"
#include "ResourceManager/XMLResource.h"
#include "TransformComponent.h"

//-----------------------------------------------------------------------------------------------------------
//  class ActorFactory
//-----------------------------------------------------------------------------------------------------------

ActorFactory::ActorFactory()
    : lastActorId(INVALID_ACTOR_ID)
{
    // TODO: register all actor components. i.e:
    //this->componentFactory.Register<SomeComponent>(SomeComponent::ComponentId);
}

//-----------------------------------------------
// Public
//-----------------------------------------------

std::shared_ptr<Actor> ActorFactory::CreateActor(const std::string &actorResource, tinyxml2::XMLElement *overrides, const Matrix4f *initialTransform)
{
    Resource r(actorResource);
    std::shared_ptr<ResourceHandle> handle = g_engine->GetResourceManager()->GetHandle(r);
    std::shared_ptr<XMLResourceExtraData> xmlExtra = std::static_pointer_cast<XMLResourceExtraData>(handle->GetExtra());
    tinyxml2::XMLElement *root = xmlExtra->GetRoot();
    if (!root)
    {
        LogError("Creating actor from resource has failed: %s", actorResource);
        return std::shared_ptr<Actor>();
    }

    std::shared_ptr<Actor> actor(TG_NEW Actor(this->GetNextActorId()));
    if (!actor->Init())
    {
        LogError("Failed to initialize actor: %s", actorResource);
        return std::shared_ptr<Actor>();
    }

    // Load the components.
    for (tinyxml2::XMLElement *node = root->FirstChildElement(); node; node = node->NextSiblingElement())
    {
        std::shared_ptr<ActorComponent> component(CreateComponent(node));

        if (component)
        {
            actor->AddComponent(component);
            component->SetOwner(actor);
        }
        else
        {
            return std::shared_ptr<Actor>();
        }
    }

    if (overrides)
    {
        ModifyActor(actor, overrides);
    }

    // Set initial actor position.
    if (initialTransform)
    {
        std::shared_ptr<TransformComponent> transformComponent = actor->GetComponent<TransformComponent>(TransformComponent::GetIdStatic());
    }

    actor->PostInit();

    return actor;
}

void ActorFactory::ModifyActor(std::shared_ptr<Actor> actor, tinyxml2::XMLElement *overrides)
{

}

//-----------------------------------------------
// Private
//-----------------------------------------------

ActorId ActorFactory::GetNextActorId()
{
    this->lastActorId++;
    return this->lastActorId;
}

std::shared_ptr<ActorComponent>    CreateComponent(tinyxml2::XMLElement *data)
{
    return std::shared_ptr<ActorComponent>();
}