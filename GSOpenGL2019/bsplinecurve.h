#ifndef BSPLINECURVE_H
#define BSPLINECURVE_H

#include <vector>
#include "visualobject.h"


typedef gs2019::Vector3D Vec3;
typedef gs2019::Vector2D Vec2;
class BSplineCurve : public VisualObject
{
public:
    // ikke alle disse funksjonene er nødvendige
    BSplineCurve();
    BSplineCurve(std::vector<float> knots, std::vector<Vec3> controlpoints, int degree=2);

    void setKnotsAndControlPoints(std::vector<float> knots, std::vector<Vec3> points);

    Vec3 evaluateBSpline(int my, float x);
    Vec3 evaluateBSpline(int degree, int startKnot, float x);
    int findKnotInterval(float x);

    void init() override;//VertexBufferObjects();
    void draw() override;//(GLint positionAttribute, GLint colorAttribute, GLint textureAttribute=-1);

private:
    Vec3 b[4];      // control points
    int n;          //n = number of knots
    int d;          //d = degree
    std::vector<float> t;   // knots
};

#endif // BSPLINECURVE_H
