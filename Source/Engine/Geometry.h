#ifndef __GEOMETRY_H__
#define __GEOMETRY_H__

#include <stack>
#include <vector>
#include <Eigen/StdDeque>

#include <Eigen/Dense>
using namespace Eigen;

//-----------------------------------------------------------------------------------------------------------
//  class Math3D
//-----------------------------------------------------------------------------------------------------------

class Math3D
{
public:
#if !defined(DEBUG)
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW
#endif
        
    static Matrix4f MakeOrthoMatrix(float left, float right,
                                    float bottom, float top,
                                    float nearZ, float farZ);
};


//-----------------------------------------------------------------------------------------------------------
//  class MatrixStack
//-----------------------------------------------------------------------------------------------------------

// We are using std::vector because std::stack doesn't build with specified aligned_allocator.
// std::vector is the same functional and fast as std::stack, therefore we are not missing anything out.
typedef std::vector< Matrix4f, Eigen::aligned_allocator<Matrix4f> > Stack;

class MatrixStack
{
public:
#if !defined(DEBUG)
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW
#endif

    MatrixStack();
    ~MatrixStack();

    void        Push(const Matrix4f &matrix);
    void        Pop();
    Matrix4f    GetTop() const;
    void        MultiplyWithTop(const Matrix4f &matrix);

private:
    Stack stack;
};


//-----------------------------------------------------------------------------------------------------------
//  class Point
//-----------------------------------------------------------------------------------------------------------

class Point
{
public:
    Point(float x, float y) : x(x), y(y) { }

    float GetX() const { return this->x; }
    float GetY() const { return this->y; }

private:
    float x;
    float y;
};

#endif // __GEOMETRY_H__