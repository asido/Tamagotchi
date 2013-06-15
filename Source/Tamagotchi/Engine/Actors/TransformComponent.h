#ifndef __TRANSFORMCOMPONENT_H__
#define __TRANSFORMCOMPONENT_H__

#include <memory>

#include <Eigen/Dense>
using namespace Eigen;

#include <tinyxml2.h>

#include "ActorComponent.h"

//-----------------------------------------------------------------------------------------------------------
//  class TransformComponent
//-----------------------------------------------------------------------------------------------------------

class TransformComponent : public ActorComponent
{
public:
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW

    TransformComponent();

    virtual bool                Init(tinyxml2::XMLElement *data) override;

    virtual ComponentId         GetId() override;
    virtual const std::string   GetName() const override { return "TransformComponent"; };

    Matrix4f                    GetTransform() const;
    void                        SetTransform(const Matrix4f &newTransform);
    Vector3f                    GetPosition() const;
    void                        SetPosition(const Vector3f &newPosition);

private:
    Matrix4f    transform;
};

#endif // __TRANSFORMCOMPONENT_H__