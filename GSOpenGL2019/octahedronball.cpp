#include <iostream>
#include <sstream>
#include "octahedronball.h"


//! \param n - the recursion level (default is 0 which makes the original Octahedron)
//!
//! The number of (duplicated) vertices are calculated based on the parameter n - the recursion level.
//! Initially we have 8 triangles, each with 3 vertices.
//! A subdivision consists of
//! - compute the midpoint of each edge in a triangel
//! - add these midpoint as vertices
//! - make three new edges by connecting new vertices
//! - preserve orientation
//! - move the three new points on the unit ball's surface by normalizing. Remember the vertex vector
//! is also a normal to the ball's surface.
//! The formula for the number of vertices is
//! m_nVertices = 3 * 8 * 4^n
//! where
//! - 3 comes from the number of vertices in a triangle,
//! - 8 comes from the original number of triangles in a regular Octahedron
//! - n is the recursion level (number of repeated subdivisions)
//!

OctahedronBall::OctahedronBall(std::string objectName, int n, gs2019::Vector3D inTrans, gs2019::Vector3D inRot, float rotAmount, gs2019::Vector3D inScale, bool contRot, bool bounce) : m_rekursjoner(n), m_indeks(0), VisualObject()
{
    mName = objectName;

    mVertices.reserve(3 * 8 * pow(4, m_rekursjoner));
    oktaederUnitBall();

    mMatrix.setToIdentity();
    mTranslationMatrix.setToIdentity();
    mRotationMatrix.setToIdentity();
    mScaleMatrix.setToIdentity();

    //Set member variables
    mContRot = contRot;
    mBounce = bounce;

    radius = ((inScale.x + inScale.y + inScale.z) / 3);

    //Transformasjon
    mTranslationMatrix.translate(inTrans.x, inTrans.y, inTrans.z);
    //mRotationMatrix.rotate(rotAmount, inRot.x, inRot.y, inRot.z);
    mScaleMatrix.scale(inScale.x, inScale.y, inScale.z);

    if(!contRot)
    {
        mMatrix.rotate(rotAmount, inRot.x, inRot.y, inRot.z);
    }

    // ====== Phsics ======
    if (simulatePhysics)
    {
        physicTimer.start();
    }

    updateMatrix();
}

//!//! \brief OctahedronBall::~OctahedronBall() virtual destructor
//!
OctahedronBall::~OctahedronBall()
{
    glDeleteVertexArrays( 1, &mVAO );
    glDeleteBuffers( 1, &mVBO );
}

// Parametre inn: xyz koordinatene til et triangle v1, v2, v3 ccw
// Bevarer orienteringen av hjørnene
//!
//! \brief OctahedronBall::lagTriangel()
//! \param v1 - position on the unit ball for vertex 1
//! \param v2 - position on the unit ball for vertex 2
//! \param v3 - position on the unit ball for vertex 3
//!
//! lagTriangel() creates vertex data for a triangle's 3 vertices. This is done in the
//! final step of recursion.
//!
void OctahedronBall::lagTriangel(const Vec3& v1, const Vec3& v2, const Vec3& v3)
{
    Vertex v{};

    // Updating radius.
    radius = (((v1.x + v2.x + v3.x) / 3) +
              ((v1.y + v2.y + v3.y) / 3) +
              ((v1.z + v2.z + v3.z) / 3)) / 3;

    v.set_xyz(v1.x, v1.y, v1.z);		// koordinater v.x = v1.x, v.y=v1.y, v.z=v1.z
    v.set_rgb(v1.x, v1.y, v1.z);	// rgb
    v.set_uv(0.0f, 0.0f);			// kan utelates
    mVertices.push_back(v);
    v.set_xyz(v2.x, v2.y, v2.z);
    v.set_rgb(v2.x, v2.y, v2.z);
    v.set_uv(1.0f, 0.0f);
    mVertices.push_back(v);
    v.set_xyz(v3.x, v3.y, v3.z);
    v.set_rgb(v3.x, v3.y, v3.z);
    v.set_uv(0.5f, 1.0f);
    mVertices.push_back(v);
}

// Rekursiv subdivisjon av triangel
//!
//! \brief OctahedronBall::subDivide() recursive subdivision of a triangel
//! \param a coordinates for vertex a
//! \param b coordinates for vertex b
//! \param c coordinates for vertex c
//! \param n level of recursion
//!
//! The function tests
//! (if n>0)
//! - three new edge midpoints are computed and normalized,
//! - four new calls to subDivide() is done - one for each new triangle, preserving orientation
//! and with decreased parameter n
//! else
//! - call lagTriangel(a, b, c)
//!
void OctahedronBall::subDivide(const Vec3& a, const Vec3& b, const Vec3& c, int n)
{
    if (n>0) {
        Vec3 v1 = a+b; v1.normalize();
        Vec3 v2 = a+c; v2.normalize();
        Vec3 v3 = c+b; v3.normalize();
        subDivide(a, v1, v2, n-1);
        subDivide(c, v2, v3, n-1);
        subDivide(b, v3, v1, n-1);
        subDivide(v3, v2, v1, n-1);

        //qDebug() << "v1: " << v1.x << v1.y << v1.z;
        //qDebug() << "v2: " << v2.x << v2.y << v2.z;
        //qDebug() << "v3: " << v3.x << v3.y << v3.z;

    } else {
        lagTriangel(a, b, c);
    }
}

