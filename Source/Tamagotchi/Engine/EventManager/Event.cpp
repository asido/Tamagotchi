#include "Event.h"

//-----------------------------------------------------------------------------------------------------------
//  Event_Test
//-----------------------------------------------------------------------------------------------------------

const EventType Event_Test::Type(0x9f85ee47);

Event_Test::Event_Test(int value)
    : testValue(value)
{

}

EventType Event_Test::GetEventType() const
{
    return this->Type;
}

const std::string& Event_Test::GetName() const
{
    static const std::string name("Event_Test");
    return name;
}