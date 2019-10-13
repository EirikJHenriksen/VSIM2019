#ifndef OBJECT_H
#define OBJECT_H

#include <QOpenGLFunctions_4_1_Core>
#include <QOpenGLTexture>
#include <iostream>
#include <QMatrix4x4>
#include <vector>
#include <cmath>
#include "mathlab.h"
#include "stb_image.h"
#include "shader.h"
#include "material.h"
#include "vertex.h"

class VisualObject : protected QOpenGLFunctions_4_1_Core
{
private:

public:
    VisualObject();
    ~VisualObject();

    virtual void init()=0;
    virtual void draw()=0;

    virtual void move(float x, float y, float z, gs2019::Vector3D surfNormal){}
    virtual void rotate(float amount, int x, int y, int z){}
    virtual void setLocation(float inX, float inY, float inZ){}
    virtual void setHeight(float inY){}

    virtual gs2019::Vector3D getPosition(){}
    virtual gs2019::Vector3D getRotation(){}
    virtual gs2019::Vector3D getScale(){}

    virtual bool collisionTest(VisualObject* inputObject){}
    virtual void collisionResponse(gs2019::Vector3D input){}

    virtual float getRadius(){}
    virtual void updateMatrix(){}

    // World space matrix
    gs2019::Matrix4x4 mMatrix;

    void setShader(Shader *shader);

    std::vector<Vertex> get_mVertecies();

    float radius = 0.5f;

    bool mContRot;
    bool mBounce;

    std::string mName;

    Material mMaterial;

    virtual void setDynamicObject(VisualObject* dynamicObject){}

    // Replace this with a vector that can hold objects that can collide.
    VisualObject* mDynamicObject = nullptr;

protected:
    std::vector<Vertex> mVertices;
    std::vector<GLuint> mIndices;

    GLuint mVAO{0};
    GLuint mVBO{0};
    GLuint mEAB{0};
    GLint mMatrixUniform{0};

    // Matrix for the different transformations that can be done to the object
    gs2019::Matrix4x4 mTranslationMatrix;
    gs2019::Matrix4x4 mRotationMatrix;
    gs2019::Matrix4x4 mScaleMatrix;

    int time = 0;
};

#endif // OBJECT_H
