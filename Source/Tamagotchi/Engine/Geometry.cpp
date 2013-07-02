#include "Geometry.h"
#include "Logger.h"

//-----------------------------------------------------------------------------------------------------------
//  class Math3D
//-----------------------------------------------------------------------------------------------------------

Matrix4f Math3D::MakeOrthoMatrix(float left, float right,
                                 float bottom, float top,
                                 float nearZ, float farZ)
{
    float ral = right + left;
    float rsl = right - left;
    float tab = top + bottom;
    float tsb = top - bottom;
    float fan = farZ + nearZ;
    float fsn = farZ - nearZ;

    Matrix4f m;
    m << 2.0f / rsl, 0.0f, 0.0f, 0.0f,
        0.0f, 2.0f / tsb, 0.0f, 0.0f,
        0.0f, 0.0f, -2.0f / fsn, 0.0f,
        -ral / rsl, -tab / tsb, -fan / fsn, 1.0f;

    // Eigen takes the argument in column-major, thus the transpose.
    // For efficiency you could write the values to matrix in transposed order, but
    // this function isn't called that often, thus I prefer readability.
    m.transposeInPlace();

    return m;
}


//-----------------------------------------------------------------------------------------------------------
//  class MatrixStack
//-----------------------------------------------------------------------------------------------------------

MatrixStack::MatrixStack()
{
    // Keep bottom matrix an identity matrix.
    this->stack.push(Matrix4f::Identity());
}

MatrixStack::~MatrixStack()
{
    while (!this->stack.empty())
    {
        this->stack.pop();
    }
}

void MatrixStack::Push(const Matrix4f &matrix)
{
    this->stack.push(matrix);
}

void MatrixStack::Pop()
{
    LogAssert(!this->stack.empty() && "Attempting to pop() an empty stack.");
    this->stack.pop();
}

Matrix4f MatrixStack::GetTop() const
{
    LogAssert(!this->stack.empty() && "Attempting to top() an empty stack.");
    return this->stack.top();
}

void MatrixStack::MultiplyWithTop(const Matrix4f &matrix)
{
    LogAssert(!this->stack.empty() && "Attempting to MultiplyWithTop() while stack is empty.");
    this->stack.top() *= matrix;
}