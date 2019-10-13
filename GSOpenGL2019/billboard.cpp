#include "billboard.h"

Billboard::Billboard(std::string objectName, gs2019::Vector3D inTrans, gs2019::Vector3D inScale)
{
    mName = objectName;

    // Normal (Used for color for now)
    gs2019::Vector3D Normal(1.f, 1.f, 1.f);

    // Creates UV coordinates
    gs2019::Vector2D UV1(0.f, 1.f/4.f);
    gs2019::Vector2D UV2(1.f/4.f, 1.f/4.f);
    gs2019::Vector2D UV3(1.f/4.f, 0.f);
    gs2019::Vector2D UV4(1.f/4.f, 0.f);
    gs2019::Vector2D UV5(0.f, 0.f);
    gs2019::Vector2D UV6(0.f, 1.f/4.f);

    // Creates vertex points
    gs2019::Vector3D vert1(-1.f, -1.f, 0.f);
    gs2019::Vector3D vert2(1.f, -1.f, 0.f);
    gs2019::Vector3D vert3(1.f, 1.f, 0.f);
    gs2019::Vector3D vert4(-1.f, 1.f, 0.f);

    // Push vertecies into array!
    mVertices.push_back(Vertex{vert1, Normal, UV1});
    mVertices.push_back(Vertex{vert2, Normal, UV2});
    mVertices.push_back(Vertex{vert3, Normal, UV3});

    mVertices.push_back(Vertex{vert3, Normal, UV4});
    mVertices.push_back(Vertex{vert4, Normal, UV5});
    mVertices.push_back(Vertex{vert1, Normal, UV6});

    mMatrix.setToIdentity();

    //Transformasjon
    mMatrix.translate(inTrans.x, inTrans.y, inTrans.z);
    mMatrix.scale(inScale.x, inScale.y, inScale.z);

    BillboardPosition = inTrans;
    BillboardScale = inScale;
}

Billboard::~Billboard()
{
    glDeleteVertexArrays( 1, &mVAO );
    glDeleteBuffers( 1, &mVBO );
}

