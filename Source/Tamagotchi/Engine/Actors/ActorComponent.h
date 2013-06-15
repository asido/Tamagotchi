#ifndef __ACTORCOMPONENT_H__
#define __ACTORCOMPONENT_H__

#include <memory>
#include <string>

#include <tinyxml2.h>

#include <Eigen/Dense>
using namespace Eigen;

#include "Logger.h"

class Actor;

typedef unsigned int ComponentId;

const ComponentId INVALID_COMPONENT_ID = 0;

//-----------------------------------------------------------------------------------------------------------
//  class ActorComponent
//-----------------------------------------------------------------------------------------------------------

class ActorComponent
{
	friend class ActorFactory;

public:
	virtual bool				Init(tinyxml2::XMLElement *data) = 0;
	virtual void				PostInit() {}
	virtual void				Update(float delta) {}

	virtual ComponentId			GetId() = 0;
	virtual const std::string	GetName() const = 0;

private:
	void						SetOwner(std::weak_ptr<Actor> owner) { this->owner = owner; }
	std::shared_ptr<Actor>		GetOwner() const
	{
		std::shared_ptr<Actor> o = this->owner.lock();
		if (!o)
		{
			LogWarning("%s::owner == NULL", this->GetName());
		}
		return o;
	}

private:
	std::weak_ptr<Actor> owner;
};

#endif // __ACTORCOMPONENT_H__