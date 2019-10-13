#include "trianglesurface.h"
#include <iostream>

TriangleSurface::TriangleSurface(gs2019::Vector3D inTrans, gs2019::Vector3D inRot, float rotAmount, gs2019::Vector3D inScale, bool contRot, bool bounce) : VisualObject()
{
    Vertex v{};
    v.set_xyz(0,0,0); v.set_rgb(1,0,0); mVertices.push_back(v);
    v.set_xyz(0.5,0,0); v.set_rgb(0,1,0); mVertices.push_back(v);
    v.set_xyz(0.5,0.5,0); v.set_rgb(0,0,1); mVertices.push_back(v);
    v.set_xyz(0,0,0); v.set_rgb(0,1,0); mVertices.push_back(v);
    v.set_xyz(0.5,0.5,0); v.set_rgb(1,0,0); mVertices.push_back(v);
    v.set_xyz(0,0.5,0); v.set_rgb(0,0,1); mVertices.push_back(v);

    mMatrix.setToIdentity();

    //Transformasjon
    mMatrix.translate(inTrans.x, inTrans.y, inTrans.z);
    mMatrix.scale(inScale.x, inScale.y, inScale.z);

    if(!contRot)
    {
        //mMatrix.rotate(rotAmount, inRot.x, inRot.y, inRot.z);
    }

    //Set member variables
    mContRot = contRot;
    mBounce = bounce;
}

TriangleSurface::TriangleSurface(std::string objectName, std::string filename, gs2019::Vector3D inTrans, gs2019::Vector3D inRot, float rotAmount, gs2019::Vector3D inScale, bool contRot, bool bounce, bool collisionActive) : VisualObject()
{
    mName = objectName;

    readFile(filename);
    mMatrix.setToIdentity();

    //Transformasjon
    mMatrix.translate(inTrans.x, inTrans.y, inTrans.z);
    mMatrix.scale(inScale.x, inScale.y, inScale.z);

    msx = inScale.x;
    msy = inScale.y;
    msz = inScale.z;

    if(!contRot)
    {
        //mMatrix.rotate(rotAmount, inRot.x, inRot.y, inRot.z);
    }

    //Set member variables
    mContRot = contRot;
    mBounce = bounce;

    mCollisionActive = collisionActive;

    mCollisionAABB->generateAABB();
}

TriangleSurface::~TriangleSurface()
{
    glDeleteVertexArrays( 1, &mVAO );
    glDeleteBuffers( 1, &mVBO );
}

void TriangleSurface::readFile(std::string filnavn)
{
    std::ifstream inn;
    inn.open(filnavn.c_str());

    if (inn.is_open()) {
        unsigned int n;
        Vertex vertex;
        inn >> n;
        mVertices.reserve(n);
        for (unsigned int i=0; i<n; i++) {
            inn >> vertex;
            mVertices.push_back(vertex);
        }
        inn.close();
    }
}

void TriangleSurface::construct()
{
  float xmin=0.0f, xmax=1.0f, ymin=0.0f, ymax=1.0f, h=0.25f;
  for (auto x=xmin; x<xmax; x+=h)
      for (auto y=ymin; y<ymax; y+=h)
      {
          float z = sin(MathLab::PI*x)*sin(MathLab::PI*y);
          mVertices.push_back(Vertex{x,y,z,x,y,z,0,0});
          z = sin(MathLab::PI*(x+h))*sin(MathLab::PI*y);
          mVertices.push_back(Vertex{x+h,y,z,x,y,z,0,0});
          z = sin(MathLab::PI*x)*sin(MathLab::PI*(y+h));
          mVertices.push_back(Vertex{x,y+h,z,x,y,z,0,0});
          mVertices.push_back(Vertex{x,y+h,z,x,y,z,0,0});
          z = sin(MathLab::PI*(x+h))*sin(MathLab::PI*y);
          mVertices.push_back(Vertex{x+h,y,z,x,y,z,0,0});
          z = sin(MathLab::PI*(x+h))*sin(MathLab::PI*(y+h));
          mVertices.push_back(Vertex{x+h,y+h,z,x,y,z,0,0});
      }
}

void TriangleSurface::writeFile(std::string filnavn)
{
    std::ofstream ut;
    ut.open(filnavn.c_str());

    if (ut.is_open())
    {
        auto n = mVertices.size();
        Vertex vertex;
        ut << n << std::endl;
        for (auto it=mVertices.begin(); it != mVertices.end(); it++)
        {
            vertex = *it;
            ut << vertex << std::endl;
        }
        ut.close();
    }
}

