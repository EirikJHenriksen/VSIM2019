#ifndef TEXTURE_H
#define TEXTURE_H

#include <QOpenGLFunctions_4_1_Core>
#include "stb_image.h"

class Texture : protected QOpenGLFunctions_4_1_Core
{
private:
    GLubyte pixels[16];
    GLuint mId{0};
    unsigned char *mBitmap;
    int mColumns;
    int mRows;
    int mnByte;
    void readImgmap(const std::string& filename, GLuint textureUnit);

public:
    //Texture(GLuint textureUnit = 0);  //basic texture from code
    Texture(const std::string &filename, GLuint textureUnit = 0);
    GLuint id() const;
};

#endif // TEXTURE_H
