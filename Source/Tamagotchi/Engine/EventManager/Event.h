#ifndef __EVENT_H__
#define __EVENT_H__

#include <string>

typedef unsigned long EventType;

//-----------------------------------------------------------------------------------------------------------
//  class IEvent
//-----------------------------------------------------------------------------------------------------------

class IEvent
{
public:
    virtual const EventType GetEventType() const = 0;
    virtual const std::string& GetName() const = 0;
};


//-----------------------------------------------------------------------------------------------------------
//  Events
//-----------------------------------------------------------------------------------------------------------

// TODO: declare all necessary events.

#endif // __EVENT_H__