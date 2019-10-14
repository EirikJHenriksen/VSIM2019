#include "graph.h"

#include <iostream>
#include <fstream>

graph::graph(bool itDoesNothing)
{
    // Loads LAS information from a txt-file!!!
    loadLASData("../GSOpenGL2019/Data/MapData/godoynes_simplified.txt");

    mMatrix.setToIdentity();
}

// THIS CONSTRUCTOR DOESENT WORK NOW!!!
graph::graph(int subdivisions, float sizeX , float sizeY, float sizeZ, float inHeightOffset) : VisualObject ()
{
    // UV
    gs2019::Vector2D UV1(0.f, 1.f);
    gs2019::Vector2D UV2(0.f, 0.f);
    gs2019::Vector2D UV3(1.f, 0.f);
    gs2019::Vector2D UV4(0.f, 1.f);
    gs2019::Vector2D UV5(1.f, 0.f);
    gs2019::Vector2D UV6(1.f, 1.f);

    heightOffset = inHeightOffset;

    mSizeX = sizeX;
    mSizeZ = sizeZ;
    mSubdivisions = subdivisions;

    //subdivisions += 1;

    float distanceX = sizeX / subdivisions;
    float distanceZ = sizeZ / subdivisions;

    for (auto i = 0; i < subdivisions; ++i)
    {
        for (auto j = 0; j < subdivisions; ++j)
        {
            gs2019::Vector3D Normal(0.f, 1.f, 0.f);

            // Creates vertecies
            //gs2019::Vector3D vert1((distanceX*(i - 0) - (sizeX/2)), applyHeightFunction((distanceX*(i - 0) - (sizeX/2)), sizeY, (distanceZ*(j - 0)) - (sizeZ/2)), (distanceZ*(j - 0)) - (sizeZ/2));
            //gs2019::Vector3D vert2((distanceX*(i - 0) - (sizeX/2)), applyHeightFunction((distanceX*(i - 0) - (sizeX/2)), sizeY, (distanceZ*(j + 1)) - (sizeZ/2)), (distanceZ*(j + 1)) - (sizeZ/2));
            //gs2019::Vector3D vert3((distanceX*(i + 1) - (sizeX/2)), applyHeightFunction((distanceX*(i + 1) - (sizeX/2)), sizeY, (distanceZ*(j + 1)) - (sizeZ/2)), (distanceZ*(j + 1)) - (sizeZ/2));
            //gs2019::Vector3D vert4((distanceX*(i + 1) - (sizeX/2)), applyHeightFunction((distanceX*(i + 1) - (sizeX/2)), sizeY, (distanceZ*(j - 0)) - (sizeZ/2)), (distanceZ*(j - 0)) - (sizeZ/2));

            // Sets normals for first polygon in quad.
            //Normal = (vert3 - vert2)^(vert1 - vert2);
            //
            // Push vertecies into array!
            //mVertices.push_back(Vertex{vert1, Normal, gs2019::Vector2D(UV1.x, UV1.y)});
            //mVertices.push_back(Vertex{vert2, Normal, gs2019::Vector2D(UV2.x, UV2.y)});
            //mVertices.push_back(Vertex{vert3, Normal, gs2019::Vector2D(UV3.x, UV3.y)});
            //
            // Sets normals for second polygon in quad.
            //Normal = (vert1 - vert4)^(vert3 - vert4);
            //
            //mVertices.push_back(Vertex{vert1, Normal, gs2019::Vector2D(UV3.x, UV3.y)});
            //mVertices.push_back(Vertex{vert3, Normal, gs2019::Vector2D(UV4.x, UV4.y)});
            //mVertices.push_back(Vertex{vert4, Normal, gs2019::Vector2D(UV1.x, UV1.y)});
        }
    }

    mMatrix.setToIdentity();
}

