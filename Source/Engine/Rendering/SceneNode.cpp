#include "SceneNode.h"
#include "Scene.h"
#include "Logger.h"
#include "TamagotchiEngine.h"
#include "ShaderManager.h"
#include "Camera.h"
#include "Actors/RenderComponent.h"
#include "Actors/TransformComponent.h"
#include "ResourceManager/ResourceManager.h"
#include "ResourceManager/TextureResource.h"

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

bool SceneNode::OnPreRender(Scene &scene)
{
    std::shared_ptr<Actor> actor = g_engine->GetGameLogic()->GetActor(this->actorId);
    if (!actor)
    {
        // During sanity check we have to push some matrix or otherwise Pop() at SceneNode::OnPostRender will screw up the stack.
        scene.PushMatrix(Matrix4f::Identity());
        return false;
    }

    std::shared_ptr<TransformComponent> transformComp = actor->GetComponent<TransformComponent>(TransformComponent::GetIdStatic());
    if (!transformComp)
    {
        scene.PushMatrix(Matrix4f::Identity());
        return false;
    }

    scene.PushMatrix(transformComp->GetTransform());
    return true;
}

void SceneNode::OnRenderChildren(Scene &scene)
{
    for (SceneNodeList::iterator it = this->childNodes.begin(), end = this->childNodes.end(); it != end; ++it)
    {
        std::shared_ptr<SceneNode> node = *it;

        if (node->IsVisible())
        {
            if (node->OnPreRender(scene))
            {
                node->OnRender(scene);
                node->OnRenderChildren(scene);
                node->OnPostRender(scene);
            }
        }
    }
}

void SceneNode::OnPostRender(Scene &scene)
{
    scene.PopMatrix();
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

unsigned int SpriteSceneNode::VertexCount(ARRAY_SIZE(SpriteVerticies));

SpriteSceneNode::SpriteSceneNode(ActorId actorId, std::weak_ptr<RenderComponent> renderComp)
    : SceneNode(actorId, renderComp)
{
    LogSpam("SpriteSceneNode created for actor: %d", GetActorId());
}

SpriteSceneNode::~SpriteSceneNode()
{
    LogSpam("SpriteSceneNode destroyed for actor: %d", GetActorId());
}

bool SpriteSceneNode::Init()
{
    // Init shader.
    const std::string &shaderName = GetRenderComponent()->GetShaderName();
    this->shader = std::static_pointer_cast<SpriteShader>(g_engine->GetShaderManager()->GetShader(shaderName));
    if (!this->shader)
    {
        LogError("Failed to load shader: %s", shaderName.c_str());
        return false;
    }

    // Init VBO.
    glGenVertexArraysOES(1, &this->glVertexArray);
    glBindVertexArrayOES(this->glVertexArray);

    glGenBuffers(1, &this->glBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, this->glBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(SpriteVerticies), SpriteVerticies, GL_STATIC_DRAW);

    // Position coord vertex data.
    glVertexAttribPointer(DEFAULT_VERTEX_ATTRIB_POSITION, 3, GL_FLOAT, GL_FALSE, sizeof(DefaultVertexData), static_cast<GLvoid*>(NULL + offsetof(DefaultVertexData, positionCoords)));
    glEnableVertexAttribArray(DEFAULT_VERTEX_ATTRIB_POSITION);

    // Texture coord vertex data.
    glVertexAttribPointer(DEFAULT_VERTEX_ATTRIB_TEXTURE, 2, GL_FLOAT, GL_FALSE, sizeof(DefaultVertexData), reinterpret_cast<GLvoid*>(NULL + offsetof(DefaultVertexData, textureCoords)));
    glEnableVertexAttribArray(DEFAULT_VERTEX_ATTRIB_TEXTURE);

    GL_CHECK_ERROR();

    LogSpam("SpriteSceneNode initialized for actor: %d", GetActorId());

    return true;
}

void SpriteSceneNode::OnRender(Scene &scene)
{
    std::shared_ptr<SpriteRenderComponent> renderComponent = std::static_pointer_cast<SpriteRenderComponent>(this->GetRenderComponent());
    if (!renderComponent)
    {
        LogWarning("renderComponent == NULL");
        return;
    }

    glBindVertexArrayOES(this->glVertexArray);

    const Resource r(renderComponent->GetTextureFilename());
    std::shared_ptr<ResourceHandle> textureHandle = g_engine->GetResourceManager()->GetHandle(r);
    std::shared_ptr<GLESTextureResourceExtraData> textureExtra = std::static_pointer_cast<GLESTextureResourceExtraData>(textureHandle->GetExtra());

    this->shader->SetTexture(textureExtra->GetTexture());
    
    Matrix4f mvp = scene.GetCamera()->CalculateMVP(scene);
    this->shader->SetMvpMatrix(mvp);

    if (!this->shader->PrepareToRender())
    {
        LogError("Shader prepare to render has failed.");
        return;
    }

    glDrawArrays(GL_TRIANGLE_STRIP, 0, SpriteSceneNode::VertexCount);

    GL_CHECK_ERROR();
}