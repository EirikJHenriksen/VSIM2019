#include "matrix4x4.h"
#include "mathlab.h"
#include <cmath>
#include <QDebug>

namespace gs2019
{
Matrix4x4::Matrix4x4(float inX1, float inY1, float inZ1, float inW1,
                               float inX2, float inY2, float inZ2, float inW2,
                               float inX3, float inY3, float inZ3, float inW3,
                               float inX4, float inY4, float inZ4, float inW4)
{
    A[0][0] = inX1;
    A[0][1] = inY1;
    A[0][2] = inZ1;
    A[0][3] = inW1;

    A[1][0] = inX2;
    A[1][1] = inY2;
    A[1][2] = inZ2;
    A[1][3] = inW2;

    A[2][0] = inX3;
    A[2][1] = inY3;
    A[2][2] = inZ3;
    A[2][3] = inW3;

    A[3][0] = inX4;
    A[3][1] = inY4;
    A[3][2] = inZ4;
    A[3][3] = inW4;
}

Matrix4x4::Matrix4x4(Vector4D row1, Vector4D row2, Vector4D row3, Vector4D row4)
{
    A[0][0] = row1.x;
    A[0][1] = row1.y;
    A[0][2] = row1.z;
    A[0][3] = row1.w;

    A[1][0] = row2.x;
    A[1][1] = row2.y;
    A[1][2] = row2.z;
    A[1][3] = row2.w;

    A[2][0] = row3.x;
    A[2][1] = row3.y;
    A[2][2] = row3.z;
    A[2][3] = row3.w;

    A[3][0] = row4.x;
    A[3][1] = row4.y;
    A[3][2] = row4.z;
    A[3][3] = row4.w;
}

void Matrix4x4::setToIdentity()
{
    A[0][0] = 1;
    A[0][1] = 0;
    A[0][2] = 0;
    A[0][3] = 0;

    A[1][0] = 0;
    A[1][1] = 1;
    A[1][2] = 0;
    A[1][3] = 0;

    A[2][0] = 0;
    A[2][1] = 0;
    A[2][2] = 1;
    A[2][3] = 0;

    A[3][0] = 0;
    A[3][1] = 0;
    A[3][2] = 0;
    A[3][3] = 1;
}

float Matrix4x4::calculateDeterminant()
{
    Vector3D helpVector1(A[1][1], A[1][2], A[1][3]);
    Vector3D helpVector2(A[2][1], A[2][2], A[2][3]);
    Vector3D helpVector3(A[3][1], A[3][2], A[3][3]);

    Vector3D helpVector4(A[1][0], A[1][2], A[1][3]);
    Vector3D helpVector5(A[2][0], A[2][2], A[2][3]);
    Vector3D helpVector6(A[3][0], A[3][2], A[3][3]);

    Vector3D helpVector7(A[1][0], A[1][1], A[1][3]);
    Vector3D helpVector8(A[2][0], A[2][1], A[2][3]);
    Vector3D helpVector9(A[3][0], A[3][1], A[3][3]);

    Vector3D helpVector10(A[1][0], A[1][1], A[1][2]);
    Vector3D helpVector11(A[2][0], A[2][1], A[2][2]);
    Vector3D helpVector12(A[3][0], A[3][1], A[3][2]);

    return ( (A[0][0]*calculateDeterminantHelper(helpVector1, helpVector2, helpVector3) )-( A[0][1]*calculateDeterminantHelper(helpVector4, helpVector5, helpVector6) )
            +( A[0][2]*calculateDeterminantHelper(helpVector7, helpVector8, helpVector9) )-( A[0][3]*calculateDeterminantHelper(helpVector10, helpVector11, helpVector12)) );
}

float Matrix4x4::calculateDeterminantHelper(Vector3D row1, Vector3D row2, Vector3D row3)
{
    return ( row1.x*((row2.y*row3.z) - (row3.y*row2.z)) )-( row1.y*((row2.x*row3.z) - (row3.x*row2.z)) )+( row1.z*((row2.x*row3.y) - (row3.x*row2.y)) );
}


void Matrix4x4::pivot(int row)
{
    float temp1 = A[row][0];
    float temp2 = A[row][1];
    float temp3 = A[row][2];
    float temp4 = A[row][3];

    A[row][0] = A[row+1][0];
    A[row][1] = A[row+1][1];
    A[row][2] = A[row+1][2];
    A[row][3] = A[row+1][3];

    A[row+1][0] = temp1;
    A[row+1][1] = temp2;
    A[row+1][2] = temp3;
    A[row+1][3] = temp4;
}

void Matrix4x4::LU()
{
    for (int k=0; k<m-1; k++) {
        //pivot(k); <-- Sett inn senere!

        // Ved radoperasjoner skal vi oppnå 0-ere under diagonalelementet
        // i alle rader nedenfor (altså kolonne k
        // Vi subtraherer et multiplum av k-te
        // rad fra radene nedenfor, nuller ut kolonner fra venstre
        for (int i=k+1; i<m; i++)
        {
            // Skal mult med denne og deretter trekke fra rad i
            // denne skal bli null og vi kan lagre faktoren her
            A[i][k] = A[i][k]/A[k][k];
            for (int j=k+1; j<n; j++)
            {
                // kolonnene til høyre for den som blir nullet ut
                A[i][j] = A[i][j] - A[i][k]*A[k][j];
            }
        }
    }
}

Vector4D Matrix4x4::solve(Vector4D &b) const
{
    Vector4D x;
    for (int k=0; k<m; k++)
        for (int i=k+1; i<n; i++)
            b[i] = b[i]-A[i][k]*b[k];
    for (int i=n-1; i>=0; i--) {
        x[i] = b[i];
        for (int j=i+1; j<n; j++)
            x[i] = x[i] - A[i][j]*x[j];
        x[i] = x[i]/A[i][i];
    }
    return x;
}


void Matrix4x4::operator* (const float multiplier)
{
    for(auto a = 0; a <= 3; ++a)
    {
        for(auto b = 0; b <= 3; ++b)
        {
            A[a][b] *= multiplier;
        }
    }
}

Vector4D Matrix4x4::operator* (const Vector4D multiplier)
{
    Vector4D result;

    A[0][0] *= multiplier.x;
    A[0][1] *= multiplier.y;
    A[0][2] *= multiplier.z;
    A[0][3] *= multiplier.w;

    result[0] = A[0][0] + A[0][1] + A[0][2] + A[0][3];

    A[1][0] *= multiplier.x;
    A[1][1] *= multiplier.y;
    A[1][2] *= multiplier.z;
    A[1][3] *= multiplier.w;

    result[1] = A[1][0] + A[1][1] + A[1][2] + A[1][3];

    A[2][0] *= multiplier.x;
    A[2][1] *= multiplier.y;
    A[2][2] *= multiplier.z;
    A[2][3] *= multiplier.w;

    result[2] = A[2][0] + A[2][1] + A[2][2] + A[2][3];

    A[3][0] *= multiplier.x;
    A[3][1] *= multiplier.y;
    A[3][2] *= multiplier.z;
    A[3][3] *= multiplier.w;

    result[3] = A[3][0] + A[3][1] + A[3][2] + A[3][3];

    return result;
}

Matrix4x4 Matrix4x4::operator* (const Matrix4x4 multiplier)
{
    Matrix4x4 resultat;

    for(auto a=0; a <= 3; ++a)
    {
        for(auto b=0; b <= 3; ++b)
        {
            for(auto c=0; c <= 3; ++c)
            {
                resultat.A[a][b] += (A[a][c] * multiplier.A[c][b]);
            }
        }
    }

    return resultat;
}

void Matrix4x4::scale(float inX, float inY, float inZ)
{
    Matrix4x4 scaleMatrix(Vector4D(inX, 0, 0, 0), Vector4D(0, inY, 0, 0), Vector4D(0, 0, inZ, 0), Vector4D(0, 0, 0, 1));

    *this = *this * scaleMatrix;
}

void Matrix4x4::rotate(float amount, int inX, int inY, int inZ)
{
    double rotation = amount*(MathLab::PI/180);

    Matrix4x4 rotateMatrix;

    if (inZ)
    {
        rotateMatrix =  Matrix4x4(Vector4D((cos(rotation)), -sin(rotation), 0, 0), Vector4D(sin(rotation), cos(rotation), 0, 0), Vector4D(0, 0, 1, 0), Vector4D(0, 0, 0, 1));
    }

    if (inY)
    {
        rotateMatrix =  Matrix4x4(Vector4D((cos(rotation)), 0, sin(rotation), 0), Vector4D(0, 1, 0, 0), Vector4D(-sin(rotation), 0, cos(rotation), 0), Vector4D(0, 0, 0, 1));
    }

    if (inX)
    {
        rotateMatrix =  Matrix4x4(Vector4D(1, 0, 0, 0), Vector4D(0, cos(rotation), -sin(rotation), 0), Vector4D(0, sin(rotation), cos(rotation), 0), Vector4D(0, 0, 0, 1));
    }

    *this = *this * rotateMatrix;
}

void Matrix4x4::translate(float inX, float inY, float inZ)
{
    Matrix4x4 translateMatrix (Vector4D(1, 0, 0, inX), Vector4D(0, 1, 0, inY), Vector4D(0, 0, 1, inZ), Vector4D(0, 0, 0, 1));

    *this = *this * translateMatrix;
}

float& Matrix4x4::operator[] (const std::pair<int, int>& index)
{
    return A[index.first][index.second];
}

GLfloat* Matrix4x4::constData()
{
    return *A;
}

void Matrix4x4::debugPrint()
{
    qDebug() << A[0][0] << "    " << A[0][1] << "   " << A[0][2] << "   " << A[0][3];

    qDebug() << A[1][0] << "    " << A[1][1] << "   " << A[1][2] << "   " << A[1][3];

    qDebug() << A[2][0] << "    " << A[2][1] << "   " << A[2][2] << "   " << A[2][3];

    qDebug() << A[3][0] << "    " << A[3][1] << "   " << A[3][2] << "   " << A[3][3];
    qDebug() << "\n";
}
}