void graph::loadLASData(std::string filnavn)
{
    float minX{0.f};
    float maxX{0.f};

    float minY{0.f};
    float maxY{0.f};

    float minZ{0.f};
    float maxZ{0.f};


    // Read txt file generated from LAS data
    std::ifstream inn;
    inn.open(filnavn.c_str());

    if (inn.is_open())
    {
        unsigned int n;
        Vec3 readPoints;
        inn >> n;
        mVertices.reserve(n);

        inn >> readPoints.x >> readPoints.z >> readPoints.y;

        // Invert along z-axis.
        readPoints.z = readPoints.z * -1.f;

        // Sets initial maximum and minimum values.
        minX = readPoints.x;    maxX = readPoints.x;
        minY = readPoints.y;    maxY = readPoints.y;
        minZ = readPoints.z;    maxZ = readPoints.z;

        mMapPoints.push_back(readPoints);

        for (unsigned int i=1; i<n; i++)
        {
            inn >> readPoints.x >> readPoints.z >> readPoints.y;

            // Invert along z-axis.
            readPoints.z = readPoints.z * -1.f;

            // Updates maximum and minimum.
            if (readPoints.x < minX)
                minX = readPoints.x;
            if (readPoints.y < minY)
                minY = readPoints.y;
            if (readPoints.z < minZ)
                minZ = readPoints.z;

            if (readPoints.x > maxX)
                maxX = readPoints.x;
            if (readPoints.y > maxY)
                maxY = readPoints.y;
            if (readPoints.z > maxZ)
                maxZ = readPoints.z;

            mMapPoints.push_back(readPoints);
        }
        inn.close();
    }

    //qDebug() << "Min points: " << minX << minY << minZ;
    //qDebug() << "Max points: " << maxX << maxY << maxZ;

    Vec3 vertexOffset{0.f, 0.f, 0.f};

    vertexOffset.x = minX + ((maxX - minX)/2);
    vertexOffset.y = minY + ((maxY - minY)/2);
    vertexOffset.z = minZ + ((maxZ - minZ)/2);

    qDebug() << "Min points: " << minX - vertexOffset.x << minY - vertexOffset.y << minZ - vertexOffset.z;
    qDebug() << "Max points: " << maxX - vertexOffset.x << maxY - vertexOffset.y << maxZ - vertexOffset.z;

    // Shifts all map points towards the center of the scene.
    for(int i = 0; i < mMapPoints.size(); i++)
    {
        mMapPoints[i].x = mMapPoints[i].x - vertexOffset.x;
        mMapPoints[i].y = mMapPoints[i].y - vertexOffset.y;
        mMapPoints[i].z = mMapPoints[i].z - vertexOffset.z;
    }

    buildGrid(64, (maxX - vertexOffset.x)*2, 1.f, (maxX - vertexOffset.x)*2, -1.5f); //Set subdivisions to 64 in final version!!!
}

