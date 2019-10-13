#include "vector2d.h"
#include <cmath>

namespace gs2019
{
Vector2D::Vector2D(float a, float b)
{
    x = a;
    y = b;
}

const Vector2D& Vector2D::operator = (const Vector2D& v)
{
    x = v.x;
    y = v.y;

    return *this;
}

Vector2D Vector2D::operator + (const Vector2D& v) const
{
    Vector2D temp;

    temp.x = x+v.x;
    temp.y = y+v.y;

    return temp;
}

Vector2D Vector2D::operator - (const Vector2D& v) const
{
    Vector2D temp;

    temp.x = x-v.x;
    temp.y = y-v.y;

    return temp;
}

float Vector2D::operator * (const Vector2D& v) const
{
    return ((x * v.x) + (y * v.y));
}

Vector3D Vector2D::operator ^ (const Vector2D& v) const
{
    float k;

    float u1 = x;
    float u2 = y;

    float v1 = v.x;
    float v2 = v.y;

    k = (u1*v2)-(u2*v1);

    return Vector3D(0, 0, k);
}

float Vector2D::length() const
{
    return sqrt((x*x)+(y*y));
}

void Vector2D::normalize()
{
    x = x/length();
    y = y/length();
}

Vector2D Vector2D::operator * (float c) const
{
    Vector2D multipliedVec;

    multipliedVec.x = x;
    multipliedVec.y = y;

    multipliedVec.x = x*c;
    multipliedVec.y = y*c;

    return multipliedVec;
}

float& Vector2D::operator[] (const int index)
{
    assert(index >= 0 && index <= 1);

    switch (index)
    {
    case 0:
        return x;
    case 1:
        return y;
    default:
        qDebug() << "ERROR, SPECIFIED VECTOR-2D INDEX IS OUT OF BOUNDS!!!";
        return y;
    }
}

Vector3D Vector2D::barycentricCoordinates(const Vector2D& p1, const Vector2D& p2, const Vector2D& p3)
{
    Vector2D p12 = p2-p1;
    Vector2D p13 = p3-p1;
    Vector3D n = p12^p13;
    float areal_123 = n.length(); // dobbelt areal

    Vector3D baryc; // til retur.

    // u
    Vector2D p = p2 - *this;
    Vector2D q = p3 - *this;
    n = p^q;
    baryc.x = n.z/areal_123;

    // v
    p = p3 - *this;
    q = p1 - *this;
    n = p^q;
    baryc.y = n.z/areal_123;

    // w
    p = p1 - *this;
    q = p2 - *this;
    n = p^q;
    baryc.z = n.z/areal_123;

    return baryc;
}


void Vector2D::debugPrint()
{
    qDebug() << x << "    " << y;
    qDebug() << "\n";
}
}
