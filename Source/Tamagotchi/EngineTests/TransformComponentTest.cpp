#include "TransformComponentTest.h"
#include "Actors/TransformComponent.h"

CPPUNIT_TEST_SUITE_REGISTRATION(TransformComponentTest);

void TransformComponentTest::setUp()
{
    this->transformComponent = new TransformComponent;
    CPPUNIT_ASSERT(this->transformComponent->GetTransform() == Matrix4f::Identity());
}

void TransformComponentTest::tearDown()
{
    delete this->transformComponent;
}

void TransformComponentTest::SetTransformTest()
{
    Matrix4f m;
    m <<     1,  2,  3,  4,
             5,  6,  7,  8,
             9, 10, 11, 12,
            13, 14, 15, 16;

    CPPUNIT_ASSERT(this->transformComponent->GetTransform() == Matrix4f::Identity());
    this->transformComponent->SetTransform(m);
    CPPUNIT_ASSERT(this->transformComponent->GetTransform() == m);
}

void TransformComponentTest::SetPositionTest()
{
    Vector3f newPos(5, 5, 5);
    Vector3f zero(0, 0, 0);

    CPPUNIT_ASSERT(this->transformComponent->GetPosition() == zero);
    this->transformComponent->SetPosition(newPos);
    CPPUNIT_ASSERT(this->transformComponent->GetPosition() == newPos);
}

void TransformComponentTest::SetScaleTest()
{
    Vector3f newScale(2, 2, 2);
    Vector3f defaultScale(1, 1, 1);

    CPPUNIT_ASSERT(this->transformComponent->GetScale() == defaultScale);
    this->transformComponent->SetScale(newScale);
    CPPUNIT_ASSERT(this->transformComponent->GetScale() == newScale);
}