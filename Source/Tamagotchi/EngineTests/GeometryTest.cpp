#include <cppunit/config/SourcePrefix.h>

#include <Eigen/Dense>
using namespace Eigen;

#include "GeometryTest.h"

// We need to access private std::stack
#define private public
#include "Geometry.h"
#undef private

CPPUNIT_TEST_SUITE_REGISTRATION(GeometryTest);

void GeometryTest::MatrixStackTest()
{
    MatrixStack matrixStack;
    CPPUNIT_ASSERT(matrixStack.stack.size() == 1);
    
    Matrix4f m1;
    m1 << 1.0f, 0.0f, 0.0f, 4.0f,
          0.0f, 1.0f, 0.0f, 4.0f,
          0.0f, 0.0f, 1.0f, 0.0f,
          0.0f, 0.0f, 0.0f, 1.0f;
    Matrix4f m2;
    m2 << 1.0f, 0.0f, 0.0f, 5.0f,
          0.0f, 1.0f, 0.0f, 5.0f,
          0.0f, 0.0f, 1.0f, 0.0f,
          0.0f, 0.0f, 0.0f, 1.0f;

    matrixStack.Push(m1);
    CPPUNIT_ASSERT(matrixStack.stack.size() == 2);
    CPPUNIT_ASSERT(matrixStack.GetTop() == m1);

    matrixStack.Push(m2);
    CPPUNIT_ASSERT(matrixStack.stack.size() == 3);
    CPPUNIT_ASSERT(matrixStack.GetTop() == m2);

    matrixStack.Push(m1);
    CPPUNIT_ASSERT(matrixStack.stack.size() == 4);
    CPPUNIT_ASSERT(matrixStack.GetTop() == m1);

    matrixStack.MultiplyWithTop(m2);
    CPPUNIT_ASSERT(matrixStack.stack.size() == 4);
    CPPUNIT_ASSERT(matrixStack.GetTop() == (m1 * m2));

    matrixStack.Pop();
    CPPUNIT_ASSERT(matrixStack.stack.size() == 3);

    matrixStack.Pop();
    CPPUNIT_ASSERT(matrixStack.stack.size() == 2);

    matrixStack.Pop();
    CPPUNIT_ASSERT(matrixStack.stack.size() == 1);

    matrixStack.Pop();
    CPPUNIT_ASSERT(matrixStack.stack.size() == 0);
}