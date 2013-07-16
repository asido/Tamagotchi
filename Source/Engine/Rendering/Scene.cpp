#include "Scene.h"
#include "SceneNode.h"
#include "Camera.h"
#include "Geometry.h"
#include "TamagotchiEngine.h"
#include "Rendering/Renderer.h"
#include "EventManager/EventManager.h"

//-----------------------------------------------------------------------------------------------------------
//  class Scene
//-----------------------------------------------------------------------------------------------------------

//-----------------------------------------------
// Public
//-----------------------------------------------

Scene::Scene()
    : rootNode(TG_NEW RootSceneNode), camera(TG_NEW Camera), matrixStack(TG_NEW MatrixStack)
{
    std::shared_ptr<IRenderer> renderer = g_engine->GetRenderer();
    Matrix4f projectionMatrix = Math3D::MakeOrthoMatrix(0.0f, 100.0f * renderer->GetAspectRatio(), 0.0f, 100.0f, 0.0f, 100.0f);
    this->camera->SetProjectionMatrix(projectionMatrix);

    EventManager::Get().AddListener(fastdelegate::MakeDelegate(this, &Scene::NewRenderComponentDelegate), Event_NewRenderComponent::Type);

    LogSpam("Scene created.");
}

Scene::~Scene()
{
    EventManager::Get().RemoveListener(fastdelegate::MakeDelegate(this, &Scene::NewRenderComponentDelegate), Event_NewRenderComponent::Type);
    LogSpam("Scene destroyed.");
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

void Scene::PushMatrix(const Matrix4f &matrix)
{
    this->matrixStack->Push(this->matrixStack->GetTop() * matrix);
}

void Scene::PopMatrix()
{
    this->matrixStack->Pop();
}

Matrix4f Scene::GetTopMatrix() const
{
    return this->matrixStack->GetTop();
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