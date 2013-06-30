#ifndef __CAMERA_H__
#define __CAMERA_H__

#include <Eigen/Dense>
using namespace Eigen;

class Camera
{
#if !defined(DEBUG) && !defined(_DEBUG)
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW
#endif

public:
    Camera();

    Matrix4f GetViewMatrix() const { return this->viewMatrix; }

private:
    Matrix4f viewMatrix;
};

#endif // __CAMERA_H__