#ifndef MATRIX4X4_H
#define MATRIX4X4_H

#include <QOpenGLFunctions_4_1_Core>
#include "vector4d.h"

namespace gs2019
{
class Matrix4x4
{
private:
    float A[4][4]{};

    float DeterminantArray3x3[3][3]{};

    //Antall rader.
    int n = 4;
    //Antall kolonner.
    int m = 4;

public:
    Matrix4x4(float inX1 = 0.f, float inY1 = 0.f, float inZ1 = 0.f, float inW1 = 0.f,
                    float inX2 = 0.f, float inY2 = 0.f, float inZ2 = 0.f, float inW2 = 0.f,
                    float inX3 = 0.f, float inY3 = 0.f, float inZ3 = 0.f, float inW3 = 0.f,
                    float inX4 = 0.f, float inY4 = 0.f, float inZ4 = 0.f, float inW4 = 0.f);

    Matrix4x4(Vector4D row1, Vector4D row2, Vector4D row3, Vector4D row4);

    void setToIdentity();

    float calculateDeterminant();
    float calculateDeterminantHelper(Vector3D row1, Vector3D row2, Vector3D row3);

    void operator* (const float multiplier);
    Vector4D operator* (const Vector4D multiplier);
    Matrix4x4 operator* (const Matrix4x4 multiplier);

    void scale(float inX, float inY, float inZ);
    void rotate(float amount, int inX, int inY, int inZ);
    void translate(float inX, float inY, float inZ);

    void pivot(int row);
    void LU();
    Vector4D solve(Vector4D &b) const;

    float& operator[] (const std::pair<int, int>& index);

    GLfloat *constData();

    void debugPrint();
};
}

#endif // MATRIX4X4_H
