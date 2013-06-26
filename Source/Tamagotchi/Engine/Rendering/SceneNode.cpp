#include "SceneNode.h"
#include "Logger.h"

//-----------------------------------------------------------------------------------------------------------
//  class SceneNode
//-----------------------------------------------------------------------------------------------------------

//-----------------------------------------------
// Public
//-----------------------------------------------

SceneNode::SceneNode(ActorId actor)
    : actorId(actor)
{

}

bool SceneNode::AddChild(std::shared_ptr<SceneNode> kid)
{
    kid->SetParent(std::shared_ptr<SceneNode>(this));
    this->childNodes.push_back(kid);
    return true;
}

bool SceneNode::RemoveChild(ActorId id)
{
    for (SceneNodeList::iterator it = this->childNodes.begin(); it != this->childNodes.end(); ++it)
    {
        std::shared_ptr<SceneNode> node = *it;
        
        if (node->GetActorId() != INVALID_ACTOR_ID && id == node->GetActorId())
        {
            it = this->childNodes.erase(it);
            return true;
        }
    }

    return false;
}

bool SceneNode::IsVisible() const
{
    LogError("Implement me!");
    return false;
}

void SceneNode::OnUpdate(float delta)
{
    for (SceneNodeList::iterator it = this->childNodes.begin(); it != this->childNodes.end(); ++it)
    {
        std::shared_ptr<SceneNode> node = *it;
        node->OnUpdate(delta);
    }
}

void SceneNode::OnPreRender()
{
    LogError("Implement me!");
}

void SceneNode::OnRender()
{
    LogError("Implement me!");
}

void SceneNode::OnRenderChildren()
{
    LogError("Implement me!");
}

void SceneNode::OnPostRender()
{
    LogError("Implement me!");
}