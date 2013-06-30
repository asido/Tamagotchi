#include "Camera.h"
#include "Scene.h"

Camera::Camera()
    : projectionMatrix(Matrix4f::Identity()), viewMatrix(Matrix4f::Identity())
{

}

Matrix4f Camera::CalculateMVP(const Scene &scene) const
{
    Matrix4f world = scene.GetTopMatrix();
    Matrix4f mvp = world * this->viewMatrix;
    return this->projectionMatrix * mvp;
}