void graph::buildGrid(int subdivisions, float sizeX , float sizeY, float sizeZ, float inHeightOffset)
{
    // UV
    //gs2019::Vector2D UV1(0.f, 1.f);
    //gs2019::Vector2D UV2(0.f, 0.f);
    //gs2019::Vector2D UV3(1.f, 0.f);
    //gs2019::Vector2D UV4(0.f, 1.f);
    //gs2019::Vector2D UV5(1.f, 0.f);
    //gs2019::Vector2D UV6(1.f, 1.f);

    //==============================================================
    heightOffset = inHeightOffset;

    mSizeX = sizeX;
    mSizeZ = sizeZ;
    mSubdivisions = subdivisions;

    float min = -(sizeX/2);
    float spacing = sizeX/mSubdivisions;

    for (int i = 0; i <= mSubdivisions; i++)
    {
        for (int j = 0; j <= mSubdivisions; j++)
        {
            //qDebug() << min+(i*spacing) << 0.f << min+(j*spacing);
            Vertex temp;
            temp.set_xyz(min+(i*spacing), 0.f, min+(j*spacing));
            mVertices.push_back(temp);
        }
    }

    // make indices
    // each loop is a square
    for (int i = 0; i <= (mSubdivisions)*(mSubdivisions)+(mSubdivisions-2); i++)
    {
        if ((i+1)%(mSubdivisions+1) != 0)
        {
            mIndices.push_back(i);
            mIndices.push_back(i+1);
            mIndices.push_back(i+mSubdivisions+1);

            mIndices.push_back(i+1);
            mIndices.push_back(i+mSubdivisions+2);
            mIndices.push_back(i+mSubdivisions+1);
        }
    }

    //    float distanceX = sizeX / subdivisions;
    //    float distanceZ = sizeZ / subdivisions;

    //    for (auto i = 0; i < subdivisions; ++i)
    //    {
    //        for (auto j = 0; j < subdivisions; ++j)
    //        {
    //            gs2019::Vector3D Normal(0.f, 1.f, 0.f);

    //            // Creates vertecies - OLD METHOD -
    //            //gs2019::Vector3D vert1((distanceX*(i - 0) - (sizeX/2)), applyHeightFunction((distanceX*(i - 0) - (sizeX/2)), sizeY, (distanceZ*(j - 0)) - (sizeZ/2)), (distanceZ*(j - 0)) - (sizeZ/2));
    //            //gs2019::Vector3D vert2((distanceX*(i - 0) - (sizeX/2)), applyHeightFunction((distanceX*(i - 0) - (sizeX/2)), sizeY, (distanceZ*(j + 1)) - (sizeZ/2)), (distanceZ*(j + 1)) - (sizeZ/2));
    //            //gs2019::Vector3D vert3((distanceX*(i + 1) - (sizeX/2)), applyHeightFunction((distanceX*(i + 1) - (sizeX/2)), sizeY, (distanceZ*(j + 1)) - (sizeZ/2)), (distanceZ*(j + 1)) - (sizeZ/2));
    //            //gs2019::Vector3D vert4((distanceX*(i + 1) - (sizeX/2)), applyHeightFunction((distanceX*(i + 1) - (sizeX/2)), sizeY, (distanceZ*(j - 0)) - (sizeZ/2)), (distanceZ*(j - 0)) - (sizeZ/2));

    //            // Make grid!
    //            gs2019::Vector3D vert1((distanceX*(i - 0) - (sizeX/2)), sizeY, (distanceZ*(j - 0)) - (sizeZ/2));
    //            gs2019::Vector3D vert2((distanceX*(i - 0) - (sizeX/2)), sizeY, (distanceZ*(j + 1)) - (sizeZ/2));
    //            gs2019::Vector3D vert3((distanceX*(i + 1) - (sizeX/2)), sizeY, (distanceZ*(j + 1)) - (sizeZ/2));
    //            gs2019::Vector3D vert4((distanceX*(i + 1) - (sizeX/2)), sizeY, (distanceZ*(j - 0)) - (sizeZ/2));

    //            // Sets normals for first polygon in quad.
    //            Normal = (vert3 - vert2)^(vert1 - vert2);

    //            // Push vertecies into array!
    //            mVertices.push_back(Vertex{vert1, Normal, gs2019::Vector2D(UV1.x, UV1.y)});
    //            mVertices.push_back(Vertex{vert2, Normal, gs2019::Vector2D(UV2.x, UV2.y)});
    //            mVertices.push_back(Vertex{vert3, Normal, gs2019::Vector2D(UV3.x, UV3.y)});

    //            // Sets normals for second polygon in quad.
    //            Normal = (vert1 - vert4)^(vert3 - vert4);

    //            mVertices.push_back(Vertex{vert1, Normal, gs2019::Vector2D(UV3.x, UV3.y)});
    //            mVertices.push_back(Vertex{vert3, Normal, gs2019::Vector2D(UV4.x, UV4.y)});
    //            mVertices.push_back(Vertex{vert4, Normal, gs2019::Vector2D(UV1.x, UV1.y)});
    //        }
    //    }

    locateNeighbours();

    applyHeightFunction();

    mMatrix.setToIdentity();
}

