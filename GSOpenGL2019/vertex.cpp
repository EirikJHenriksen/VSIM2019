#include "vertex.h"

Vertex::Vertex()
{
    xyz.x = 0;
    xyz.y = 0;
    xyz.z = 0;

    normal.x = 0;
    normal.y = 0;
    normal.z = 0;

    uv.x = 0;
    uv.y = 0;
}

Vertex::Vertex(float inX, float inY, float inZ, float inR, float inG, float inB, float inU, float inV)
{
    xyz.x = inX;
    xyz.y = inY;
    xyz.z = inZ;

    normal.x = inR;
    normal.y = inG;
    normal.z = inB;

    uv.x = inU;
    uv.y = inV;
}

Vertex::Vertex(gs2019::Vector3D inXyz, gs2019::Vector3D inNormal, gs2019::Vector2D inUv)
{
    xyz = inXyz;
    normal = inNormal;
    uv = inUv;
}

std::ostream& operator<< (std::ostream& os, const Vertex& v)
{
    os << std::fixed;
    os << "(" << v.xyz.x << ", " << v.xyz.y << ", " << v.xyz.z << ") ";
    os << "(" << v.normal.x << ", " << v.normal.y << ", " << v.normal.z << ") ";
    os << "(" << v.uv.x << ", " << v.uv.y << ") ";
    return os;
}

std::istream& operator>> (std::istream& is, Vertex& v)
{
    // Trenger fire temporære variabler som kun skal lese inn parenteser og komma
    char dum, dum2, dum3, dum4;
    is >> dum >> v.xyz.x >> dum2 >> v.xyz.y >> dum3 >> v.xyz.z >> dum4;
    is >> dum >> v.normal.x >> dum2 >> v.normal.y >> dum3 >> v.normal.z >> dum4;
    is >> dum >> v.uv.x >> dum2 >> v.uv.y >> dum3;
    return is;
}

void Vertex::set_xyz(float inX, float inY, float inZ)
{
    xyz.x = inX;
    xyz.y = inY;
    xyz.z = inZ;

    //uv.x = 0;
    //uv.y = 0;
}

void Vertex::set_rgb(float inR, float inG, float inB)
{
    normal.x = inR;
    normal.y = inG;
    normal.z = inB;
}

void Vertex::set_uv(float inU, float inV)
{
    uv.x = inU;
    uv.y = inV;
}

gs2019::Vector3D Vertex::get_xyz()
{
    return xyz;
}

gs2019::Vector2D Vertex::get_uv()
{
    return uv;
}

gs2019::Vector3D Vertex::get_rgb()
{
    return normal;
}
