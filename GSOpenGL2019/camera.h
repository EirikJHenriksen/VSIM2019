#ifndef CAMERA_H
#define CAMERA_H

#include <QOpenGLFunctions_4_1_Core>
#include <QMatrix4x4>
#include "mathlab.h"

class Camera : protected QOpenGLFunctions_4_1_Core
{
public:
    Camera(gs2019::Vector3D inPosition);
    ~Camera();

    QMatrix4x4 mVMatrix;
    QMatrix4x4 mPMatrix;

    gs2019::Vector3D position, at;

    QVector3D globalUp = QVector3D(0.f, 1.f, 0.f);
    gs2019::Vector3D mUp{0.f, 1.f, 0.f};
    gs2019::Vector3D mRight{-1.f, 0.f, 0.f};

    GLint mPMatrixUniform{0};
    GLint mVMatrixUniform{0};

    void lookAt(gs2019::Vector3D inAt);
    void perspective(float verticalAngle, float aspectRatio, float nearPlane, float farPlane);

    //void draw();
};

#endif // CAMERA_H