void TriangleSurface::init()
{
    initializeOpenGLFunctions();

    //Vertex Array Object - VAO
    glGenVertexArrays( 1, &mVAO );
    glBindVertexArray( mVAO );

    //Vertex Buffer Object to hold vertices - VBO
    glGenBuffers( 1, &mVBO );
    glBindBuffer( GL_ARRAY_BUFFER, mVBO );

    //Vertex Buffer Object to hold vertices - VBO
    glBufferData( GL_ARRAY_BUFFER, mVertices.size()*sizeof( Vertex ), mVertices.data(), GL_STATIC_DRAW );

    //Element Buffer Object - EBO
    //glGenBuffers(1, &mEBO);
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEBO);
    //glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(mVertices) * sizeof(GLuint), mVertices.data(), GL_STATIC_DRAW);

    // 1rst attribute buffer : vertices
    glBindBuffer(GL_ARRAY_BUFFER, mVBO);
    glVertexAttribPointer(0, 3, GL_FLOAT,GL_FALSE, sizeof(Vertex), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    // 2nd attribute buffer : colors
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    // 3rd attribute buffer : uvs
    glVertexAttribPointer(2, 2,  GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)( 6 * sizeof( GLfloat ) ));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);
}

void TriangleSurface::draw()
{
    //==================== Rotation/Translation based on time ===================

    if(mBounce || mContRot)
    {
        ++time;

        if(mBounce)
        {
            mMatrix.translate(0.f, sin(time*0.1f)*0.05f, 0.f);
        }

        if(mContRot)
        {
            mMatrix.rotate(1.f, 0, 1, 0);
        }
    }

    // Update the position of the object
    mx = mMatrix[{0,3}];
    my = mMatrix[{1,3}];
    mz = mMatrix[{2,3}];

    mrx = mMatrix[{0,3}];
    mry = mMatrix[{1,3}];
    mrz = mMatrix[{2,3}];

    if (mCollisionActive && mDynamicObject && collisionTest(mDynamicObject))
    {
        //gs2019::Vector3D direction = (getPosition() - mDynamicObject->getPosition());


        //if (mDynamicObject->getPosition().y > getPosition().y || mDynamicObject->getPosition().y < getPosition().y)
        //    direction = gs2019::Vector3D(0.f, 1.f, 0.f);
        //
        //if (mDynamicObject->getPosition().z > getPosition().z || mDynamicObject->getPosition().z < getPosition().z)
        //    direction = gs2019::Vector3D(0.f, 0.f, 1.f);

        mDynamicObject->collisionResponse(findColDir());
    }

    glUseProgram(mMaterial.mShader->getProgram());
    glBindVertexArray(mVAO);
    mMaterial.mShader->transmitUniformData(&mMatrix, &mMaterial);
    glDrawArrays(GL_TRIANGLES, 0, signed(mVertices.size()));
}

// Returns what direction the colliding object should move
gs2019::Vector3D TriangleSurface::findColDir()
{
    gs2019::Vector3D returnVec(0.f, 0.f, 0.f);

    if ((mDynamicObject->getPosition().x + mDynamicObject->radius) > mCollisionAABB->minXYZ.x)
        returnVec = returnVec + gs2019::Vector3D(1.f, 0.f, 0.f);

    if ((mDynamicObject->getPosition().x + mDynamicObject->radius) < mCollisionAABB->maxXYZ.x)
        returnVec = returnVec + gs2019::Vector3D(-1.f, 0.f, 0.f);

    if ((mDynamicObject->getPosition().z + mDynamicObject->radius) > mCollisionAABB->minXYZ.z)
        returnVec = returnVec + gs2019::Vector3D(0.f, 0.f, 1.f);

    if ((mDynamicObject->getPosition().z + mDynamicObject->radius) < mCollisionAABB->maxXYZ.z)
        returnVec = returnVec + gs2019::Vector3D(0.f, 0.f, -1.f);

    return returnVec;
}

// ========= MOVEMENT =========
void TriangleSurface::move(float x, float y, float z, gs2019::Vector3D surfNormal)
{
    mMatrix.translate(x, y, z);
    mx = mMatrix[{0,3}];
    my = mMatrix[{1,3}];
    mz = mMatrix[{2,3}];
}

void TriangleSurface::rotate(float amount, int x, int y, int z)
{
    mMatrix.rotate(amount, x, y, z);
    mrx += x;
    mry += y;
    mrz += z;
}

void TriangleSurface::setLocation(float inX, float inY, float inZ)
{
    mMatrix[{0,3}] = inX;
    mMatrix[{1,3}] = inY;
    mMatrix[{2,3}] = inZ;
}

void TriangleSurface::setDynamicObject(VisualObject *dynamicObject)
{
    mDynamicObject = dynamicObject;
}

gs2019::Vector3D TriangleSurface::getPosition()
{
    return gs2019::Vector3D(mx, my, mz);
}

gs2019::Vector3D TriangleSurface::getRotation()
{
    return gs2019::Vector3D(mrx, mry, mrz);
}

gs2019::Vector3D TriangleSurface::getScale()
{
    return gs2019::Vector3D(msx, msy, msz);
}

bool TriangleSurface::collisionTest(VisualObject* inputObject)
{
    return mCollisionAABB->checkObjectOverlap(inputObject);
}
