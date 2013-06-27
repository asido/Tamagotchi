#include "TransformComponent.h"
#include "StringUtilities.h"

const std::string TransformComponent::name("TransformComponent");

ComponentId TransformComponent::GetIdStatic()
{
    static ComponentId id = INVALID_COMPONENT_ID;

    if (id == INVALID_COMPONENT_ID)
    {
        id = StringUtilities::Hash(name);
    }
    return id;
}

TransformComponent::TransformComponent()
    : transform(Matrix4f::Identity())
{
    
}

bool TransformComponent::Init(tinyxml2::XMLElement *data)
{
    return false;
}

ComponentId TransformComponent::GetId() const
{
    return GetIdStatic();
}

const std::string& TransformComponent::GetName() const
{
    return name;
}

Matrix4f TransformComponent::GetTransform() const
{
    return this->transform;
}

void TransformComponent::SetTransform(const Matrix4f &newTransform)
{
    this->transform = newTransform;
}

Vector3f TransformComponent::GetPosition() const
{
    return Vector3f(this->transform(3,0),
                    this->transform(3,1),
                    this->transform(3,2));
}

void TransformComponent::SetPosition(const Vector3f &newPosition)
{
    this->transform(3,0) = newPosition(0);
    this->transform(3,1) = newPosition(1);
    this->transform(3,2) = newPosition(2);
}

Vector3f TransformComponent::GetScale() const
{
    return Vector3f(this->transform(0,0),
                    this->transform(1,1),
                    this->transform(2,2));
}

void TransformComponent::SetScale(const Vector3f &newScale)
{
    this->transform(0,0) = newScale(0);
    this->transform(1,1) = newScale(1);
    this->transform(2,2) = newScale(2);
}