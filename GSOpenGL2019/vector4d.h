#ifndef VECTOR4D_H
#define VECTOR4D_H

#include <QOpenGLFunctions_4_1_Core>
#include "vector3d.h"
#include <QDebug>

namespace gs2019 {

class Vector4D
{ 
public:
    Vector4D(Vector3D inputVec3D, float inputW);

    Vector4D(float inputX=0.f, float inputY=0.f, float inputZ=0.f, float inputW=0.f);

    const Vector4D& operator = (const Vector4D& v);   // Assignment
    Vector4D operator + (const Vector4D& v) const;     // Addition
    Vector4D operator - (const Vector4D& v) const;      // Subtraction
    float operator * (const Vector4D& v) const;         	// Dot product
    Vector4D operator ^ (const Vector4D& v) const;     // Cross product
    float length() const;                               		// return length
    void normalize();                                   		// Normalize to unit length
    Vector4D operator * (float c) const;

    float& operator[] (const int index);

    float x;
    float y;
    float z;
    float w;

    void debugPrint();
};
}

#endif // VECTOR4D_H
