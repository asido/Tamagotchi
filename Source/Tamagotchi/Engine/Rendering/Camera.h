#ifndef __CAMERA_H__
#define __CAMERA_H__

#include <Eigen/Dense>
using namespace Eigen;

class Scene;

class Camera
{
#if !defined(DEBUG) && !defined(_DEBUG)
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW
#endif

public:
    Camera();

    Matrix4f    CalculateMVP(const Scene &scene) const;

    void        SetProjectionMatrix(const Matrix4f &newMatrix) { this->projectionMatrix = newMatrix; }
    Matrix4f    GetViewMatrix() const { return this->viewMatrix; }

private:
    Matrix4f projectionMatrix;
    Matrix4f viewMatrix;
};

#endif // __CAMERA_H__