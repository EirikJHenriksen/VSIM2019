#include "vector3d.h"
#include <cmath>

namespace gs2019
{
Vector3D::Vector3D(float a, float b, float c)
{
    x = a;
    y = b;
    z = c;
}

const Vector3D& Vector3D::operator = (const Vector3D& v)
{
    x = v.x;
    y = v.y;
    z = v.z;

    return *this;
}

Vector3D Vector3D::operator + (const Vector3D& v) const
{
    Vector3D temp;

    temp.x = x+v.x;
    temp.y = y+v.y;
    temp.z = z+v.z;

    return temp;
}

Vector3D Vector3D::operator - (const Vector3D& v) const
{
    Vector3D temp;

    temp.x = x-v.x;
    temp.y = y-v.y;
    temp.z = z-v.z;

    return temp;
}

float Vector3D::operator * (const Vector3D& v) const
{
    return ((x * v.x) + (y * v.y) + (z * v.z));
}

Vector3D Vector3D::operator ^ (const Vector3D& v) const
{
    float i;
    float j;
    float k;

    float u1 = x;
    float u2 = y;
    float u3 = z;

    float v1 = v.x;
    float v2 = v.y;
    float v3 = v.z;

    i = (u2*v3)-(u3*v2);
    j = (u1*v3)-(u3*v1);
    k = (u1*v2)-(u2*v1);

    j = j*(-1);

    return Vector3D(i, j, k);
}

bool Vector3D::operator < (const Vector3D& v) const
{
    float thisVectorLength = sqrt((x*x)+(y*y)+(z*z));
    float otherVectorLength = sqrt((v.x*v.x)+(v.y*v.y)+(v.z*v.z));

    if (thisVectorLength < otherVectorLength) {return true;};
    return false;
}

bool Vector3D::operator > (const Vector3D& v) const
{
    float thisVectorLength = sqrt((x*x)+(y*y)+(z*z));
    float otherVectorLength = sqrt((v.x*v.x)+(v.y*v.y)+(v.z*v.z));

    if (thisVectorLength > otherVectorLength) {return true;};
    return false;
}

double Vector3D::length() const
{
    return sqrt((x*x)+(y*y)+(z*z));
}

void Vector3D::normalize()
{
    double newX = x/length();
    double newY = y/length();
    double newZ = z/length();

    x = newX;
    y = newY;
    z = newZ;
}

Vector3D Vector3D::operator * (float c) const
{
    Vector3D multipliedVec;

    multipliedVec.x = x;
    multipliedVec.y = y;
    multipliedVec.z = z;

    multipliedVec.x = x*c;
    multipliedVec.y = y*c;
    multipliedVec.z = z*c;

    return multipliedVec;
}

float& Vector3D::operator[] (const int index)
{
    assert(index >= 0 && index <= 2);

    switch (index)
    {
    case 0:
        return x;
    case 1:
        return y;
    case 2:
        return z;
    default:
        qDebug() << "ERROR, SPECIFIED VECTOR-3D INDEX IS OUT OF BOUNDS!!!";
        return z;
    }
}

void Vector3D::debugPrint()
{
    qDebug() << x << "    " << y << "   " << z;
    qDebug() << "\n";
}
}
