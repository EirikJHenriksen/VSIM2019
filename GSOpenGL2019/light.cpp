#include "light.h"
#include "renderwindow.h"

Light::Light(gs2019::Vector3D inTrans)
{
    mVertices.insert( mVertices.end(),
    {//Vertex data - normals not correct
     Vertex{gs2019::Vector3D(-0.5f, -0.5f,  0.5f),    gs2019::Vector3D(0.f, 0.f, 1.0f),  gs2019::Vector2D(0.f,  0.f)},   //Left low
     Vertex{gs2019::Vector3D( 0.5f, -0.5f,  0.5f),    gs2019::Vector3D(0.f, 0.f, 1.0f),  gs2019::Vector2D(1.f,  0.f)},   //Right low
     Vertex{gs2019::Vector3D( 0.0f,  0.5f,  0.0f),    gs2019::Vector3D(0.f, 0.f, 1.0f),  gs2019::Vector2D(0.5f, 0.5f)},  //Top
     Vertex{gs2019::Vector3D( 0.0f, -0.5f, -0.5f),    gs2019::Vector3D(0.f, 0.f, 1.0f),  gs2019::Vector2D(0.5f, 0.5f)}   //Back low
                      });

    mIndices.insert( mIndices.end(),
    { 0, 1, 2,
      1, 3, 2,
      3, 0, 2,
      0, 3, 1
                     });

    mMatrix.setToIdentity();

    // Transforms
    mMatrix.translate(inTrans.x, inTrans.y, inTrans.z);
}

void Light::init()
{
    initializeOpenGLFunctions();

    //Vertex Array Object - VAO
    glGenVertexArrays( 1, &mVAO );
    glBindVertexArray( mVAO );

    //Vertex Buffer Object to hold vertices - VBO
    glGenBuffers( 1, &mVBO );
    glBindBuffer( GL_ARRAY_BUFFER, mVBO );

    glBufferData( GL_ARRAY_BUFFER, mVertices.size()*sizeof(Vertex), mVertices.data(), GL_STATIC_DRAW );

    // 1rst attribute buffer : vertices
    glBindBuffer(GL_ARRAY_BUFFER, mVBO);
    glVertexAttribPointer(0, 3, GL_FLOAT,GL_FALSE, sizeof(Vertex), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    // 2nd attribute buffer : colors
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,  sizeof(Vertex),  (GLvoid*)(3 * sizeof(GLfloat)) );
    glEnableVertexAttribArray(1);

    // 3rd attribute buffer : uvs
    glVertexAttribPointer(2, 2,  GL_FLOAT, GL_FALSE, sizeof( Vertex ), (GLvoid*)( 6 * sizeof( GLfloat ) ));
    glEnableVertexAttribArray(2);

    //Second buffer - holds the indices (Element Array Buffer - EAB):
    glGenBuffers(1, &mEAB);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEAB);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mIndices.size() * sizeof(GLuint), mIndices.data(), GL_STATIC_DRAW);

    glBindVertexArray(0);
}

void Light::draw()
{
    glUseProgram(mMaterial.mShader->getProgram());
    glBindVertexArray( mVAO );
    mMaterial.mShader->transmitUniformData(&mMatrix, &mMaterial);
    glDrawElements(GL_TRIANGLES, mIndices.size(), GL_UNSIGNED_INT, nullptr);
}

gs2019::Vector3D Light::getPosition()
{
    return gs2019::Vector3D(mMatrix[{0,3}], mMatrix[{1,3}], mMatrix[{2,3}]);
}
