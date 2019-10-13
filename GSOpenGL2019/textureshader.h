#ifndef TEXTURESHADER_H
#define TEXTURESHADER_H

#include "shader.h"
#include "mathlab.h"
#include "material.h"

class TextureShader : public Shader
{
public:
    TextureShader(const GLchar *vertexPath, const GLchar *fragmentPath, const GLchar *geometryPath = nullptr);

    void transmitUniformData(gs2019::Matrix4x4 *modelMatrix, Material *material) override;

private:
    GLint objectColorUniform{-1};
    GLint textureUniform{-1};
};

#endif // TEXTURESHADER_H