#include "spriteshader.h"
#include "material.h"

SpriteShader::SpriteShader(const GLchar *vertexPath, const GLchar *fragmentPath, const GLchar *geometryPath) : Shader(vertexPath, fragmentPath, geometryPath)
{
    mMatrixUniform = glGetUniformLocation( program, "mMatrix" );
    vMatrixUniform = glGetUniformLocation( program, "vMatrix" );
    pMatrixUniform = glGetUniformLocation( program, "pMatrix" );
    objectColorUniform = glGetUniformLocation( program, "objectColor" );
    textureUniform = glGetUniformLocation(program, "textureSampler");

    billboardPosUniform = glGetUniformLocation(program, "billboardPosition");
    billboardScaleUniform = glGetUniformLocation(program, "billboardScale");
}

void SpriteShader::transmitUniformData(gs2019::Matrix4x4 *modelMatrix, Material *material)
{    
    Shader::transmitUniformData(modelMatrix); // <-- SHADEREN KRÆSJER HER. NÅR DEN KJØRER DENNE FUNKSJONEN OG SENDER INN "vMatrix"!!!!

    if(!mSprite)
    {
        qDebug() << "ERROR: No sprite/billboard set in SpriteShader.cpp";
    }

    glUniform1i(textureUniform, material->mTextureUnit); //TextureUnit = 0 as default);
    glUniform3f(objectColorUniform, material->mObjectColor.x, material->mObjectColor.y, material->mObjectColor.z);

    glUniform3f(billboardPosUniform, mSprite->BillboardPosition.x, mSprite->BillboardPosition.y, mSprite->BillboardPosition.z);
    glUniform3f(billboardScaleUniform, mSprite->BillboardScale.x, mSprite->BillboardScale.y, mSprite->BillboardScale.z);
}

void SpriteShader::setSprite(Billboard* inputBillboard)
{
    mSprite = inputBillboard;
}
