#include "SceneNode.h"
#include "Logger.h"

//-----------------------------------------------------------------------------------------------------------
//  class SceneNode
//-----------------------------------------------------------------------------------------------------------

//-----------------------------------------------
// Public
//-----------------------------------------------

SceneNode::SceneNode(ActorId actor, std::weak_ptr<RenderComponent> renderComp)
    : actorId(actor), renderComponent(renderComp)
{

}

bool SceneNode::Init()
{
    // TODO: ask ShaderManager to load the shader.
    this->shader = std::shared_ptr<Shader>(TG_NEW DefaultShader);
    if (!this->shader->Init(vertexExtra->GetGlShader(), fragmentExtra->GetGlShader()))
    {
        LogError("Shader initialization has failed: %s.", shaderName.c_str());
        return false;
    }
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

void SceneNode::OnUpdate(const Scene &scene, float delta)
{
    for (SceneNodeList::iterator it = this->childNodes.begin(); it != this->childNodes.end(); ++it)
    {
        std::shared_ptr<SceneNode> node = *it;
        node->OnUpdate(delta);
    }
}

void SceneNode::OnPreRender(const Scene &scene)
{
    LogError("Implement me!");
}

void SceneNode::OnRenderChildren(const Scene &scene)
{
    LogError("Implement me!");
}

void SceneNode::OnPostRender(const Scene &scene)
{
    LogError("Implement me!");
}

std::shared_ptr<SceneNode> SceneNode::GetParent() const
{
    std::shared_ptr<SceneNode> p = this->parent.lock();
    if (!p)
    {
        LogWarning("SceneNode::parent == NULL.");
    }
    return p;
}

std::shared_ptr<RenderComponent> SceneNode::GetRenderComponent() const
{
    std::shared_ptr<RenderComponent> r = this->renderComponent.lock();
    if (!r)
    {
        LogWarning("SceneNode::renderComponent == NULL.");
    }
    return r;
}

//-----------------------------------------------------------------------------------------------------------
//  class SpriteSceneNode
//-----------------------------------------------------------------------------------------------------------

SpriteSceneNode::SpriteSceneNode(ActorId actorId, std::weak_ptr<RenderComponent> renderComp)
    : SceneNode(actorId, renderComp)
{

}

bool SpriteSceneNode::Init()
{
    if (!SceneNode::Init())
    {
        LogError("SceneNode::Init() has failed.");
        return false;
    }

    return true;
}

void SpriteSceneNode::OnRender(const Scene &scene)
{
    std::shared_ptr<SpriteRenderComponent> renderComponent = std::static_pointer_cast<SpriteRenderComponent>(this->GetRenderComponent());
    if (!renderComponent)
    {
        LogWarning("renderComponent == NULL");
        return;
    }

    glBindVertexArrayOES(renderComponent->GetGlVertexArray());

    if (!renderComponent->GetShader()->PrepareToRender())
    {
        LogError("Shader prepare to render has failed.");
        return false;
    }

    glDrawArrays(GL_TRIANGLE_STRIP, 0, renderComponent->GetVertexCount());
}