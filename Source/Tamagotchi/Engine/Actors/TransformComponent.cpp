#include "TransformComponent.h"
#include "StringUtilities.h"

TransformComponent::TransformComponent()
	: transform(Matrix4f::Identity())
{
	
}

bool TransformComponent::Init(tinyxml2::XMLElement *data)
{
	return false;
}

ComponentId TransformComponent::GetId()
{
	static ComponentId id = NULL;

	if (!id)
	{
		id = StringUtilities::Hash(this->GetName());
	}
	return id;
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