#include "vector4d.h"
#include <cmath>
#include <QDebug>

namespace gs2019
{
Vector4D::Vector4D(float inputX, float inputY, float inputZ, float inputW)
{
    x = inputX;
    y = inputY;
    z = inputZ;
    w = inputW;
}

Vector4D::Vector4D(Vector3D inputVec3D, float inputW)
{
    x = inputVec3D.x;
    y = inputVec3D.y;
    z = inputVec3D.z;

    w = inputW;
}

const Vector4D& Vector4D::operator = (const Vector4D& v)
{
    x = v.x;
    y = v.y;
    z = v.z;
    w = v.w;

    return *this;
}

Vector4D Vector4D::operator + (const Vector4D& v) const
{
    Vector4D temp;

    temp.x = x+v.x;
    temp.y = y+v.y;
    temp.z = z+v.z;
    temp.w = w+v.w;

    return temp;
}

Vector4D Vector4D::operator - (const Vector4D& v) const
{
    Vector4D temp;

    temp.x = x-v.x;
    temp.y = y-v.y;
    temp.z = z-v.z;
    temp.w = w-v.w;

    return temp;
}

float Vector4D::operator * (const Vector4D& v) const
{
    return ((x * v.x) + (y * v.y) + (z * v.z) + (w * v.w));
}

Vector4D Vector4D::operator ^ (const Vector4D& v) const
{
//    float i;
//    float j;
//    float k;
//    float l;

//    float u1 = x;
//    float u2 = y;
//    float u3 = z;
//    float u4 = w;

//    float v1 = v.x;
//    float v2 = v.y;
//    float v3 = v.z;
//    float v4 = v.w;

//    i = (u2*v3)-(u3*v2);
//    j = (u1*v3)-(u3*v1);
//    k = (u1*v2)-(u2*v1);

//    j = j*(-1);

//    return Vector4D(i, j, k);
    return Vector4D(0, 0, 0, 0);
}

float Vector4D::length() const
{
    return sqrt((x*x)+(y*y)+(z*z)+(w*w));
}

void Vector4D::normalize()
{
    x = x/length();
    y = y/length();
    z = z/length();
    w = w/length();
}

Vector4D Vector4D::operator * (float c) const
{
    Vector4D multipliedVec;

    multipliedVec.x = x;
    multipliedVec.y = y;
    multipliedVec.z = z;
    multipliedVec.w = w;

    multipliedVec.x = x*c;
    multipliedVec.y = y*c;
    multipliedVec.z = z*c;
    multipliedVec.w = w*c;

    return multipliedVec;
}

float& Vector4D::operator[] (const int index)
{
    assert(index >= 0 && index <= 3);

    switch (index)
    {
    case 0:
        return x;
    case 1:
        return y;
    case 2:
        return z;
    case 3:
        return w;
    default:
        qDebug() << "ERROR, SPECIFIED VECTOR-4D INDEX IS OUT OF BOUNDS!!!";
        return w;
    }
}

void Vector4D::debugPrint()
{
    qDebug() << x << "  " << y << " " << z << " " << w;
    qDebug() << "\n";
}

}