//!
//! \brief OctahedronBall::oktaederUnitBall() creates 8 unit ball vertices and call subDivide()
//!
//! If the parameter n of the constructor OctahedronBall() is zero, the result will be the
//! original octahedron consisting of 8 triangles with duplicated vertices.
//!
void OctahedronBall::oktaederUnitBall()
{
    Vec3 v0{0, 0, 1};
    Vec3 v1{1, 0, 0};
    Vec3 v2{0, 1, 0};
    Vec3 v3{-1, 0, 0};
    Vec3 v4{0, -1, 0};
    Vec3 v5{0, 0, -1};

    subDivide(v0, v1, v2, m_rekursjoner);
    subDivide(v0, v2, v3, m_rekursjoner);
    subDivide(v0, v3, v4, m_rekursjoner);
    subDivide(v0, v4, v1, m_rekursjoner);
    subDivide(v5, v2, v1, m_rekursjoner);
    subDivide(v5, v3, v2, m_rekursjoner);
    subDivide(v5, v4, v3, m_rekursjoner);
    subDivide(v5, v1, v4, m_rekursjoner);
}

//!
//! \brief OctahedronBall::initVertexBufferObjects() calls glGenBuffers(), glBindBuffer() and glBufferdata()
//! for using later use of glDrawArrays()
//!
void OctahedronBall::init()
{
    initializeOpenGLFunctions();

    //Vertex Array Object - VAO
    glGenVertexArrays( 1, &mVAO );
    glBindVertexArray( mVAO );

    //Vertex Buffer Object to hold vertices - VBO
    glGenBuffers( 1, &mVBO );
    glBindBuffer( GL_ARRAY_BUFFER, mVBO );

    glBufferData( GL_ARRAY_BUFFER, mVertices.size()*sizeof(Vertex), mVertices.data(), GL_STATIC_DRAW );

    glBindBuffer(GL_ARRAY_BUFFER, mVBO);
    glVertexAttribPointer(0, 3, GL_FLOAT,GL_FALSE,sizeof(Vertex), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,  sizeof(Vertex),  (GLvoid*)(3 * sizeof(GLfloat)) );
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
}

//!
//! \brief OctahedronBall::draw() draws a ball using glDrawArrays()
//! \param positionAttribute    vertex shader variable for position
//! \param normalAttribute      vertex shader variable for normal or color
//! \param textureAttribute     vertex shader variable for texture coordinates (optional)
//!
//! draw()
//! - glBindBuffer()
//! - glVertexAttribPointer()
//! - glBindTexture()
//! - glVertexAttribPointer()
//! - glDrawArrays() with GL_TRIANGLES
//!
void OctahedronBall::draw()
{
    //==================== Simulate velocity and friction ===================
    if (simulateVelocity)
    {
        rotate((vz*(180/MathLab::PI)/radius), 1, 0, 0);
        rotate(-(vx*(180/MathLab::PI)/radius), 0, 0, 1);

        vx *= friction;
        vz *= friction;

        if (vy > 0.f)
        {
            vy *= 0.8f;
        }

        mTranslationMatrix.translate(0.f, vy, 0.f);
    }

    if (vx > 0.f)
        mTranslationMatrix.translate(vx, 0.f, 0.f);

    if (vx < 0.f)
        mTranslationMatrix.translate(vx, 0.f, 0.f);

    if (vz > 0.f)
        mTranslationMatrix.translate(0.f, 0.f, vz);

    if (vz < 0.f)
        mTranslationMatrix.translate(0.f, 0.f, vz);

    // FJERN DETTE HVIS TING FUNGERER:
    //==================================
    //        if ((maxX-radius >= mx) && (vx > 0.f))
    //            mTranslationMatrix.translate(vx, 0.f, 0.f);

    //        if ((minX+radius <= mx) && (vx < 0.f))
    //            mTranslationMatrix.translate(vx, 0.f, 0.f);

    //        if ((maxZ-radius >= mz) && (vz > 0.f))
    //            mTranslationMatrix.translate(0.f, 0.f, vz);

    //        if ((minZ+radius <= mz) && (vz < 0.f))
    //            mTranslationMatrix.translate(0.f, 0.f, vz);
    //==================================

    updateMatrix();

    //==================== Rotation/Translation based on time ===================
    if (mBounce || mContRot)
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

    if (simulatePhysics)
    {
        // Sets up delta-time.
        deltaTime = (physicTimer.elapsed() / 1000.f) - lastTime;
        lastTime = physicTimer.elapsed() / 1000.f;

        // Calculates the normal of the ground
        Vec3 Normal = (VertexA - VertexB)^(VertexC - VertexB);
        Normal.normalize();
        //Normal.debugPrint();

        if (mTranslationMatrix[{1,3}] + 0.3f > groundHeight)
        {
            // Fix velocity - It's not physically correct. Page 6 & 7 - Ballsimulering.pdf
            velocity = velocity + (Normal * deltaTime * 0.2f);

            mTranslationMatrix[{0,3}] = mTranslationMatrix[{0,3}] + (velocity.x);
            mTranslationMatrix[{2,3}] = mTranslationMatrix[{2,3}] + (velocity.z);
        }

        // Downwards velocity.
        velocity = velocity + (Gravity * deltaTime);
        mTranslationMatrix[{1,3}] = mTranslationMatrix[{1,3}] + velocity.y;

        // Makes sure the ball doesn't clip through the ground.
        if (mTranslationMatrix[{1,3}] < groundHeight)
        {
            mTranslationMatrix[{1,3}] = groundHeight;
            //velocity.y = 0;
        }

        updateMatrix();
    }

    glUseProgram(mMaterial.mShader->getProgram());
    glBindVertexArray(mVAO);
    mMaterial.mShader->transmitUniformData(&mMatrix, &mMaterial);

    glDrawArrays(GL_TRIANGLES, 0, mVertices.size());
}

