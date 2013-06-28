#include "Scene.h"
#include "SceneNode.h"
#include "EventManager/EventManager.h"

//-----------------------------------------------------------------------------------------------------------
//  class Scene
//-----------------------------------------------------------------------------------------------------------

//-----------------------------------------------
// Public
//-----------------------------------------------

Scene::Scene()
{
    EventManager::Get().AddListener(fastdelegate::MakeDelegate(this, &Scene::NewRenderComponentDelegate), Event_NewRenderComponent::Type);
}

bool Scene::AddChild(std::shared_ptr<SceneNode> node)
{
    ActorId actorId = node->GetActorId();

    if (actorId != INVALID_ACTOR_ID)
    {
        this->actorMap[actorId] = node;
    }

    return this->rootNode->AddChild(node);
}

bool Scene::RemoveChild(ActorId actorId)
{
    if (actorId == INVALID_ACTOR_ID)
    {
        LogWarning("actorId == INVALID_ACTOR_ID");
        return false;
    }

    this->actorMap.erase(actorId);
    return this->rootNode->RemoveChild(actorId);
}

std::shared_ptr<SceneNode> Scene::FindChild(ActorId actorId)
{
    SceneActorMap::iterator it = this->actorMap.find(actorId);
    if (it == this->actorMap.end())
    {
        return std::shared_ptr<SceneNode>();
    }
    return it->second;
}

void Scene::PushAndSetMatrix(const Matrix4f &matrix)
{
    LogError("Implement me!");
}

void Scene::OnUpdate(float delta)
{
    if (!this->rootNode)
    {
        LogWarning("rootNode == NULL");
        return;
    }

    return this->rootNode->OnUpdate(*this, delta);
}

void Scene::OnRender()
{
    if (!this->rootNode)
    {
        LogWarning("rootNode == NULL");
        return;
    }

    this->rootNode->OnPreRender(*this);
    this->rootNode->OnRender(*this);
    this->rootNode->OnRenderChildren(*this);
    this->rootNode->OnPostRender(*this);
}

//-----------------------------------------------
// Event delegates
//-----------------------------------------------

void Scene::NewRenderComponentDelegate(std::shared_ptr<IEvent> e)
{
    std::shared_ptr<Event_NewRenderComponent> event = std::static_pointer_cast<Event_NewRenderComponent>(e);
    std::shared_ptr<SceneNode> sceneNode = event->GetSceneNode();
    AddChild(sceneNode);
}