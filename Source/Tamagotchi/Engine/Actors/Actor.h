#ifndef __ACTOR_H__
#define __ACTOR_H__

#include <map>
#include <memory>
#include "ActorComponent.h"

class XMLElement;

typedef unsigned int ActorId;
typedef std::map< ComponentId, std::shared_ptr<ActorComponent> > ActorComponents;

const ActorId INVALID_ACTOR_ID = 0;

class Actor
{
public:
	bool Init(XMLElement *data);
	void PostInit();
	void Destroy();
	void Update(float delta);

	ActorId GetId() const { return this->id; }

	template <class ComponentType>
	std::weak_ptr<ComponentType> GetComponent(ComponentId id)
	{
		ActorComponents::iterator it = this->components.find(id);

		if (it != this->components.end())
		{
			std::shared_ptr<ActorComponent> base(it->second);
			std::shared_ptr<ComponentType>	sub(static_pointer_cast<ComponentType>(base));
			std::weak_ptr<ComponentType>	weakSub(sub);
			return weakSub;
		}
		else
		{
			return std::weak_ptr<ComponentType>();
		}
	}

private:
	ActorId			id;
	ActorComponents	components;
};

#endif // __ACTOR_H__