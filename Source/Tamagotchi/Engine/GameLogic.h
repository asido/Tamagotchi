#ifndef __GAMELOGIC_H__
#define __GAMELOGIC_H__

#include <memory>
#include <map>
#include <list>

#include "Actors/Actor.h"

class GameView;
class ActorFactory;
class XMLElement;

typedef std::map< ActorId, std::shared_ptr<Actor> > ActorMap;
typedef std::list< std::shared_ptr<GameView> >      GameViewList;

class GameLogic
{
public:
    GameLogic();
    ~GameLogic();

    bool Init();
    
    virtual void                    AddGameView(std::shared_ptr<GameView> gameView, ActorId actorId=INVALID_ACTOR_ID);
    virtual void                    RemoveGameView(std::shared_ptr<GameView> gameView);

    virtual std::shared_ptr<Actor>  GetActor(ActorId actorId);
    virtual std::shared_ptr<Actor>  CreateActor(const std::string &actorResource, tinyxml2::XMLElement *override, const Eigen::Matrix4f *initialTransform);
    virtual void                    DestroyActor(ActorId actorId);

    virtual void                    OnUpdate(float delta);

private:
    ActorMap                        actors;
    GameViewList                    gameViews;
    std::shared_ptr<ActorFactory>   actorFactory;
};

#endif // __GAMELOGIC_H__