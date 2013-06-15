#ifndef __ACTORFACTORY_H__
#define __ACTORFACTORY_H__

#include <string>
#include <memory>
#include <tinyxml2.h>
#include "Actor.h"
#include "ObjectFactory.h"

#include <Eigen/Dense>
using namespace Eigen;

//-----------------------------------------------------------------------------------------------------------
//  class ActorFactory
//-----------------------------------------------------------------------------------------------------------

class ActorFactory
{
public:
	ActorFactory();

	std::shared_ptr<Actor>	CreateActor(const std::string &actorResource, tinyxml2::XMLElement *overrides, const Matrix4f *initialTransform);
	void					ModifyActor(std::shared_ptr<Actor> actor, tinyxml2::XMLElement *overrides);

private:
	ActorId							GetNextActorId();
	std::shared_ptr<ActorComponent>	CreateComponent(tinyxml2::XMLElement *data);

private:
	GenericObjectFactory<ActorComponent, ComponentId> componentFactory;
	ActorId lastActorId;
};

#endif // __ACTORFACTORY_H__