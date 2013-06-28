#include "ActorFactory.h"
#include "TamagotchiEngine.h"
#include "Logger.h"
#include "ResourceManager/ResourceManager.h"
#include "ResourceManager/XMLResource.h"
#include "TransformComponent.h"
#include "RenderComponent.h"

//-----------------------------------------------------------------------------------------------------------
//  class ActorFactory
//-----------------------------------------------------------------------------------------------------------

ActorFactory::ActorFactory()
    : lastActorId(INVALID_ACTOR_ID)
{
    this->componentFactory.Register<TransformComponent>(TransformComponent::GetIdStatic());
    this->componentFactory.Register<SpriteRenderComponent>(SpriteRenderComponent::GetIdStatic());
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
        LogError("Creating actor from resource has failed: %s", actorResource.c_str());
        return std::shared_ptr<Actor>();
    }

    std::shared_ptr<Actor> actor(TG_NEW Actor(this->GetNextActorId()));
    if (!actor->Init())
    {
        LogError("Failed to initialize actor: %s", actorResource.c_str());
        return std::shared_ptr<Actor>();
    }

    // Load the components.
    for (tinyxml2::XMLElement *node = root->FirstChildElement(); node; node = node->NextSiblingElement())
    {
        std::shared_ptr<ActorComponent> component(CreateComponent(node));

        if (component)
        {
            if (actor->AddComponent(component))
            {
                component->SetOwner(actor);
            }
            else
            {
                LogError("Failed to add actor component %s to actor ID %d", node->Value(), actor->GetId());
            }
        }
        else
        {
            LogError("Actor creation has failed because a component could not be created: %s.", node->Value());
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
        LogAssert(transformComponent);
        transformComponent->SetTransform(*initialTransform);
    }

    actor->PostInit();

    return actor;
}

void ActorFactory::ModifyActor(std::shared_ptr<Actor> actor, tinyxml2::XMLElement *overrides)
{
    for (tinyxml2::XMLElement *node = overrides->FirstChildElement(); node; node = node->NextSiblingElement())
    {
        ComponentId componentId = ActorComponent::GetIdFromName(node->Value());
        std::shared_ptr<ActorComponent> component = actor->GetComponent<ActorComponent>(componentId);
        if (component)
        {
            component->Init(node);
        }
        else
        {
            component = CreateComponent(node);
            if (component)
            {
                if (actor->AddComponent(component))
                {
                    component->SetOwner(actor);
                }
                else
                {
                    LogError("Failed to add actor component %s to actor ID %d", node->Value(), actor->GetId());
                }
            }
        }
    }
}

//-----------------------------------------------
// Private
//-----------------------------------------------

ActorId ActorFactory::GetNextActorId()
{
    this->lastActorId++;
    return this->lastActorId;
}

std::shared_ptr<ActorComponent> ActorFactory::CreateComponent(tinyxml2::XMLElement *data)
{
    const std::string name(data->Value());
    std::shared_ptr<ActorComponent> component(this->componentFactory.Create(ActorComponent::GetIdFromName(name)));
    
    if (component)
    {
        if (!component->Init(data))
        {
            LogError("Failed to initialize component named '%s'.", name.c_str());
            return std::shared_ptr<ActorComponent>();
        }
    }
    else
    {
        LogError("Failed to create component named '%s'.", name.c_str());
        return std::shared_ptr<ActorComponent>();
    }

    return component;
}