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

SceneNode::SceneNode()
    : actorId(INVALID_ACTOR_ID)
{

}

SceneNode::SceneNode(ActorId actor, std::weak_ptr<RenderComponent> renderComp)
    : actorId(actor), renderComponent(renderComp)
{
    LogSpam("SceneNode created for actor: %d", actor);
}

SceneNode::~SceneNode()
{
    LogSpam("SceneNode destroyed for actor: %d", this->actorId);
}

bool SceneNode::AddChild(std::shared_ptr<SceneNode> kid)
{
    kid->SetParent(shared_from_this());
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
    // TODO: for now everything is visible, but we need to do some logic here in order
    // to improve game performance.
    return true;
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
    // TODO: push and set world matrix to actor TransformComponent matrix.
}

void SceneNode::OnRenderChildren(const Scene &scene)
{
    for (SceneNodeList::iterator it = this->childNodes.begin(), end = this->childNodes.end(); it != end; ++it)
    {
        std::shared_ptr<SceneNode> node = *it;

        if (node->IsVisible())
        {
            node->OnPreRender(scene);
            node->OnRender(scene);
            node->OnRenderChildren(scene);
            node->OnPostRender(scene);
        }
    }
}

void SceneNode::OnPostRender(const Scene &scene)
{
    // TODO: pop a matrix pushed during SceneNode::OnUpdate
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
//  class RootSceneNode
//-----------------------------------------------------------------------------------------------------------

RootSceneNode::RootSceneNode()
{
    LogSpam("RootSceneNode created.");
}

RootSceneNode::~RootSceneNode()
{
    LogSpam("RootSceneNode destroyed.");
}


//-----------------------------------------------------------------------------------------------------------
//  class SpriteSceneNode
//-----------------------------------------------------------------------------------------------------------

static const DefaultVertexData SpriteVerticies[] = {
    { Vector3f(0.0f, 0.0f, 0.0f), Vector2f(0.0f, 0.0f) },
    { Vector3f(1.0f, 0.0f, 0.0f), Vector2f(1.0f, 0.0f) },
    { Vector3f(0.0f, 1.0f, 0.0f), Vector2f(0.0f, 1.0f) },
    { Vector3f(1.0f, 1.0f, 0.0f), Vector2f(1.0f, 1.0f) }
};

SpriteSceneNode::SpriteSceneNode(ActorId actorId, std::weak_ptr<RenderComponent> renderComp)
    : SceneNode(actorId, renderComp), vertexCount(4)
{
    LogSpam("SpriteSceneNode created for actor: %d", GetActorId());
}

SpriteSceneNode::~SpriteSceneNode()
{
    LogSpam("SpriteSceneNode destroyed for actor: %d", GetActorId());
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
    glBufferData(GL_ARRAY_BUFFER, sizeof(SpriteVerticies), SpriteVerticies, GL_STATIC_DRAW);

    glVertexAttribPointer(DEFAULT_VERTEX_ATTRIB_POSITION, 3, GL_FLOAT, GL_FALSE, sizeof(DefaultVertexData), static_cast<GLvoid*>(NULL + offsetof(DefaultVertexData, positionCoords)));
    glEnableVertexAttribArray(DEFAULT_VERTEX_ATTRIB_POSITION);

    // TODO: we need sprite shader attributes.
    //glVertexAttribPointer(VERTEX_ATTRIB_TEXTURE, 2, GL_FLOAT, GL_FALSE, sizeof(DefaultVertexData), reinterpret_cast<GLvoid*>(NULL + offsetof(DefaultVertexData, textureCoords)));
    //glEnableVertexAttribArray(VERTEX_ATTRIB_TEXTURE);

    GL_CHECK_ERROR();

    LogSpam("SpriteSceneNode initialized for actor: %d", GetActorId());

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

    GL_CHECK_ERROR();
}