void graph::applyHeightFunction()
{
    //float result;
    //result = ((cos(inX/3) * cos(inX/3)) * inY*3) + heightOffset;
    //return result;

    // Only works correctly if the X and Z sizes of the grid are the same!!!
    float vertexGridSize = mSizeX / mSubdivisions;
    int points = 0;
    float height = 0;

    // for each vertex in grid
    for (int i = 0; i < mVertices.size(); i++)
    {
        qDebug() << "Processing: " << i+1 << " / " << mVertices.size();
        points = 0;
        height = 0;

        // for each datapoint...
        for (int j = 0; j < mMapPoints.size(); j++)
        {
            // if datapoint is within radius of vertex
            if ((mMapPoints[j].x < (mVertices[i].get_xyz().x + vertexGridSize)) && (mMapPoints[j].z < (mVertices[i].get_xyz().z + vertexGridSize)))
            {
                //qDebug() << "alters point height!";

                // Add height
                points++;
                height += mMapPoints[j].y;

                // Delete point
                mMapPoints.erase(mMapPoints.begin()+j);
                j--;
            }
        }

        if (points > 0)
            height = height/points;

        mVertices[i].set_xyz(mVertices[i].get_xyz().x, height, mVertices[i].get_xyz().z);
    }
}

float graph::findTargetHeight(gs2019::Vector3D location)
{
    if((location.x < (mSizeX/2) && location.z < (mSizeZ/2)) && (location.x > (-mSizeX/2) && location.z > (-mSizeZ/2)))
    {
        Vec2 coordinates{location.x, location.z};
        int TriangleID = findTriangleIndices(coordinates);

        //qDebug() << "TRIANGLE ID: " << TriangleID;

        if (TriangleID != (-1))
        {
            vertexA = Vec3(mVertices[mIndices[(TriangleID*3)+2]].get_xyz().x, mVertices[mIndices[(TriangleID*3)+2]].get_xyz().y, mVertices[mIndices[(TriangleID*3)+2]].get_xyz().z);
            vertexB = Vec3(mVertices[mIndices[(TriangleID*3)+1]].get_xyz().x, mVertices[mIndices[(TriangleID*3)+1]].get_xyz().y, mVertices[mIndices[(TriangleID*3)+1]].get_xyz().z);
            vertexC = Vec3(mVertices[mIndices[(TriangleID*3)]].get_xyz().x, mVertices[mIndices[(TriangleID*3)]].get_xyz().y, mVertices[mIndices[(TriangleID*3)]].get_xyz().z);

            Vec3 heightWeight = coordinates.barycentricCoordinates(Vec2(vertexA.x, vertexA.z),
                                                                                         Vec2(vertexB.x, vertexB.z),
                                                                                         Vec2(vertexC.x, vertexC.z));

            //heightWeight.debugPrint();

            heightCalculation = ((vertexC.y * heightWeight.z) + (vertexB.y * heightWeight.y) + (vertexA.y * heightWeight.x));

            //qDebug() << "Target Height:" << heightCalculation;
            return heightCalculation;
        }

        //qDebug() << "Within bounds: NO VALID HEIGHT!!!";
        return heightCalculation;
    }
    else
    {
        //qDebug() << "OUT OF BOUNDS!!!";
        return heightCalculation;
    }
}

