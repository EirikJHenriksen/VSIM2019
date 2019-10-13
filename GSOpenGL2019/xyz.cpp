#include "xyz.h"

XYZ::XYZ()
{
    // UV
    gs2019::Vector2D uv(0, 0);

    // Origin
    gs2019::Vector3D vert0(0, 0, 0);

    // Vertices
    gs2019::Vector3D vertX(1, 0, 0);
    gs2019::Vector3D vertY(0, 1, 0);
    gs2019::Vector3D vertZ(0, 0, 1);

    mVertices.push_back(Vertex{vert0, vertX, uv});
    mVertices.push_back(Vertex{vertX, vertX, uv});
    mVertices.push_back(Vertex{vert0, vertY, uv});
    mVertices.push_back(Vertex{vertY, vertY, uv});
    mVertices.push_back(Vertex{vert0, vertZ, uv});
    mVertices.push_back(Vertex{vertZ, vertZ, uv});

    mMatrix.setToIdentity();
}

XYZ::~XYZ()
{
    glDeleteVertexArrays( 1, &mVAO );
    glDeleteBuffers( 1, &mVBO );
}

void XYZ::init()
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

    // 1rst attribute buffer : vertices
    glBindBuffer(GL_ARRAY_BUFFER, mVBO);
    glVertexAttribPointer(0, 3, GL_FLOAT,GL_FALSE, sizeof(Vertex), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    // 2nd attribute buffer : colors
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
}

void XYZ::draw()
{
    glUseProgram(mMaterial.mShader->getProgram());
    glBindVertexArray(mVAO);
    mMaterial.mShader->transmitUniformData(&mMatrix, &mMaterial);

    glDrawArrays(GL_LINES, 0, signed(mVertices.size()));
}
