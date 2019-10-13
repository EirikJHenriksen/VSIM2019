#ifndef OBJMESH_H
#define OBJMESH_H
#include "visualobject.h"


class ObjMesh : public VisualObject
{
public:
    ObjMesh();
    ObjMesh(std::string objectName, std::string filename, gs2019::Vector3D inTrans, gs2019::Vector3D inRot, float rotAmount, gs2019::Vector3D inScale, bool contRot, bool bounce);

    ~ObjMesh() override;

    void readFile(std::string filename);

    virtual void draw() override;
    virtual void init() override;

    void move(float x, float y, float z, gs2019::Vector3D surfNormal) override;
    void rotate(float amount, int x, int y, int z) override;
    void setLocation(float inX, float inY, float inZ) override;
    gs2019::Vector3D getPosition() override;
    gs2019::Vector3D getRotation() override;

private:
    float mx, my, mz; // position
    float mrx, mry, mrz; // rotation
};

#endif // OBJMESH_H