float graph::findQuadLocation(gs2019::Vector3D location)
{
    //Finds the two triangles that makes up the quad that the object is within.

    float posX = location.x;
    float posZ = location.z;

    //int numberOfQuads = (mSubdivisions * mSubdivisions) * 2;

    //Dividing the grid into sectors based on what side of the X- and Z-axis the object is in.
    //int sectorWidth = mSubdivisions/2;

    int sectorResolution = (mSubdivisions/2);
    float sectorPosX = posX/((mSizeX/2)/sectorResolution);
    float sectorPosZ = posZ/((mSizeZ/2)/sectorResolution);

    int quadLocX = sectorPosX + sectorResolution + 1;
    int quadLocZ = sectorPosZ + sectorResolution + 1;

    // For debugging
    if(false)
    {
        if(posX < 0)
        {
            if(posZ < 0)
            {
                qDebug() << "UL: QUAD LOCATION X-AXIS: " << quadLocX <<" QUAD LOCATION Z-AXIS: " << quadLocZ;
            }
            else
            {
                qDebug() << "LL: QUAD LOCATION X-AXIS: " << quadLocX <<" QUAD LOCATION Z-AXIS: " << quadLocZ;
            }
        }
        else
        {
            if(posZ < 0)
            {
                qDebug() << "UR: QUAD LOCATION X-AXIS: " << quadLocX <<" QUAD LOCATION Z-AXIS: " << quadLocZ;
            }
            else
            {
                qDebug() << "LR: QUAD LOCATION X-AXIS: " << quadLocX <<" QUAD LOCATION Z-AXIS: " << quadLocZ;
            }
        }
    }

    return 1; //findTriangle(quadLocX, quadLocZ, gs2019::Vector2D(location.x, location.z));
}

void graph::locateNeighbours()
{
    mNeighbour.reserve(mIndices.size());

    // loop for each index, to set its neighbour
    for (uint i = 0; i < mIndices.size(); i++)
    {
        // in case there is no neighbour
        mNeighbour.push_back(-1);

        // use barysentric to find a point outside the edge
        // first index = i (then i+1, i+2)
        // triangle ABC
        Vec2 A;
        Vec2 B;
        Vec2 C;

        if (i%3 == 0)
        {
            // A
            A = Vec2(mVertices[mIndices[i]].get_xyz().x, mVertices[mIndices[i]].get_xyz().z);
            B = Vec2(mVertices[mIndices[i+1]].get_xyz().x, mVertices[mIndices[i+1]].get_xyz().z);
            C = Vec2(mVertices[mIndices[i+2]].get_xyz().x, mVertices[mIndices[i+2]].get_xyz().z);
        }
        else if ((i-1)%3 == 0)
        {
            // B
            A = Vec2(mVertices[mIndices[i]].get_xyz().x, mVertices[mIndices[i]].get_xyz().z);
            B = Vec2(mVertices[mIndices[i+1]].get_xyz().x, mVertices[mIndices[i+1]].get_xyz().z);
            C = Vec2(mVertices[mIndices[i-1]].get_xyz().x, mVertices[mIndices[i-1]].get_xyz().z);
        }
        else
        {
            // C
            A = Vec2(mVertices[mIndices[i]].get_xyz().x, mVertices[mIndices[i]].get_xyz().z);
            B = Vec2(mVertices[mIndices[i-2]].get_xyz().x, mVertices[mIndices[i-2]].get_xyz().z);
            C = Vec2(mVertices[mIndices[i-1]].get_xyz().x, mVertices[mIndices[i-1]].get_xyz().z);
        }

        // Makes a testpoint outside of the current triangle (It should appear in it's neighbour, opposite A)
        Vec2 testSpot;

        testSpot = (B * 0.5f) + (C * 0.5f);
        float t = 1.5f;
        testSpot = (A * (1-t)) + (testSpot * t);

        //A.debugPrint();
        //B.debugPrint();
        //C.debugPrint();
        //testSpot.debugPrint();

        // loop through triangles until you find the triangle its in
        for (uint firstIndexOfTriangle = 0; firstIndexOfTriangle <= mIndices.size()-3; firstIndexOfTriangle+=3)
        {
            // triangle DEF
            Vec2 D(mVertices[mIndices[firstIndexOfTriangle]].get_xyz().x, mVertices[mIndices[firstIndexOfTriangle]].get_xyz().z);
            Vec2 E(mVertices[mIndices[firstIndexOfTriangle+1]].get_xyz().x, mVertices[mIndices[firstIndexOfTriangle+1]].get_xyz().z);
            Vec2 F(mVertices[mIndices[firstIndexOfTriangle+2]].get_xyz().x, mVertices[mIndices[firstIndexOfTriangle+2]].get_xyz().z);

            // get the barysentric coordinates of the spot
            Vec3 bary = testSpot.barycentricCoordinates(F,E,D);

            // if inside
            if (bary.x > 0.f && bary.y > 0.f && bary.z > 0.f)
            {
                // in the neighbour, set mNeighbour to this triangle
                mNeighbour[i] = firstIndexOfTriangle/3;
            }
        }
    }
}

