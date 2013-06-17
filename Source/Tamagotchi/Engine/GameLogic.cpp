#include "GameLogic.h"
#include "GameView.h"
#include "Actors/ActorFactory.h"
#include "defines.h"

GameLogic::GameLogic()
{

}

GameLogic::~GameLogic()
{

}

bool GameLogic::Init()
{
    this->actorFactory = std::shared_ptr<ActorFactory>(TG_NEW ActorFactory);
    return true;
}

void GameLogic::AddGameView(std::shared_ptr<GameView> gameView, ActorId actorId)
{

}

void GameLogic::RemoveGameView(std::shared_ptr<GameView> gameView)
{

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