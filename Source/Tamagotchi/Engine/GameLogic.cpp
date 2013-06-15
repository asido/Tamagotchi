#include "GameLogic.h"
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

std::shared_ptr<Actor> GameLogic::CreateActor(const std::string &actorResource, XMLElement *overrides /*, const Mat4x4 initialTransform=NULL*/)
{
    return std::shared_ptr<Actor>();
}

void GameLogic::DestroyActor(ActorId actorId)
{

}

void GameLogic::OnUpdate(float delta)
{

}