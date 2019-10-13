#ifndef GRAPH_H
#define GRAPH_H

#include "visualobject.h"
#include <cmath>
#include <vector>

typedef gs2019::Vector3D Vec3;
typedef gs2019::Vector2D Vec2;

class graph : public VisualObject
{
public:
    graph(bool itDoesNothing);
    graph(int subdivisions, float sizeX , float sizeY, float sizeZ, float inHeightOffset);

    //float applyHeightFunctionOLD(float inX, float inY, float inZ);
    void applyHeightFunction();

    float findTargetHeight(gs2019::Vector3D location);

    float findQuadLocation(gs2019::Vector3D location);
    float findTriangle(int x, int z, gs2019::Vector2D targetLocation);

    // Triangle search using indices.
    void locateNeighbours();

    int findTriangleIndices(gs2019::Vector2D targetLocation);
    int findTriangleStage2(gs2019::Vector2D targetLocation, int triangleID);

    std::vector<int> mNeighbour;

    //=========================================

    void makeSmooth();

    void init() override;
    void draw() override;

    float mSizeX{0.f};
    float mSizeZ{0.f};
    int mSubdivisions{0};

    // ======= PHYSICS =======
    Vec3 vertexA{0.f, 0.f, 0.f};
    Vec3 vertexB{0.f, 0.f, 0.f};
    Vec3 vertexC{0.f, 0.f, 0.f};

    float heightCalculation{0.f};

    void loadLASData(std::string filnavn);

    std::vector<Vec3> mMapPoints;

    void buildGrid(int subdivisions, float sizeX, float sizeY, float sizeZ, float inHeightOffset);

private:
    float heightOffset;

    std::vector<Vertex*> vertexMatch;
};

#endif // GRAPH_H
