#include "bsplinecurve.h"

BSplineCurve::BSplineCurve()
{

}

BSplineCurve::BSplineCurve(std::vector<float> knots, std::vector<Vec3> controlpoints, int degree)
{
    d = degree;

    setKnotsAndControlPoints(knots, controlpoints);
    updateVertecies();
}

BSplineCurve::~BSplineCurve()
{

}

void BSplineCurve::updateVertecies()
{
    // Creates vertecies so you can see the BSpline
    Vertex temp(Vec3(0.f, 0.f, 0.f), Vec3(1.f, 0.f, 0.f), Vec2(0.f, 0.f));

    mVertices.clear();

    float SplineResoulution{100.f};
    for (unsigned int i = 0; i <= SplineResoulution; i++)
    {
        temp.set_xyz(evaluateBSpline(d, i/SplineResoulution).x, evaluateBSpline(d, i/SplineResoulution).y, evaluateBSpline(d, i/SplineResoulution).z);
        mVertices.push_back(temp);

        //qDebug() << "Vector (" << temp.get_xyz().x << temp.get_xyz().y << temp.get_xyz().z << ")";
    }

    mMatrix.setToIdentity();
    mMatrix.translate(0.f, 0.f, 0.f);
    mMatrix.scale(1.f, 1.f, 1.f);

    init();
}

void BSplineCurve::setKnotsAndControlPoints(std::vector<float> knots, std::vector<Vec3> points)
{
    // Resets vectors and arrays.
    t.clear();
    b[0] = Vec3();
    b[1] = Vec3();
    b[2] = Vec3();
    b[3] = Vec3();

    n = knots.size();

    //qDebug() << "Degree: " << d << " Number of knots: " << n;

    for (int i = 0; i < points.size(); i++)
    {
        b[i] = points[i];
    }

    for (int i = 0; i < knots.size(); i++)
    {
        t.push_back(knots[i]);
    }
}

Vec3 BSplineCurve::travelAlongSpline(float inputTime)
{
    return evaluateBSpline(d, inputTime);
}

// Parametre inn:
// x - en parameterverdi på skjøtvektor
// my - et tall slik at t[my] <= x < t[my+1]
// Returverdi: et punkt på splinekurven
// b,n,d,t er objektvariable i klassen BSplineCurve
Vec3 BSplineCurve::evaluateBSpline(int my, float x)
{
    Vec3 a[20]; // forutsetter da at n+d+1 <= 20
    for (int j=0; j<=d; j++)
        a[d-j] = b[my-j];

    for (int k=d; k>0; k--) {
        int j = my-k;
        for (int i=0; i<k; i++) {
            j++;
            float w = (x-t[j])/(t[j+k]-t[j]);
            a[i] = a[i] * (1-w) + a[i+1] * w;
        }
    }
    return a[0];
}

void BSplineCurve::init()
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

void BSplineCurve::draw()
{
    glBindVertexArray(mVAO);
    glUniformMatrix4fv(mMatrixUniform, 1, GL_TRUE, mMatrix.constData());

    glDrawArrays(GL_LINE_STRIP, 0, signed(mVertices.size())); //GL_LINE_STRIP
}
