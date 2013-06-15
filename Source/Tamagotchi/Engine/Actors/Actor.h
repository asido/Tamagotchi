#ifndef __ACTOR_H__
#define __ACTOR_H__

#include <map>
#include <memory>
#include <tinyxml2.h>
#include "ActorComponent.h"

class XMLElement;

typedef unsigned int ActorId;
typedef std::map< ComponentId, std::shared_ptr<ActorComponent> > ActorComponents;

const ActorId INVALID_ACTOR_ID = 0;

class Actor
{
public:
	bool Init(tinyxml2::XMLElement *data);
	void PostInit();
	void Destroy();
	void Update(float delta);

	template <class ComponentType>
	std::shared_ptr<ComponentType> GetComponent(ComponentId id)
	{
		ActorComponents::iterator it = this->components.find(id);

		if (it != this->components.end())
		{
			return std::shared_ptr<ComponentType>(it->second);
		}
		else
		{
			return std::shared_ptr<ComponentType>();
		}
	}

	ActorId GetId() const { return this->id; }

private:
	ActorId			id;
	ActorComponents	components;
};

#endif // __ACTOR_H__