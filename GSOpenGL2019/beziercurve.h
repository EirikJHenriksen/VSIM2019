#ifndef BEZIERCURVE_H
#define BEZIERCURVE_H

#include "visualobject.h"
#include "mathlab.h"

typedef gs2019::Vector3D Vec3;
class BezierCurve : public VisualObject
{
public:
    BezierCurve(Vec3 pointA, Vec3 pointB, Vec3 pointC, Vec3 pointD, int degree, float resolution, bool drawCurve, bool drawControll);
    ~BezierCurve() override;

    Vec3 b[4];

    Vec3 curveColor = {1.f, 0.f, 0.f};
    Vec3 controllColor = {0.f, 0.f, 1.f};

    Vec3 evaluateBezier(int degree, float t);

    void init() override;
    void draw() override;
};

#endif // BEZIERCURVE_H
