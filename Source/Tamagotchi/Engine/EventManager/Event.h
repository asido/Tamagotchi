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
    virtual EventType           GetEventType() const = 0;
    virtual const std::string&  GetName() const = 0;
};


//-----------------------------------------------------------------------------------------------------------
//  Events
//-----------------------------------------------------------------------------------------------------------

class Event_Test : public IEvent
{
public:
    static const EventType Type;

    Event_Test(int value);

    virtual EventType           GetEventType() const override;
    virtual const std::string&  GetName() const override;

    int                         GetTestValue() const { return testValue; }

private:
    int testValue;
};

#endif // __EVENT_H__