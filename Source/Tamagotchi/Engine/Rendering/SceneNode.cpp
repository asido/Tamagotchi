#include "SceneNode.h"
#include "Scene.h"
#include "Logger.h"
#include "TamagotchiEngine.h"
#include "ShaderManager.h"
#include "Actors/RenderComponent.h"
#include "ResourceManager/ResourceManager.h"

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
        node->OnUpdate(scene, delta);
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

enum {
    VERTEX_ATTRIB_POSITION=0,
    VERTEX_ATTRIB_TEXTURE
};

typedef struct {
    Vector3f positionCoords;
    Vector2f textureCoords;
} VertexData;

static const VertexData SpriteVerticies[] = {
    { Vector3f(0.0f, 0.0f, 0.0f), Vector2f(0.0f, 0.0f) },
    { Vector3f(1.0f, 0.0f, 0.0f), Vector2f(1.0f, 0.0f) },
    { Vector3f(0.0f, 1.0f, 0.0f), Vector2f(0.0f, 1.0f) },
    { Vector3f(1.0f, 1.0f, 0.0f), Vector2f(1.0f, 1.0f) }
};

SpriteSceneNode::SpriteSceneNode(ActorId actorId, std::weak_ptr<RenderComponent> renderComp)
    : SceneNode(actorId, renderComp), vertexCount(4)
{

}

bool SpriteSceneNode::Init()
{
    const std::string &shaderName = GetRenderComponent()->GetShaderName();
    this->shader = g_engine->GetShaderManager()->GetShader(shaderName);

    // Init VBO
    glGenVertexArraysOES(1, &this->glVertexArray);
    glBindVertexArrayOES(this->glVertexArray);

    glGenBuffers(1, &this->glBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, this->glBuffer);
    glBufferData(this->glBuffer, sizeof(SpriteVerticies), SpriteVerticies, GL_STATIC_DRAW);

    glVertexAttribPointer(VERTEX_ATTRIB_POSITION, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), static_cast<GLvoid*>(NULL + offsetof(VertexData, positionCoords)));
    glEnableVertexAttribArray(VERTEX_ATTRIB_POSITION);

    glVertexAttribPointer(VERTEX_ATTRIB_TEXTURE, 2, GL_FLOAT, GL_FALSE, sizeof(VertexData), reinterpret_cast<GLvoid*>(NULL + offsetof(VertexData, textureCoords)));
    glEnableVertexAttribArray(VERTEX_ATTRIB_TEXTURE);

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

    glBindVertexArrayOES(this->glVertexArray);

    if (!this->shader->PrepareToRender())
    {
        LogError("Shader prepare to render has failed.");
        return;
    }

    glDrawArrays(GL_TRIANGLE_STRIP, 0, this->vertexCount);
}