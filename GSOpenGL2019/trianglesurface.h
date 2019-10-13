#ifndef TRIANGLESURFACE_H
#define TRIANGLESURFACE_H

#include "visualobject.h"
#include "collisionaabb.h"
#include <fstream>



class TriangleSurface : public VisualObject
{
public:
    TriangleSurface(gs2019::Vector3D inTrans, gs2019::Vector3D inRot, float rotAmount, gs2019::Vector3D inScale, bool contRot, bool bounce);
    TriangleSurface(std::string objectName, std::string filnavn, gs2019::Vector3D inTrans, gs2019::Vector3D inRot, float rotAmount, gs2019::Vector3D inScale, bool contRot, bool bounce, bool collisionActive);

    ~TriangleSurface() override;

    void readFile(std::string filnavn);
    void construct();
    void writeFile(std::string filnavn);

    void init() override;
    void draw() override;

    gs2019::Vector3D findColDir();

    void move(float x, float y, float z, gs2019::Vector3D surfNormal) override;
    void rotate(float amount, int x, int y, int z) override;
    void setLocation(float inX, float inY, float inZ) override;

    void setDynamicObject(VisualObject* dynamicObject) override;

    gs2019::Vector3D getPosition() override;
    gs2019::Vector3D getRotation() override;
    gs2019::Vector3D getScale() override;

    CollisionAABB* mCollisionAABB = new CollisionAABB(this, false);

    bool mCollisionActive = false;

    bool collisionTest(VisualObject* inputObject) override;

private:
    float mx, my, mz; // position
    float mrx, mry, mrz; // rotation
    float msx, msy, msz; // scale
};

#endif // TRIANGLESURFACE_H
