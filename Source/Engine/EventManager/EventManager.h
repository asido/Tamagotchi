#ifndef __EVENTMANAGER_H__
#define __EVENTMANAGER_H__

#include <memory>
#include <map>
#include <list>
#include <FastDelegate.h>
#include "Event.h"

//-----------------------------------------------------------------------------------------------------------
//  class EventManager
//-----------------------------------------------------------------------------------------------------------

class IEvent;

typedef fastdelegate::FastDelegate1< std::shared_ptr<IEvent> >  EventListenerDelegate;
typedef std::list<EventListenerDelegate>                        EventListenerList;
typedef std::map<EventType, EventListenerList>                  EventListenerMap;
typedef std::list< std::shared_ptr<IEvent> >                    EventQueue;

const unsigned int EVENTMANAGER_NUM_QUEUES      = 2;
const unsigned int EVENTMANAGER_INFINITE_MILLIS = 0xFFFFFFFF;

class EventManager
{
public:
    static EventManager& Get();

    EventManager();

    bool AddListener(const EventListenerDelegate &eventDelegate, const EventType type);
    bool RemoveListener(const EventListenerDelegate &eventDelegate, const EventType type);

    bool TriggerEvent(const std::shared_ptr<IEvent> event) const;
    bool QueueEvent(const std::shared_ptr<IEvent> event);
    bool AbortEvent(EventType type, bool all = false);

    bool Update(unsigned long maxMillis = EVENTMANAGER_INFINITE_MILLIS);

private:
    EventListenerMap    eventListeners;
    EventQueue          queues[EVENTMANAGER_NUM_QUEUES];
    int                 activeQueue;
};

#endif // __EVENTMANAGER_H__