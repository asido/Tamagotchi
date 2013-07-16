#ifndef __EVENTMANAGERTEST_H__
#define __EVENTMANAGERTEST_H__

#include <memory>
#include <cppunit/extensions/HelperMacros.h>

//-----------------------------------------------------------------------------------------------------------
//  class EventManagerTest
//-----------------------------------------------------------------------------------------------------------

class IEvent;

class EventManagerTest : public CPPUNIT_NS::TestFixture
{
    CPPUNIT_TEST_SUITE(EventManagerTest);
    CPPUNIT_TEST(AddListenerTest);
    CPPUNIT_TEST(TriggerEventTest);
    CPPUNIT_TEST(QueueEventTest);
    CPPUNIT_TEST(AbortEventTest);
    CPPUNIT_TEST_SUITE_END();

public:
    void AddListenerTest();
    void TriggerEventTest();
    void QueueEventTest();
    void AbortEventTest();

    void TestEventHandler(std::shared_ptr<IEvent> event);

private:
    bool handlerCalled;
};

#endif // __EVENTMANAGERTEST_H__