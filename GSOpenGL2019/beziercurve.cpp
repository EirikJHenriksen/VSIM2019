#include "beziercurve.h"

BezierCurve::BezierCurve(Vec3 pointA, Vec3 pointB, Vec3 pointC, Vec3 pointD, int degree, float resolution, bool drawCurve, bool drawControll)
{
    // Defines points in array "b"
    b[0] = pointA;
    b[1] = pointB;
    b[2] = pointC;
    b[3] = pointD;

    // Calculate resolution
    resolution = 1.f / resolution;

    // Draws the Bezier Curve
    if (drawCurve)
    {
        for (float i = 0; i <= 1.f; i += resolution)
        {
            mVertices.push_back(Vertex{evaluateBezier(degree, i), curveColor, gs2019::Vector2D(1.f, 1.f)});
        }
    }

    // Draws the Controll Polygon
    if (drawControll)
    {
        mVertices.push_back(Vertex{pointD, controllColor, gs2019::Vector2D(1.f, 1.f)});
        mVertices.push_back(Vertex{pointC, controllColor, gs2019::Vector2D(1.f, 1.f)});
        mVertices.push_back(Vertex{pointB, controllColor, gs2019::Vector2D(1.f, 1.f)});
        mVertices.push_back(Vertex{pointA, controllColor, gs2019::Vector2D(1.f, 1.f)});
    }

    mMatrix.setToIdentity();
}

BezierCurve::~BezierCurve()
{

}

Vec3 BezierCurve::evaluateBezier(int degree, float t)
{
    Vec3 c[4];

    for (int i=0; i<4; i++)
        c[i] = b[i];

    for (int k=1; k<=degree; k++)
    {
        for (int i=0; i<degree-k+1; i++)
            c[i] = c[i] * (1-t) + c[i+1] * t;
    }
    return c[0];
}

void BezierCurve::init()
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

    //enable the matrixUniform
    //mMatrixUniform = glGetUniformLocation( matrixUniform, "matrix" );

    glBindVertexArray(0);
}

void BezierCurve::draw()
{
    glBindVertexArray(mVAO);
    glUniformMatrix4fv(mMatrixUniform, 1, GL_TRUE, mMatrix.constData());

    glDrawArrays(GL_LINE_STRIP, 0, signed(mVertices.size()));
}
