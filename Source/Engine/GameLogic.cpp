#include "GameLogic.h"
#include "UI/GameView.h"
#include "Actors/ActorFactory.h"
#include "UI/HudFactory.h"
#include "ResourceManager/ResourceManager.h"
#include "ResourceManager/XMLResource.h"
#include "TamagotchiEngine.h"
#include "EngineConfig.h"
#include "defines.h"

//-----------------------------------------------------------------------------------------------------------
//  class GameLogic
//-----------------------------------------------------------------------------------------------------------

GameLogic::GameLogic()
{

}

GameLogic::~GameLogic()
{

}

bool GameLogic::Init()
{
    this->actorFactory = std::shared_ptr<ActorFactory>(TG_NEW ActorFactory);
    this->hudFactory = std::shared_ptr<HudFactory>(TG_NEW HudFactory);
    return true;
}

void GameLogic::AddGameView(std::shared_ptr<GameView> gameView, ActorId actorId)
{
    // TODO: do we need this actorId here?

    this->gameViews.push_back(gameView);
}

void GameLogic::RemoveGameView(std::shared_ptr<GameView> gameView)
{
    this->gameViews.remove(gameView);
}

bool GameLogic::LoadScene(const std::string &sceneFile)
{
    const Resource r(g_engine->GetEngineConfig()->GetAssetPathLevels() + sceneFile);
    std::shared_ptr<ResourceHandle> sceneHandle = g_engine->GetResourceManager()->GetHandle(r);
    if (!sceneHandle)
    {
        LogError("Couldn't load scene: %s", r.GetName().c_str());
        return false;
    }

    // Reset game view scene.
    for (GameViewList::iterator it = this->gameViews.begin(), end = this->gameViews.end(); it != end; ++it)
    {
        std::shared_ptr<GameView> gameView = *it;
        gameView->ResetScene();
    }

    // Load XML root element.
    std::shared_ptr<XMLResourceExtraData> sceneExtra = std::static_pointer_cast<XMLResourceExtraData>(sceneHandle->GetExtra());
    tinyxml2::XMLElement *root = sceneExtra->GetRoot();

    // Create static scene actors.
    tinyxml2::XMLElement *actorsElement = root->FirstChildElement("StaticActors");
    if (actorsElement)
    {
        for (tinyxml2::XMLElement *actorElement = actorsElement->FirstChildElement(); actorElement; actorElement = actorElement->NextSiblingElement())
        {
            CreateActor(actorElement->Attribute("resource"), actorElement);
        }
    }

    // Whenever we add more game view support, this certainly will need to change and so raise an error in order not to forget in the future.
    LogAssert(this->gameViews.size() == 1 && "WARNING *** Create HUD for human view only.");

    // Create HUD elements if game view is attached.
    std::shared_ptr<GameView> gameView = this->gameViews.front();
    if (gameView)
    {
        tinyxml2::XMLElement *hudElements = root->FirstChildElement("HUD");
        if (hudElements)
        {
            for (tinyxml2::XMLElement *hudElement = hudElements->FirstChildElement(); hudElement; hudElement = hudElement->NextSiblingElement())
            {
                std::shared_ptr<ScreenElement> screenElement = this->hudFactory->CreateScreenElement(hudElement);
                gameView->AddElement(screenElement);
            }
        }
    }
    

    return false;
}

bool GameLogic::UnloadCurrentScene()
{
    this->actors.clear();
    return true;
}

std::shared_ptr<Actor> GameLogic::GetActor(ActorId actorId)
{
    ActorMap::iterator it = this->actors.find(actorId);
    if (it != this->actors.end())
    {
        return it->second;
    }
    return std::shared_ptr<Actor>();
}

std::shared_ptr<Actor> GameLogic::CreateActor(const std::string &actorResource, tinyxml2::XMLElement *overrides, const Eigen::Matrix4f *initialTransform)
{
    std::shared_ptr<Actor> actor = this->actorFactory->CreateActor(actorResource, overrides, initialTransform);

    if (actor)
    {
        this->actors.insert(std::make_pair(actor->GetId(), actor));
    }

    return actor;
}

void GameLogic::DestroyActor(ActorId actorId)
{
    ActorMap::iterator it = this->actors.find(actorId);
    if (it != this->actors.end())
    {
        this->actors.erase(it);
    }
}

void GameLogic::OnUpdate(float delta)
{
    // Update all game views.
    for (GameViewList::iterator it = this->gameViews.begin(); it != this->gameViews.end(); ++it)
    {
        std::shared_ptr<GameView> gameView = *it;
        gameView->OnUpdate(delta);
    }

    // Update all game actors.
    for (ActorMap::const_iterator it = this->actors.begin(); it != this->actors.end(); ++it)
    {
        std::shared_ptr<Actor> actor = it->second;
        actor->OnUpdate(delta);
    }
}