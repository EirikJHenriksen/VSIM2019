#ifndef VECTOR3D_H
#define VECTOR3D_H

#include <QOpenGLFunctions_4_1_Core>
#include <QDebug>

namespace gs2019
{

class Vector3D
{
public:
    Vector3D(float a = 0.0f, float b = 0.0f, float c = 0.0f);  	// Constructor

    const Vector3D& operator = (const Vector3D& v);   // Assignment
    Vector3D operator + (const Vector3D& v) const;     // Addition
    Vector3D operator - (const Vector3D& v) const;      // Subtraction
    float operator * (const Vector3D& v) const;         	// Dot product
    Vector3D operator ^ (const Vector3D& v) const;     // Cross product
    double length() const;                               		// return length
    void normalize();                                   		// Normalize to unit length
    Vector3D operator * (float c) const;
    bool operator < (const Vector3D &v) const;
    bool operator > (const Vector3D &v) const;

    float& operator[] (const int index);

    float x;
    float y;
    float z;

    void debugPrint();
};

}
#endif // VECTOR3D_H
