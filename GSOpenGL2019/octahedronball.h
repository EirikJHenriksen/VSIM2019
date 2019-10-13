#ifndef OCTAHEDRONBALL_H
#define OCTAHEDRONBALL_H

#include "visualobject.h"
#include "mathlab.h"

#include "QElapsedTimer"

class Vertex;
typedef gs2019::Vector3D Vec3;
class OctahedronBall : public VisualObject
{
private:
    int m_rekursjoner;
    int m_indeks;               // brukes i rekursjon, til å bygge m_vertices
    void lagTriangel(const Vec3& v1, const Vec3& v2, const Vec3& v3);
    void subDivide(const Vec3& a, const Vec3& b, const Vec3& c, int n);
    void oktaederUnitBall();

public:
    OctahedronBall(std::string objectName, int n, gs2019::Vector3D inTrans, gs2019::Vector3D inRot, float rotAmount, gs2019::Vector3D inScale, bool contRot, bool bounce);
    ~OctahedronBall() override;
    void init() override;
    void draw() override;

    void move(float x, float y, float z, Vec3 surfNormal) override;
    void rotate(float amount, int x, int y, int z) override;
    void setHeight(float inY) override;
    void setLocation(float inX, float inY, float inZ) override;

    void collisionResponse(Vec3 input) override;

    void stopVelocity();
    void updateMatrix() override;

    gs2019::Vector3D getPosition() override;
    gs2019::Vector3D getRotation() override;
    float getRadius() override;

    // Check if the door is open (Remove when proper collision has been added).
    bool gateOpen = false;

    // ========= PHYSICS =============
    void resetPhysics();

    bool simulatePhysics{true};

    QElapsedTimer physicTimer;
    float lastTime{0.f};
    float deltaTime{0.f};

    const Vec3 Gravity{0.f, -9.81f, 0.f};

    Vec3 velocity{0.f, 0.f, 0.f};

    Vec3 VertexA{0.f, 0.f, 0.f};
    Vec3 VertexB{0.f, 0.f, 0.f};
    Vec3 VertexC{0.f, 0.f, 0.f};

    // =============================

private:
    // Limiting the movement of the sphere (Remove when proper collision has been added).
    //float minX = -10.f;
    //float minZ = -10.f;
    //float maxX = 10.f;
    //float maxZ = 10.f;

    // Real fake physics!
    bool wallBounce = true;
    float groundHeight;

    // Velocity variables
    bool simulateVelocity{false};
    float friction = 0.99f;

    Vec3 mVelocity{0.f, 0.f, 0.f};
    float vx = 0.f, vy = 0.f, vz = 0.f;

    float mx, my, mz; // position
    float mrx, mry, mrz; // rotation
};

#endif // OCTAHEDRONBALL_H
