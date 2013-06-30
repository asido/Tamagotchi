#ifndef __GEOMETRY_H__
#define __GEOMETRY_H__

#include <stack>

#include <Eigen/Dense>
using namespace Eigen;

//-----------------------------------------------------------------------------------------------------------
//  class Math3D
//-----------------------------------------------------------------------------------------------------------

class Math3D
{
#if !defined(DEBUG) && !defined(_DEBUG)
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW
#endif
        
public:
    static Matrix4f MakeOrthoMatrix(float left, float right,
                                    float bottom, float top,
                                    float nearZ, float farZ);
};


//-----------------------------------------------------------------------------------------------------------
//  class MatrixStack
//-----------------------------------------------------------------------------------------------------------

class MatrixStack
{
public:
    MatrixStack();
    ~MatrixStack();

    void        Push(const Matrix4f &matrix);
    void        Pop();
    Matrix4f    GetTop() const;
    void        MultiplyWithTop(const Matrix4f &matrix);

private:
    std::stack<Matrix4f> stack;
};

#endif // __GEOMETRY_H__