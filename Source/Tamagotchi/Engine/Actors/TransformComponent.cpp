#include "TransformComponent.h"
#include "StringUtilities.h"

//-----------------------------------------------------------------------------------------------------------
//  class TransformComponent
//-----------------------------------------------------------------------------------------------------------

//-----------------------------------------------
// Static
//-----------------------------------------------

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

//-----------------------------------------------
// Public
//-----------------------------------------------

TransformComponent::TransformComponent()
    : transform(Matrix4f::Identity())
{
    
}

bool TransformComponent::Init(tinyxml2::XMLElement *data)
{
    tinyxml2::XMLElement *positionElement = data->FirstChildElement("Position");
    if (positionElement)
    {
        Vector3f pos(atof(positionElement->Attribute("x")),
                     atof(positionElement->Attribute("y")),
                     atof(positionElement->Attribute("z")));
        this->SetPosition(pos);
    }
    else
    {
        LogWarning("Transform component data has no position element.");
    }

    tinyxml2::XMLElement *scaleElement = data->FirstChildElement("Scale");
    if (scaleElement)
    {
        Vector3f scale(atof(scaleElement->Attribute("x")),
                       atof(scaleElement->Attribute("y")),
                       atof(scaleElement->Attribute("z")));
        this->SetScale(scale);
    }
    else
    {
        LogWarning("Transform component data has no scale element.");
    }

    return true;
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