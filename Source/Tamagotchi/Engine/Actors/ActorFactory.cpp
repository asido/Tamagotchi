#include "ActorFactory.h"

//-----------------------------------------------------------------------------------------------------------
//  class ActorFactory
//-----------------------------------------------------------------------------------------------------------

ActorFactory::ActorFactory()
	: lastActorId(INVALID_ACTOR_ID)
{
	// TODO: register all actor components. i.e:
	//this->componentFactory.Register<SomeComponent>(SomeComponent::ComponentId);
}

//-----------------------------------------------
// Public
//-----------------------------------------------

std::shared_ptr<Actor> ActorFactory::CreateActor(const std::string &actorResource, tinyxml2::XMLElement *overrides /*, const mat4x4 initialTransform */)
{
	return std::shared_ptr<Actor>();
}

void ActorFactory::ModifyActor(std::shared_ptr<Actor> actor, tinyxml2::XMLElement *overrides)
{

}

//-----------------------------------------------
// Private
//-----------------------------------------------

ActorId ActorFactory::GetNextActorId()
{
	this->lastActorId++;
	return this->lastActorId;
}