void OctahedronBall::resetPhysics()
{
    physicTimer.restart();
    lastTime = 0.f;
    deltaTime = 0.f;

    VertexA = {0.f, 0.f, 0.f};
    VertexB = {0.f, 0.f, 0.f};
    VertexC = {0.f, 0.f, 0.f};

    mVelocity = Vec3(0.f, 0.f, 0.f);
}

// ========= MOVEMENT =========
void OctahedronBall::move(float x, float y, float z, gs2019::Vector3D surfNormal)
{
    // Finds the velocity.
    mVelocity.x = x;
    mVelocity.y = y;
    mVelocity.z = z;

    if (simulateVelocity)
    {
        vx += x;
        vz += z;

        // If the object is on the ground it will be able to jump
        if (!(my > groundHeight + radius))
        {
            vy += y;
        }
    }

    // Finds rotation direction.
    Vec3 rotation = surfNormal^mVelocity;
    rotation.normalize();

    updateMatrix();
}

void OctahedronBall::rotate(float amount, int x, int y, int z)
{
    mRotationMatrix.rotate(amount, x, y, z);

    mrx += x;
    mry += y;
    mrz += z;
}

void OctahedronBall::setHeight(float inY)
{
    groundHeight = inY;
    //mTranslationMatrix[{1,3}] = inY;
    //updateMatrix();
}

void OctahedronBall::setLocation(float inX, float inY, float inZ)
{
    mTranslationMatrix[{0,3}] = inX;
    mTranslationMatrix[{1,3}] = inY;
    mTranslationMatrix[{2,3}] = inZ;

    mx = mMatrix[{0,3}];
    my = mMatrix[{1,3}];
    mz = mMatrix[{2,3}];

    updateMatrix();
}

void OctahedronBall::collisionResponse(Vec3 input)
{
    // TO FIX:
    // The ball doesn't bounce right all the time.
    // The ball can also be forced through objects.

    if (wallBounce)
    {
        if ((input.x > 0.f) && (vx > 0.f))
            vx *= -1;

        if ((input.x < 0.f) && (vx < 0.f))
            vx *= -1;

        if ((input.z > 0.f) && (vz > 0.f))
            vz *= -1;

        if ((input.z < 0.f) && (vz < 0.f))
            vz *= -1;
    }
}

void OctahedronBall::stopVelocity()
{
    vx = 0.f;
    vy = 0.f;
    vz = 0.f;
}

gs2019::Vector3D OctahedronBall::getPosition()
{
    return gs2019::Vector3D(mx, my, mz);
}

gs2019::Vector3D OctahedronBall::getRotation()
{
    return gs2019::Vector3D(mrx, mry, mrz);
}

float OctahedronBall::getRadius()
{
    return radius;
}

void OctahedronBall::updateMatrix()
{
    // Multiplies the transformation matricies in the right order.
    mMatrix = mTranslationMatrix * mRotationMatrix * mScaleMatrix;

    mx = mTranslationMatrix[{0,3}];
    my = mTranslationMatrix[{1,3}];
    mz = mTranslationMatrix[{2,3}];

    //mrx = mRotationMatrix[{0,3}];
    //mry = mRotationMatrix[{1,3}];
    //mrz = mRotationMatrix[{2,3}];
}
