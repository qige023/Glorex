#include "stbloader.h"
#include "esfile.h"

#include <iostream>
using std::cout;
using std::cerr;
using std::endl;

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

GLubyte* STBLoader::load(const char *filename, GLint &width,
        GLint &height, GLint &channels, GLint req_comp, GLboolean needFlip) {

    FILE *pFile = NULL;

    //http://www.50ply.com/blog/2013/01/19/loading-compressed-android-assets-with-file-pointer/
    pFile = ESFileWrapper::esFopen(filename, "r");

    GLubyte *buffer = stbi_load_from_file(pFile, &width, &height, &channels, req_comp);
    fclose(pFile);

    if (needFlip) {
        //http://stackoverflow.com/questions/8346115/why-are-bmps-stored-upside-down
        GLubyte *nBuffer = flipImage(buffer, width, height, channels);
        delete[] buffer;
        return nBuffer;
    } else {
        return buffer;
    }
}

GLuint STBLoader::loadTex(const char* fName, GLint & width, GLint &height, GLint &chennels, GLboolean alpha) {

    GLubyte * data = STBLoader::load(fName, width, height, chennels, alpha ? 4 : 3);

    if (data != NULL) {
        GLuint texID;
        glGenTextures(1, &texID);
        // All upcoming GL_TEXTURE_2D operations now have effect on our texture object
        glBindTexture(GL_TEXTURE_2D, texID);
        //This line hard code to GL_RGB, so 16 or 32 bit BMP (GL_RGBA mode)
        //is not ready for support
        glTexImage2D(GL_TEXTURE_2D, 0,  alpha ? GL_RGBA : GL_RGB, width, height, 0, alpha ? GL_RGBA : GL_RGB,
                GL_UNSIGNED_BYTE, data);
        // Set our texture parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, alpha ? GL_CLAMP_TO_EDGE : GL_REPEAT );
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, alpha ? GL_CLAMP_TO_EDGE : GL_REPEAT );
        // Set texture filtering
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glGenerateMipmap(GL_TEXTURE_2D);
        // Unbind texture when done, so we won't accidentily mess up our texture.
        glBindTexture(GL_TEXTURE_2D, 0);
        delete[] data;

        return texID;
    }

    return 0;
}

GLuint STBLoader::loadTex(const char* fName, GLboolean alpha) {
    GLint w, h, c;
    return STBLoader::loadTex(fName, w, h, c, alpha);
}

GLuint STBLoader::loadCubemap(vector<const GLchar*> faces) {
    GLuint textureID;
    glGenTextures(1, &textureID);

    GLint width, height, chennels;
    GLubyte* image;

    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
    for(GLuint i = 0; i < faces.size(); i++) {
        image = STBLoader::load(faces[i], width, height, chennels, 3, false);
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
        for( i = width * channels; i > 0; --i )
        {
            GLubyte temp = img[index1];
            img[index1] = img[index2];
            img[index2] = temp;
            ++index1;
            ++index2;
        }
    }
    return img;
}
