#ifndef VECTOR2D_H
#define VECTOR2D_H

#include <QOpenGLFunctions_4_1_Core>
#include "vector3d.h"
#include <QDebug>

namespace gs2019
{

class Vector2D
{
public:
    Vector2D(float a = 0.f, float b = 0.f);  	// Constructor

    const Vector2D& operator = (const Vector2D& v);   // Assignment
    Vector2D operator + (const Vector2D& v) const;     // Addition
    Vector2D operator - (const Vector2D& v) const;      // Subtraction
    float operator * (const Vector2D& v) const;         	// Dot product
    Vector3D operator ^ (const Vector2D& v) const;     // Cross product
    float length() const;                               		// return length
    void normalize();                                   		// Normalize to unit length
    Vector2D operator * (float c) const;

    float& operator[] (const int index);

    Vector3D barycentricCoordinates(const Vector2D& p1, const Vector2D& p2, const Vector2D& p3);

    float x{0.f};
    float y{0.f};

    void debugPrint();
};
}

#endif // VECTOR2D_H
