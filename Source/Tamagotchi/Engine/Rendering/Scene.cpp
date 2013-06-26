#include "Scene.h"
#include "SceneNode.h"

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

    return this->rootNode->OnUpdate(delta);
}

void Scene::OnRender()
{
    if (!this->rootNode)
    {
        LogWarning("rootNode == NULL");
        return;
    }

    this->rootNode->OnPreRender();
    this->rootNode->OnRender();
    this->rootNode->OnRenderChildren();
    this->rootNode->OnPostRender();
}