int graph::findTriangleIndices(gs2019::Vector2D targetLocation)
{
    int triangleID = 0;

    // First triangle
    triangleID = (findTriangleStage2(targetLocation, triangleID));

    return triangleID;
}

int graph::findTriangleStage2(gs2019::Vector2D targetLocation, int triangleID)
{
    Vec2 objectPos = targetLocation;

    //objectPos.debugPrint();

    Vec2 p0 = {mVertices[mIndices[(triangleID*3)]].get_xyz().x, mVertices[mIndices[(triangleID*3)]].get_xyz().z};
    Vec2 p1 = {mVertices[mIndices[(triangleID*3)+1]].get_xyz().x, mVertices[mIndices[(triangleID*3)+1]].get_xyz().z};
    Vec2 p2 = {mVertices[mIndices[(triangleID*3)+2]].get_xyz().x, mVertices[mIndices[(triangleID*3)+2]].get_xyz().z};

    Vec3 baryCoord = objectPos.barycentricCoordinates(p2, p1, p0);

    if ( (baryCoord.x >= 0) && (baryCoord.y >= 0) && (baryCoord.z >= 0) ) // inside
    {
        return triangleID;
    }
    else if ((baryCoord.z < baryCoord.x) && (baryCoord.z < baryCoord.y))
    {
        // Check x's neighbor
        if (mNeighbour[(triangleID*3)] < 0)
            return -1;
        else
            return findTriangleStage2(objectPos, mNeighbour[(triangleID*3)]);
    }
    else if ((baryCoord.y < baryCoord.z) && (baryCoord.y < baryCoord.x))
    {
        // Check y's neighbor
        if (mNeighbour[(triangleID*3)+1] < 0)
            return -1;
        else
            return findTriangleStage2(objectPos, mNeighbour[(triangleID*3)+1]);
    }
    else if ((baryCoord.x < baryCoord.y) && (baryCoord.x < baryCoord.z))
    {
        // Check z's neighbor
        if (mNeighbour[(triangleID*3)+2] < 0)
            return -1;
        else
            return findTriangleStage2(objectPos, mNeighbour[(triangleID*3)+2]);
    }

    return -1; // Outside the grid
}

