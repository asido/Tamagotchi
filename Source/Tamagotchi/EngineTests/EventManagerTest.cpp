#include <cppunit/config/SourcePrefix.h>
#include <FastDelegate.h>
#include "EventManagerTest.h"
#include "EventManager/EventManager.h"
#include "Clock.h"

CPPUNIT_TEST_SUITE_REGISTRATION(EventManagerTest);

void EventManagerTest::AddListenerTest()
{
    CPPUNIT_ASSERT(EventManager::Get().AddListener(fastdelegate::MakeDelegate(this, &EventManagerTest::TestEventHandler), Event_Test::Type));
    CPPUNIT_ASSERT(EventManager::Get().RemoveListener(fastdelegate::MakeDelegate(this, &EventManagerTest::TestEventHandler), Event_Test::Type));
    CPPUNIT_ASSERT(!EventManager::Get().RemoveListener(fastdelegate::MakeDelegate(this, &EventManagerTest::TestEventHandler), Event_Test::Type));
}

void EventManagerTest::TriggerEventTest()
{
    CPPUNIT_ASSERT(EventManager::Get().AddListener(fastdelegate::MakeDelegate(this, &EventManagerTest::TestEventHandler), Event_Test::Type));
    this->handlerCalled = false;
    EventManager::Get().TriggerEvent(std::shared_ptr<IEvent>(new Event_Test(10)));
    CPPUNIT_ASSERT(this->handlerCalled == true);
    CPPUNIT_ASSERT(EventManager::Get().RemoveListener(fastdelegate::MakeDelegate(this, &EventManagerTest::TestEventHandler), Event_Test::Type));
}

void EventManagerTest::QueueEventTest()
{
    // We have to init the clock or EventManager::Update will crash.
    Clock::Init();

    CPPUNIT_ASSERT(EventManager::Get().AddListener(fastdelegate::MakeDelegate(this, &EventManagerTest::TestEventHandler), Event_Test::Type));
    this->handlerCalled = false;
    CPPUNIT_ASSERT(EventManager::Get().QueueEvent(std::shared_ptr<IEvent>(new Event_Test(10))));
    CPPUNIT_ASSERT(this->handlerCalled == false);
    CPPUNIT_ASSERT(EventManager::Get().Update());
    CPPUNIT_ASSERT(this->handlerCalled == true);
    CPPUNIT_ASSERT(EventManager::Get().RemoveListener(fastdelegate::MakeDelegate(this, &EventManagerTest::TestEventHandler), Event_Test::Type));
}

void EventManagerTest::AbortEventTest()
{
    // We have to init the clock or EventManager::Update will crash.
    Clock::Init();

    CPPUNIT_ASSERT(EventManager::Get().AddListener(fastdelegate::MakeDelegate(this, &EventManagerTest::TestEventHandler), Event_Test::Type));
    this->handlerCalled = false;
    CPPUNIT_ASSERT(EventManager::Get().QueueEvent(std::shared_ptr<IEvent>(new Event_Test(10))));
    CPPUNIT_ASSERT(this->handlerCalled == false);
    CPPUNIT_ASSERT(EventManager::Get().AbortEvent(Event_Test::Type));
    CPPUNIT_ASSERT(this->handlerCalled == false);
    CPPUNIT_ASSERT(EventManager::Get().Update());
    CPPUNIT_ASSERT(this->handlerCalled == false);
    CPPUNIT_ASSERT(EventManager::Get().RemoveListener(fastdelegate::MakeDelegate(this, &EventManagerTest::TestEventHandler), Event_Test::Type));
}

//-----------------------------------------------
// Test event handler
//-----------------------------------------------

void EventManagerTest::TestEventHandler(std::shared_ptr<IEvent> event)
{
    std::shared_ptr<Event_Test> e = std::static_pointer_cast<Event_Test>(event);
    CPPUNIT_ASSERT(e->GetTestValue() == 10);
    this->handlerCalled = true;
}