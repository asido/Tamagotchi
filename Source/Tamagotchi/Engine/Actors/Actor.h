#ifndef __ACTOR_H__
#define __ACTOR_H__

#include <map>
#include <memory>
#include "ActorComponent.h"

typedef unsigned int ActorId;
typedef std::map< ComponentId, std::shared_ptr<ActorComponent> > ActorComponents;

const ActorId INVALID_ACTOR_ID = 0;

class Actor
{
public:
	explicit Actor(ActorId id);
	~Actor();

	bool Init();
	void PostInit();
	void Update(float delta);

	bool AddComponent(std::shared_ptr<ActorComponent> component);

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