#include "stbloader.h"

#include <iostream>
using std::cout;
using std::cerr;
using std::endl;

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

GLubyte* STBLoader::load(const char *filename, GLint &width, GLint &height, int force_channels, unsigned int flags) {

    int channels;
    FILE *pFile = NULL;

    int fileOpenMode = ESFileWrapper::FOPEN_DEFAULT_MODE;
    if (flags & FLAG_FOPEN_ABSOULT){
        fileOpenMode = ESFileWrapper::FOPEN_ABSOLUTE_MODE;
    }
    //http://www.50ply.com/blog/2013/01/19/loading-compressed-android-assets-with-file-pointer/
    pFile = ESFileWrapper::esFopen(filename, "r", fileOpenMode);

    GLubyte *buffer = stbi_load_from_file(pFile, &width, &height, &channels, force_channels);
    fclose(pFile);

    // It has to do with OpenGL texture coordinates and the origin (0,0) of the image.
    // OpenGL treats the origin of a texture as the lower left corner, whereas most graphics
    // programs that you work with treat the origin as the upper left corner. While most file
    // formats support both origin types (upper left and lower left), some formats commonly
    // use one type over the other.
    // Notice that BMP start at lower left, the same as GL does, but it has been flip
    // by stb_image automatically. So basically we should flip all of the image except for cubemaps.
    if (flags & FLAG_INVERT_Y) {
        GLubyte *nBuffer = flipImage(buffer, width, height, channels);
        delete[] buffer;
        return nBuffer;
    } else {
        return buffer;
    }
}

GLuint STBLoader::loadTex(const char* fName, GLint & width, GLint &height, int force_channels, unsigned int flags) {

    GLubyte *data = STBLoader::load(fName, width, height, force_channels, flags);

    if (data != NULL) {
        GLuint texID;
        glGenTextures(1, &texID);
        // All upcoming GL_TEXTURE_2D operations now have effect on our texture object
        glBindTexture(GL_TEXTURE_2D, texID);
        //This line hard code to GL_RGB, so 16 or 32 bit BMP (GL_RGBA mode)
        //is not ready for support
        glTexImage2D(GL_TEXTURE_2D, 0,  force_channels == CHANNEL_RGBA ? GL_RGBA : GL_RGB,
                width, height, 0,force_channels == CHANNEL_RGBA ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, data);
        // Set our texture parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, force_channels == CHANNEL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT );
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, force_channels == CHANNEL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT );
        // Set texture filtering
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        if (flags & FLAG_MIPMAPS) {
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glGenerateMipmap(GL_TEXTURE_2D);
        } else {
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        }
        // Unbind texture when done, so we won't accidentily mess up our texture.
        glBindTexture(GL_TEXTURE_2D, 0);
        delete[] data;

        return texID;
    }

    return 0;
}

GLuint STBLoader::loadTex( const char * fileName, int force_channels, unsigned int flags) {
    GLint w, h;
    return STBLoader::loadTex(fileName, w, h, force_channels, flags);
}

GLuint STBLoader::loadCubemap(vector<const GLchar*> faces, int force_channels, unsigned int flags) {
    GLuint textureID;
    glGenTextures(1, &textureID);

    GLint width, height;
    GLubyte* image;

    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
    for(GLuint i = 0; i < faces.size(); i++) {
        image = STBLoader::load(faces[i], width, height, force_channels, flags);
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
        delete[] image;
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

    return textureID;
}

GLubyte *STBLoader::flipImage(GLubyte *data, GLint width, GLint height, GLint channels) {
    /*  create a copy the image data    */
    GLubyte *img = (GLubyte *)malloc( width*height*channels );
    memcpy( img, data, width*height*channels );

    int i, j;
    for( j = 0; j*2 < height; ++j )
    {
        int index1 = j * width * channels;
        int index2 = (height - 1 - j) * width * channels;
        for( i = width * channels; i > 0; --i ) {
            GLubyte temp = img[index1];
            img[index1] = img[index2];
            img[index2] = temp;
            ++index1;
            ++index2;
        }
    }
    return img;
}
