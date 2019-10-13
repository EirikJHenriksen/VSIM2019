#ifndef BILLBOARD_H
#define BILLBOARD_H

#include "visualobject.h"
#include "camera.h"
#include <cmath>

class Billboard : public VisualObject
{
public:
    Billboard(std::string objectName, gs2019::Vector3D inTrans, gs2019::Vector3D inScale);

    ~Billboard() override;

    void updateUVFrame(int frameNum);

    void init() override;
    void draw() override;

    gs2019::Vector3D BillboardPosition{0.f, 0.f, 0.f};
    gs2019::Vector3D BillboardScale{1.f, 1.f, 1.f};
};

#endif // BILLBOARD_H
