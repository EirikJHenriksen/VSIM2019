#ifndef VERTEX_H
#define VERTEX_H

#include <iostream>
#include "mathlab.h"

class Vertex
{
private:
    // =========== Overloaded funksjoner ===========
    //! Overloaded ostream operator which writes all vertex data on an open textfile stream
    friend std::ostream& operator<< (std::ostream&, const Vertex&);

    //! Overloaded ostream operator which reads all vertex data from an open textfile stream
    friend std::istream& operator>> (std::istream&, Vertex&);

    // =========== Variabler ===========
    gs2019::Vector3D xyz;
    gs2019::Vector3D normal;
    gs2019::Vector2D uv;

public:
    Vertex();
    Vertex(float inX, float inY, float inZ, float inR, float inG, float inB, float inU, float inV);
    Vertex(gs2019::Vector3D inXyz, gs2019::Vector3D inNormal, gs2019::Vector2D inUv);

    void set_xyz(float inX, float inY, float inZ);
    void set_rgb(float inR, float inG, float inB);
    void set_uv(float inU, float inV);

    gs2019::Vector3D get_xyz();
    gs2019::Vector2D get_uv();
    gs2019::Vector3D get_rgb();
};

#endif // VERTEX_H