float graph::findTriangle(int x, int z, gs2019::Vector2D targetLocation)
{
    int VerteciesInQuad{6};

    float objectHeight{0.f};

    gs2019::Vector3D triangle01;
    gs2019::Vector3D triangle02;

    Vertex* quadVertecies[6];

    // Quads are built down along the Z-Axis.

    // Finds the first vertex in the quad and writes out the next ones within it.
    for (int i = 1; i <= VerteciesInQuad; ++i)
    {
        // Makes pointers to the relevant vertecies.
        quadVertecies[i - 1] = &mVertices.at((((z - 1) * VerteciesInQuad) + ((x - 1) * (mSubdivisions * VerteciesInQuad)) - 1) + i);
    }

    // First triangle
    triangle01 = targetLocation.barycentricCoordinates(gs2019::Vector2D(quadVertecies[2]->get_xyz().x, quadVertecies[2]->get_xyz().z),
            gs2019::Vector2D(quadVertecies[1]->get_xyz().x, quadVertecies[1]->get_xyz().z),
            gs2019::Vector2D(quadVertecies[0]->get_xyz().x, quadVertecies[0]->get_xyz().z));

    // Second triangle
    triangle02 = targetLocation.barycentricCoordinates(gs2019::Vector2D(quadVertecies[5]->get_xyz().x, quadVertecies[5]->get_xyz().z),
            gs2019::Vector2D(quadVertecies[4]->get_xyz().x, quadVertecies[4]->get_xyz().z),
            gs2019::Vector2D(quadVertecies[3]->get_xyz().x, quadVertecies[3]->get_xyz().z));

    // Finds the height the target object should have based on what triangle it sits ontop of.
    if (triangle01.x >= 0.f && triangle01.y >= 0.f && triangle01.z >= 0.f)
    {
        //qDebug() << "TRIANGLE 1!";
        vertexA = Vec3(quadVertecies[2]->get_xyz().x, quadVertecies[2]->get_xyz().y, quadVertecies[2]->get_xyz().z);
        vertexB = Vec3(quadVertecies[1]->get_xyz().x, quadVertecies[1]->get_xyz().y, quadVertecies[1]->get_xyz().z);
        vertexC = Vec3(quadVertecies[0]->get_xyz().x, quadVertecies[0]->get_xyz().y, quadVertecies[0]->get_xyz().z);

        objectHeight = ((quadVertecies[2]->get_xyz().y * triangle01.x) + (quadVertecies[1]->get_xyz().y * triangle01.y) + (quadVertecies[0]->get_xyz().y * triangle01.z));
    }

    if (triangle02.x >= 0.f && triangle02.y >= 0.f && triangle02.z >= 0.f)
    {
        //qDebug() << "TRIANGLE 2!";
        vertexA = Vec3(quadVertecies[5]->get_xyz().x, quadVertecies[5]->get_xyz().y, quadVertecies[5]->get_xyz().z);
        vertexB = Vec3(quadVertecies[4]->get_xyz().x, quadVertecies[4]->get_xyz().y, quadVertecies[4]->get_xyz().z);
        vertexC = Vec3(quadVertecies[3]->get_xyz().x, quadVertecies[3]->get_xyz().y, quadVertecies[3]->get_xyz().z);

        objectHeight = ((quadVertecies[5]->get_xyz().y * triangle02.x) + (quadVertecies[4]->get_xyz().y * triangle02.y) + (quadVertecies[3]->get_xyz().y * triangle02.z));
    }

    return objectHeight;
}

void graph::makeSmooth()
{
    gs2019::Vector3D Normal(0.f, 0.f, 0.f);
    gs2019::Vector3D currentVertPos(0.f, 0.f, 0.f);

    // Inefficient algorithm, find a better solution.
    for (auto it = mVertices.begin(); it != mVertices.end(); it++)
    {
        currentVertPos = (*it).get_xyz();

        for (auto it2 = mVertices.begin(); it2 != mVertices.end(); it2++)
        {
            if ((it2->get_xyz().x == currentVertPos.x) && (it2->get_xyz().y == currentVertPos.y) && (it2->get_xyz().z == currentVertPos.z))
            {
                Vertex* temp = new Vertex(*it2);
                vertexMatch.push_back(temp);
            }
        }

        for (auto it3 = vertexMatch.begin(); it3 != vertexMatch.end(); it3++)
        {
            Normal = Normal + (*it3)->get_rgb();
        }

        Normal.normalize();
        (*it).set_rgb(Normal.x, Normal.y, Normal.z);
        Normal = gs2019::Vector3D(0.f, 0.f, 0.f);
        vertexMatch.clear();
    }
}

void graph::init()
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

    // Element buffer object
    glGenBuffers(1, &mEAB);
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, mEAB );
    glBufferData( GL_ELEMENT_ARRAY_BUFFER, mIndices.size() * sizeof( GLuint ), &mIndices[0], GL_STATIC_DRAW );

    glBindVertexArray(0);
}

void graph::draw()
{
    //actual draw call
    //setContext(context);

    glUseProgram(mMaterial.mShader->getProgram());
    glBindVertexArray(mVAO);
    mMaterial.mShader->transmitUniformData(&mMatrix, &mMaterial);

    //glDrawArrays(GL_TRIANGLES, 0, signed(mVertices.size()));
    glDrawElements(GL_TRIANGLES, mIndices.size(),GL_UNSIGNED_INT, nullptr);
}