// Changes the UV's of the vertecies to allow "sprite-animations"
void Billboard::updateUVFrame(int frameNum)
{
    // Supports 6 frames for now.
    switch (frameNum)
    {
    case 0:
        // ROW 1 - COLUMN 1
        mVertices.at(0).set_uv(0.f + (1.f/4.f)*frameNum, 1.f/4.f);
        mVertices.at(1).set_uv(1.f/4.f + (1.f/4.f)*frameNum, 1.f/4.f);
        mVertices.at(2).set_uv(1.f/4.f + (1.f/4.f)*frameNum, 0.f);
        mVertices.at(3).set_uv(1.f/4.f + (1.f/4.f)*frameNum, 0.f);
        mVertices.at(4).set_uv(0.f + (1.f/4.f)*frameNum, 0.f);
        mVertices.at(5).set_uv(0.f + (1.f/4.f)*frameNum, 1.f/4.f);
        break;
    case 1:
        // ROW 1 - COLUMN 2
        mVertices.at(0).set_uv(0.f + (1.f/4.f)*frameNum, 1.f/4.f);
        mVertices.at(1).set_uv(1.f/4.f + (1.f/4.f)*frameNum, 1.f/4.f);
        mVertices.at(2).set_uv(1.f/4.f + (1.f/4.f)*frameNum, 0.f);
        mVertices.at(3).set_uv(1.f/4.f + (1.f/4.f)*frameNum, 0.f);
        mVertices.at(4).set_uv(0.f + (1.f/4.f)*frameNum, 0.f);
        mVertices.at(5).set_uv(0.f + (1.f/4.f)*frameNum, 1.f/4.f);
        break;
    case 2:
        // ROW 1 - COLUMN 3
        mVertices.at(0).set_uv(0.f + (1.f/4.f)*frameNum, 1.f/4.f);
        mVertices.at(1).set_uv(1.f/4.f + (1.f/4.f)*frameNum, 1.f/4.f);
        mVertices.at(2).set_uv(1.f/4.f + (1.f/4.f)*frameNum, 0.f);
        mVertices.at(3).set_uv(1.f/4.f + (1.f/4.f)*frameNum, 0.f);
        mVertices.at(4).set_uv(0.f + (1.f/4.f)*frameNum, 0.f);
        mVertices.at(5).set_uv(0.f + (1.f/4.f)*frameNum, 1.f/4.f);
        break;
    case 3:
        // ROW 1 - COLUMN 4
        mVertices.at(0).set_uv(0.f + (1.f/4.f)*frameNum, 1.f/4.f);
        mVertices.at(1).set_uv(1.f/4.f + (1.f/4.f)*frameNum, 1.f/4.f);
        mVertices.at(2).set_uv(1.f/4.f + (1.f/4.f)*frameNum, 0.f);
        mVertices.at(3).set_uv(1.f/4.f + (1.f/4.f)*frameNum, 0.f);
        mVertices.at(4).set_uv(0.f + (1.f/4.f)*frameNum, 0.f);
        mVertices.at(5).set_uv(0.f + (1.f/4.f)*frameNum, 1.f/4.f);
        break;
    case 4:
        // ROW 2 - COLUMN 1
        frameNum = 0;
        mVertices.at(0).set_uv(0.f + (1.f/4.f)*frameNum, 1.f/4.f + (1.f/4.f)*1);
        mVertices.at(1).set_uv(1.f/4.f + (1.f/4.f)*frameNum, 1.f/4.f + (1.f/4.f)*1);
        mVertices.at(2).set_uv(1.f/4.f + (1.f/4.f)*frameNum, 0.f + (1.f/4.f)*1);
        mVertices.at(3).set_uv(1.f/4.f + (1.f/4.f)*frameNum, 0.f + (1.f/4.f)*1);
        mVertices.at(4).set_uv(0.f + (1.f/4.f)*frameNum, 0.f + (1.f/4.f)*1);
        mVertices.at(5).set_uv(0.f + (1.f/4.f)*frameNum, 1.f/4.f + (1.f/4.f)*1);
        break;
    case 5:
        // ROW 2 - COLUMN 2
        frameNum = 1;
        mVertices.at(0).set_uv(0.f + (1.f/4.f)*frameNum, 1.f/4.f + (1.f/4.f)*1);
        mVertices.at(1).set_uv(1.f/4.f + (1.f/4.f)*frameNum, 1.f/4.f + (1.f/4.f)*1);
        mVertices.at(2).set_uv(1.f/4.f + (1.f/4.f)*frameNum, 0.f + (1.f/4.f)*1);
        mVertices.at(3).set_uv(1.f/4.f + (1.f/4.f)*frameNum, 0.f + (1.f/4.f)*1);
        mVertices.at(4).set_uv(0.f + (1.f/4.f)*frameNum, 0.f + (1.f/4.f)*1);
        mVertices.at(5).set_uv(0.f + (1.f/4.f)*frameNum, 1.f/4.f + (1.f/4.f)*1);
        break;
    }

// Make a loop that sets the correct numbers later.
//    for (int i = 0; i < 6; ++i)
//    {
//        //gs2019::Vector3D VertexPos = mVertices.at(i).set_uv();

//        //mVertices.at(i).get_uv();
//        //mVertices.at(i).set_uv();
//    }

    // Set up the new UV's.
    init();
}

void Billboard::init()
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

void Billboard::draw()
{
    glUseProgram(mMaterial.mShader->getProgram());
    glBindVertexArray(mVAO);
    mMaterial.mShader->transmitUniformData(&mMatrix, &mMaterial);
    glDrawArrays(GL_TRIANGLES, 0, signed(mVertices.size()));
}
