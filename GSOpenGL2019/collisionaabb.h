#ifndef COLLISIONAABB_H
#define COLLISIONAABB_H

#include "visualobject.h"

class CollisionAABB
{
public:
    CollisionAABB(VisualObject* inputMesh, bool isTrigger);

    void generateAABB();
    bool checkObjectOverlap(VisualObject* otherObject);

    bool mTrigger = false;

    VisualObject* mOwnerMesh;

    gs2019::Vector3D maxXYZ{0.f, 0.f, 0.f};
    gs2019::Vector3D minXYZ{0.f, 0.f, 0.f};

    gs2019::Vector3D meshLocation{0.f, 0.f, 0.f};
};

#endif // COLLISIONAABB_H
