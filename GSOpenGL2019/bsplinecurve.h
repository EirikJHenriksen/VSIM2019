#ifndef BSPLINECURVE_H
#define BSPLINECURVE_H

#include <vector>
#include "visualobject.h"


typedef gs2019::Vector3D Vec3;
class BSplineCurve : public VisualObject
{
public:
    // ikke alle disse funksjonene er nødvendige
    BSplineCurve();
    BSplineCurve(std::vector<float> knots, std::vector<Vec3> controlpoints, int degree=2);
    void initVertexBufferObjects();
    void draw(GLint positionAttribute, GLint colorAttribute, GLint textureAttribute=-1);
    void setKnotsAndControlPoints(std::vector<float> knots, std::vector<Vec3> points);
    Vec3 evaluateBSpline(int my, float x);
    int findKnotInterval(float x);
    Vec3 evaluateBSpline(int degree, int startKnot, float x);


private:
    Vec3 b[4];      // control points
    int n;          //n = number of knots
    int d;          //d = degree
    std::vector<float> t;   // knots
};

#endif // BSPLINECURVE_H
