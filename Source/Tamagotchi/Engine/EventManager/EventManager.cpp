#include "EventManager.h"
#include "Logger.h"
#include "Clock.h"

//-----------------------------------------------
// Static
//-----------------------------------------------

EventManager& EventManager::Get()
{
    static EventManager instance;
    return instance;
}

//-----------------------------------------------
// Public
//-----------------------------------------------

EventManager::EventManager()
    : activeQueue(0)
{

}

bool EventManager::AddListener(const EventListenerDelegate &eventDelegate, const EventType type)
{                                             // This will find *OR* create the entry.
    EventListenerList &eventListenerList = this->eventListeners[type];

    for (EventListenerList::iterator it = eventListenerList.begin(); it != eventListenerList.end(); ++it)
    {
        if (eventDelegate == (*it))
        {
            LogWarning("Attempting to double-register a delegate of type: %d", type);
            return false;
        }
    }

    eventListenerList.push_back(eventDelegate);
    return true;
}

bool EventManager::RemoveListener(const EventListenerDelegate &eventDelegate, const EventType type)
{
    bool success = false;

    EventListenerMap::iterator it = this->eventListeners.find(type);
    if (it != this->eventListeners.end())
    {
        EventListenerList &listeners = it->second;
        for (EventListenerList::iterator it = listeners.begin(); it != listeners.end(); ++it)
        {
            if (eventDelegate == (*it))
            {
                listeners.erase(it);
                success = true;
                // We don't need to continue buecause it should be impossible for the same delegate
                // function to be registered for the same event more than once.
                break;
            }
        }
    }

    return success;
}

bool EventManager::TriggerEvent(const std::shared_ptr<IEvent> event) const
{
    LogSpam("Event triggered: " + event->GetName());
    bool processed = false;

    EventListenerMap::const_iterator it = this->eventListeners.find(event->GetEventType());
    if (it != this->eventListeners.end())
    {
        const EventListenerList &listeners = it->second;
        for (EventListenerList::const_iterator it = listeners.begin(); it != listeners.end(); ++it)
        {
            EventListenerDelegate listener = (*it);
            listener(event);
            processed = true;
        }
    }

    return processed;
}

bool EventManager::QueueEvent(const std::shared_ptr<IEvent> event)
{
    if (!event)
    {
        LogError("Invalid event.");
        return false;
    }

    LogSpam("Attempting to queue event: " + event->GetName());

    EventListenerMap::iterator it = this->eventListeners.find(event->GetEventType());
    if (it == this->eventListeners.end())
    {
        LogInfo("No delegates registered for event '" + event->GetName() + "', ignoring.");
        return false;
    }
    else
    {
        this->queues[this->activeQueue].push_back(event);
        LogSpam("Event queued: " + event->GetName());
        return true;
    }
}

bool EventManager::AbortEvent(EventType type, bool all /* = false */)
{
    bool success = false;
    
    EventListenerMap::iterator it = this->eventListeners.find(type);
    if (it != this->eventListeners.end())
    {
        EventQueue &eventQueue = this->queues[this->activeQueue];
        EventQueue::const_iterator it = eventQueue.begin();
        while (it != eventQueue.end())
        {
            EventQueue::const_iterator thisIt = it;
            ++it;

            if ((*thisIt)->GetEventType() == type)
            {
                eventQueue.erase(thisIt);
                success = true;

                if (!all)
                {
                    break;
                }
            }
        }
    }

    return success;
}

bool EventManager::Update(unsigned long maxMillis /* = EVENTMANAGER_INFINITE_MILLIS */)
{
    long long currMs = Clock::GetTimeMilli();
    long long maxMs = (maxMillis == EVENTMANAGER_INFINITE_MILLIS ? EVENTMANAGER_INFINITE_MILLIS : currMs + maxMillis);

    int queueToProcess = this->activeQueue;
    this->activeQueue = (this->activeQueue + 1) % EVENTMANAGER_NUM_QUEUES;
    this->queues[this->activeQueue].clear();

    LogSpam("Processing event queue: %d. Total events: %d.", queueToProcess, this->queues[queueToProcess].size());

    while (!this->queues[queueToProcess].empty())
    {
        std::shared_ptr<IEvent> event = this->queues[queueToProcess].front();
        EventType eventType = event->GetEventType();
        this->queues[queueToProcess].pop_front();
        
        // Find all the delegate functions registered for this event.
        EventListenerMap::const_iterator it = this->eventListeners.find(eventType);
        if (it != this->eventListeners.end())
        {
            const EventListenerList &listeners = it->second;
            LogSpam("Processing event: %s. Found delegates: %d", event->GetName().c_str(), listeners.size());

            // Call each listener.
            for (EventListenerList::const_iterator it = listeners.begin(); it != listeners.end(); ++it)
            {
                EventListenerDelegate listener = (*it);
                listener(event);
            }
        }

        // Check to see if time ran out.
        currMs = Clock::GetTimeMilli();
        if (maxMillis != EVENTMANAGER_INFINITE_MILLIS && currMs >= maxMs)
        {
            LogInfo("Event processing has timed out - aborting.");
            break;
        }
    }

    // If we couldn't process all of the events, push the remaining events to the new active queue.
    // NOTE: To preserve sequencing, go back-to-front, inserting them at the head of the active queue.
    bool queueFlushed = this->queues[queueToProcess].empty();
    if (!queueFlushed)
    {
        LogInfo("%d events couldn't be processed - re-scheduling for another run.", this->queues[queueToProcess].size());

        while (!this->queues[queueToProcess].empty())
        {
            std::shared_ptr<IEvent> event = this->queues[queueToProcess].back();
            this->queues[queueToProcess].pop_back();
            this->queues[this->activeQueue].push_front(event);
        }
    }

    return queueFlushed;
}