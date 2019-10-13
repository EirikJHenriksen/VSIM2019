#include "collisionaabb.h"

CollisionAABB::CollisionAABB(VisualObject* inputMesh, bool isTrigger)
{
    // If false, an object will hit and stop/bounce of the surface, otherwise it will trigger something.
    mTrigger = isTrigger;

    // Mesh that owns this collision box.
    mOwnerMesh = inputMesh;
}

void CollisionAABB::generateAABB()
{
    // Traverse mVerticies of object and find relevant points and generate a collision box.
    for (auto it = mOwnerMesh->get_mVertecies().begin(); it != mOwnerMesh->get_mVertecies().end(); it++)
    {
        // Updating the maximum values
        if (maxXYZ.x < (*it).get_xyz().x)
            maxXYZ.x = (*it).get_xyz().x;

        if (maxXYZ.y < (*it).get_xyz().y)
            maxXYZ.y = (*it).get_xyz().y;

        if (maxXYZ.z < (*it).get_xyz().z)
            maxXYZ.z = (*it).get_xyz().z;

        // Updating the minimum values
        if (minXYZ.x > (*it).get_xyz().x)
            minXYZ.x = (*it).get_xyz().x;

        if (minXYZ.y > (*it).get_xyz().y)
            minXYZ.y = (*it).get_xyz().y;

        if (minXYZ.z > (*it).get_xyz().z)
            minXYZ.z = (*it).get_xyz().z;
    }

    maxXYZ = gs2019::Vector3D(maxXYZ.x  * mOwnerMesh->getScale().x, maxXYZ.y  * mOwnerMesh->getScale().y, maxXYZ.z  * mOwnerMesh->getScale().z);
    minXYZ = gs2019::Vector3D(minXYZ.x  * mOwnerMesh->getScale().x, minXYZ.y  * mOwnerMesh->getScale().y, minXYZ.z  * mOwnerMesh->getScale().z);

    // DEBUG:
//    qDebug() << "Minimum values:";
//    minXYZ.debugPrint();
//    qDebug() << "Maximum values:";
//    maxXYZ.debugPrint();
}

bool CollisionAABB::checkObjectOverlap(VisualObject* otherObject)
{

    // Update the location of the owner mesh in case it has moved.
    meshLocation = mOwnerMesh->getPosition();

    // If input object is within all three values return true, else false.
    int counter = 0;

    if (((minXYZ.x + meshLocation.x - otherObject->radius) <= otherObject->getPosition().x) && ((maxXYZ.x + meshLocation.x + otherObject->radius) >= otherObject->getPosition().x))
    {
        //qDebug() << "X-AXIS COLLISION";
        counter++;
    }

    if (((minXYZ.y + meshLocation.y - otherObject->radius) <= otherObject->getPosition().y) && ((maxXYZ.y + meshLocation.y + otherObject->radius) >= otherObject->getPosition().y))
    {
        //qDebug() << "Y-AXIS COLLISION";
        counter++;
    }

    if (((minXYZ.z + meshLocation.z - otherObject->radius) <= otherObject->getPosition().z) && ((maxXYZ.z + meshLocation.z + otherObject->radius) >= otherObject->getPosition().z))
    {
        //qDebug() << "Z-AXIS COLLISION";
        counter++;
    }

    if (counter == 3)
    {
        //qDebug() << "COLLISION!!!";
        return true;
    }
    else
    {
        return false;
    }
    return false;
}
