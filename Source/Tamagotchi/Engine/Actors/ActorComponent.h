#ifndef __ACTORCOMPONENT_H__
#define __ACTORCOMPONENT_H__

#include <tinyxml2.h>

typedef unsigned int ComponentId;

const ComponentId INVALID_COMPONENT_ID = 0;

//-----------------------------------------------------------------------------------------------------------
//  class ActorComponent
//-----------------------------------------------------------------------------------------------------------

class ActorComponent
{
public:
	virtual bool Init(tinyxml2::XMLElement *data) = 0;
	virtual void PostInit() {}
	virtual void Update(float delta) {}
};

#endif // __ACTORCOMPONENT_H__