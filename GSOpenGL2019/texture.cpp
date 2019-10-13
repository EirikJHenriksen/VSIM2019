#include <QDebug>
#include <QImage>
#include <QBuffer>
#include <QByteArray>

#include <iostream>
#include <fstream>
#include <string>

#include "texture.h"

//Texture::Texture(GLuint textureUnit) : QOpenGLFunctions_4_1_Core()
//{
//    initializeOpenGLFunctions();
//    //small dummy texture
//    for (int i=0; i<16; i++)
//        pixels[i] = 0;
//    pixels[0] = 255;
//    pixels[4] = 255;
//    pixels[8] = 255;
//    pixels[9] = 255;
//    pixels[10] = 255;

//    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

//    glGenTextures(1, &mId);
//    glActiveTexture(GL_TEXTURE0);
//    glBindTexture(GL_TEXTURE_2D, mId);
//    qDebug() << "Texture::Texture() id = " << mId;
//    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 2, 2, 0, GL_RGB, GL_UNSIGNED_BYTE,
//                 reinterpret_cast<const GLvoid*>(pixels));
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

//    glGenTextures(1, &mId);
//    // activate the texture unit first before binding texture
//    glActiveTexture(GL_TEXTURE0 + textureUnit);
//    glBindTexture(GL_TEXTURE_2D, mId);
//    qDebug() << "Texture::Texture() id = " << mId;
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, mColumns, mRows, 0, GL_RGB, GL_UNSIGNED_BYTE, mBitmap);
//    glGenerateMipmap(GL_TEXTURE_2D);
//}

Texture::Texture(const std::string& filename, GLuint textureUnit): QOpenGLFunctions_4_1_Core()
{
    initializeOpenGLFunctions();
    readImgmap(filename, textureUnit);
}

GLuint Texture::id() const
{
    return mId;
}

void Texture::readImgmap(const std::string &filename, GLuint textureUnit)
{
    glGenTextures(1, &mId);
    glActiveTexture(GL_TEXTURE0 + textureUnit);
    glBindTexture(GL_TEXTURE_2D, mId); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load image, create texture and generate mipmaps
    int width, height, nrChannels;

    // FLIP TEXTURE
    stbi_set_flip_vertically_on_load(true);

    unsigned char *data = stbi_load(filename.data(), &width, &height, &nrChannels, 0);

    if (data)
    {
        //glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        qDebug() << "Successfully loaded texture:" << filename.data();
    }
    else
    {
        qDebug() << "FAILED to load texture: " << filename.data();
    }

    stbi_image_free(data);
}
