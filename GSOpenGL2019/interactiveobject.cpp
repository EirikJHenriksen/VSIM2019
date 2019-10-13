#include "interactiveobject.h"

InteractiveObject::InteractiveObject()
{
    // UV
    gs2019::Vector2D uv(0, 0);

    // Making colors
    gs2019::Vector3D colorR(1, 0, 0);
    gs2019::Vector3D colorG(0, 1, 0);
    gs2019::Vector3D colorB(0, 0, 1);

    gs2019::Vector3D colorP(1, 0, 1);
    gs2019::Vector3D colorY(1, 1, 0);
    gs2019::Vector3D colorO(0.9f, 0.4f, 0.1f);

    // Making vectors for vertices
    gs2019::Vector3D vert1(-0.5f, 0.5f, 0.5f);
    gs2019::Vector3D vert2(0.5f, 0.5f, 0.5f);
    gs2019::Vector3D vert3(0.5f, -0.5f, 0.5f);
    gs2019::Vector3D vert4(-0.5f, -0.5f, 0.5f);

    gs2019::Vector3D vert5(-0.5f, -0.5f, -0.5f);
    gs2019::Vector3D vert6(0.5f, -0.5f, -0.5f);
    gs2019::Vector3D vert7(0.5f, 0.5f, -0.5f);
    gs2019::Vector3D vert8(-0.5f, 0.5f, -0.5f);

    // Wall 1
    mVertices.push_back(Vertex{vert1, colorR, uv});
    mVertices.push_back(Vertex{vert2, colorR, uv});
    mVertices.push_back(Vertex{vert3, colorR, uv});

    mVertices.push_back(Vertex{vert4, colorR, uv});
    mVertices.push_back(Vertex{vert1, colorR, uv});
    mVertices.push_back(Vertex{vert3, colorR, uv});

    // Wall 2
    mVertices.push_back(Vertex{vert5, colorG, uv});
    mVertices.push_back(Vertex{vert6, colorG, uv});
    mVertices.push_back(Vertex{vert7, colorG, uv});

    mVertices.push_back(Vertex{vert8, colorG, uv});
    mVertices.push_back(Vertex{vert5, colorG, uv});
    mVertices.push_back(Vertex{vert7, colorG, uv});

    // Wall 3
    mVertices.push_back(Vertex{vert7, colorB, uv});
    mVertices.push_back(Vertex{vert2, colorB, uv});
    mVertices.push_back(Vertex{vert1, colorB, uv});

    mVertices.push_back(Vertex{vert1, colorB, uv});
    mVertices.push_back(Vertex{vert8, colorB, uv});
    mVertices.push_back(Vertex{vert7, colorB, uv});

    // Wall 4
    mVertices.push_back(Vertex{vert6, colorP, uv});
    mVertices.push_back(Vertex{vert5, colorP, uv});
    mVertices.push_back(Vertex{vert4, colorP, uv});

    mVertices.push_back(Vertex{vert4, colorP, uv});
    mVertices.push_back(Vertex{vert3, colorP, uv});
    mVertices.push_back(Vertex{vert6, colorP, uv});

    // Wall 5
    mVertices.push_back(Vertex{vert5, colorY, uv});
    mVertices.push_back(Vertex{vert8, colorY, uv});
    mVertices.push_back(Vertex{vert4, colorY, uv});

    mVertices.push_back(Vertex{vert8, colorY, uv});
    mVertices.push_back(Vertex{vert1, colorY, uv});
    mVertices.push_back(Vertex{vert4, colorY, uv});

    // Wall 6
    mVertices.push_back(Vertex{vert7, colorO, uv});
    mVertices.push_back(Vertex{vert6, colorO, uv});
    mVertices.push_back(Vertex{vert2, colorO, uv});

    mVertices.push_back(Vertex{vert6, colorO, uv});
    mVertices.push_back(Vertex{vert3, colorO, uv});
    mVertices.push_back(Vertex{vert2, colorO, uv});

    mMatrix.setToIdentity();

    //Transformasjonstest
    //mMatrix.translate(0.f, 0.25f, 0.f);
    //mMatrix.rotate(45.f, 0, 1, 0);
    mMatrix.scale(0.5f, 1.5f, 0.5f);
}

InteractiveObject::~InteractiveObject()
{
    glDeleteVertexArrays( 1, &mVAO );
    glDeleteBuffers( 1, &mVBO );
}

void InteractiveObject::init(GLint matrixUniform)
{
    mMatrixUniform = matrixUniform;

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

    //enable the matrixUniform
    //mMatrixUniform = glGetUniformLocation( matrixUniform, "matrix" );

    glBindVertexArray(0);
}

void InteractiveObject::draw()
{
    //==================== Rotasjon ===================
    ++time;

    //just to make the object rotate:
    //mMatrix.rotate(1.f, 0, 1, 0);
    //mMatrix.translate(0.f, sin(time*0.1f)*0.05f, 0.f);

    glBindVertexArray(mVAO);
    glUniformMatrix4fv(mMatrixUniform, 1, GL_TRUE, mMatrix.constData());

    glDrawArrays(GL_TRIANGLES, 0, signed(mVertices.size()));
}

void InteractiveObject::move(float x, float y, float z, gs2019::Vector3D surfNormal)
{
    mMatrix.translate(x, y, z);
    mx = mMatrix[{0,3}];
    my = mMatrix[{1,3}];
    mz = mMatrix[{2,3}];
}

void InteractiveObject::rotate(float amount, int x, int y, int z)
{
    mMatrix.rotate(amount, x, y, z);
    mrx = mMatrix[{0,3}];
    mry = mMatrix[{1,3}];
    mrz = mMatrix[{2,3}];
}

gs2019::Vector3D InteractiveObject::getPosition()
{
    return gs2019::Vector3D(mx, my, mz);
}
