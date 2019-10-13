#include "camera.h"

Camera::Camera(gs2019::Vector3D inPosition)
{
    position.x = inPosition.x;
    position.y = inPosition.y;
    position.z = inPosition.z;

    mPMatrix.setToIdentity();
    mVMatrix.setToIdentity();
}

Camera::~Camera()
{

}

void Camera::lookAt(gs2019::Vector3D inAt)
{
    mVMatrix.setToIdentity();

    QVector3D qLocation(position.x, position.y, position.z);
    QVector3D qAt(inAt.x, inAt.y, inAt.z);

    mVMatrix.lookAt(qLocation, qAt, globalUp);

    // Updates the up and right vector based on what way the camera is turned, must be updated in other places where the camera is moved.
    gs2019::Vector3D lookVector = position + inAt;

    mRight = lookVector^gs2019::Vector3D(0.f, 1.f, 0.f);
    mUp = mRight^lookVector;

    mRight.normalize();
    mUp.normalize();

    //mVMatrix.transposed();
}

void Camera::perspective(float inVerticalAngle, float inAspectRatio, float inNearPlane, float inFarPlane)
{
    mPMatrix.setToIdentity();

    mPMatrix.perspective(inVerticalAngle, inAspectRatio, inNearPlane, inFarPlane);
    mPMatrix.transposed();
}

//void Camera::init(Shader* mShaderProgram)
//{
//    initializeOpenGLFunctions();

//    mPMatrixUniform = glGetUniformLocation( mShaderProgram->getProgram(), "pmatrix" );
//    mVMatrixUniform = glGetUniformLocation( mShaderProgram->getProgram(), "vmatrix" );
//}

//void Camera::draw()
//{
//    //initializeOpenGLFunctions();
//
//    //glUniformMatrix4fv( mPMatrixUniform, 1, GL_FALSE, mPMatrix.constData());
//    //glUniformMatrix4fv( mVMatrixUniform, 1, GL_FALSE, mVMatrix.constData());
//}
