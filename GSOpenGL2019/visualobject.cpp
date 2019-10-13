#include "visualobject.h"

VisualObject::VisualObject(){}

VisualObject::~VisualObject(){}

void VisualObject::setShader(Shader *shader)
{
    mMaterial.mShader = shader;
}

std::vector<Vertex> VisualObject::get_mVertecies()
{
    return mVertices;
}
