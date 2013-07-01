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
    LogSpam("Transform component created.");
}

TransformComponent::~TransformComponent()
{
    LogSpam("Transform component destroyed.");
}

bool TransformComponent::Init(tinyxml2::XMLElement *data)
{
    if (!data)
    {
        LogError("XML data == NULL");
        return false;
    }

    tinyxml2::XMLElement *positionElement = data->FirstChildElement("Position");
    if (positionElement)
    {
        Vector3f pos(static_cast<float>(atof(positionElement->Attribute("x"))),
                     static_cast<float>(atof(positionElement->Attribute("y"))),
                     static_cast<float>(atof(positionElement->Attribute("z"))));
        this->SetPosition(pos);
    }
    else
    {
        LogWarning("Transform component data has no position element.");
    }

    tinyxml2::XMLElement *scaleElement = data->FirstChildElement("Scale");
    if (scaleElement)
    {
        Vector3f scale(static_cast<float>(atof(scaleElement->Attribute("x"))),
                       static_cast<float>(atof(scaleElement->Attribute("y"))),
                       static_cast<float>(atof(scaleElement->Attribute("z"))));
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
    return Vector3f(this->transform(0,3),
                    this->transform(1,3),
                    this->transform(2,3));
}

void TransformComponent::SetPosition(const Vector3f &newPosition)
{
    this->transform(0,3) = newPosition(0);
    this->transform(1,3) = newPosition(1);
    this->transform(2,3) = newPosition(2);
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