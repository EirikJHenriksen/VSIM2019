#ifndef SPRITESHADER_H
#define SPRITESHADER_H

#include "shader.h"
#include "mathlab.h"
#include "material.h"

#include "billboard.h"

class SpriteShader : public Shader
{
public:
    SpriteShader(const GLchar *vertexPath, const GLchar *fragmentPath, const GLchar *geometryPath = nullptr);

    void transmitUniformData(gs2019::Matrix4x4 *modelMatrix, Material *material) override;

    void setSprite(Billboard *inputBillboard);

private:
    GLint objectColorUniform{-1};
    GLint textureUniform{-1};

    GLint billboardPosUniform{-1};
    GLint billboardScaleUniform{-1};

    class Billboard *mSprite = nullptr;
};

#endif // SPRITESHADER_H
