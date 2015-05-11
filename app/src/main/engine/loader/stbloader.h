#ifndef BMPREADER_H_
#define BMPREADER_H_

#include "esutil.h"
#include "esfile.h"
#include <vector>
using std::vector;

/**
 * A Simple Wrapper for lib stb_image
 */
class STBLoader {
public:

    static const unsigned int FLAG_INVERT_Y = 1;
    static const unsigned int FLAG_POWER_OF_TWO = 2;
    static const unsigned int FLAG_MIPMAPS = 4;
    static const unsigned int FLAG_FOPEN_ABSOULT = 8;
    static const unsigned int DEFAULT_FLAG = FLAG_INVERT_Y | FLAG_MIPMAPS;

    static const int CHANNEL_RGB = 3;
    static const int CHANNEL_RGBA = 4;

	/**
	 * Loads a image file into an array suitable for loading into an
	 * OpenGL texture.  This method only supports 24 or 32 bpp images.
	 * The array that is returned should be deleted when one is finished
	 * with it.  Data is stored in the array as RGBA, 4 bytes per
	 * pixel.
	 * @param fileName the file name of the image file.
	 * @param width the width in pixels of the image is stored here.
	 * @param height the height in pixels of the image is stored here.
	 * @param force_channels 0-image format, 1-luminous, 2-luminous/alpha, 3-RGB, 4-RGBA
	 * @return a pointer to the image data
	 */
	static GLubyte *load( const char *fileName, GLint &width /*out*/, GLint &height /*out*/ , int force_channels,
	        unsigned int flags);

	/**
	 * Loads a image into an OpenGL texture.  This method only supports
	 * 24 or 32 bpp images.
	 * @param fileName the file name of the image file.
	 * @param width the width in pixels of the image is stored here.
	 * @param height the height in pixels of the image is stored here.
	 * @param force_channels 0-image format, 1-luminous, 2-luminous/alpha, 3-RGB, 4-RGBA
	 * @return the texture ID
	 */
	static GLuint loadTex( const char * fileName, GLint &width /*out*/, GLint &height /*out*/,
	        int force_channels = CHANNEL_RGB ,unsigned int flags = DEFAULT_FLAG);

	static GLuint loadTex( const char * fileName, int force_channels = CHANNEL_RGB, unsigned int flags = DEFAULT_FLAG);

	/** Loads a cubemap texture from 6 individual texture faces
	 * @param fileName the file name of the image file.
	 * Order should be:
	 * +X (right)
	 * -X (left)
	 * +Y (top)
	 * -Y (bottom)
	 * +Z (back)
	 * -Z (front)
	 */
	static GLuint loadCubemap(vector<const GLchar*> faces, int force_channels = CHANNEL_RGB, unsigned int flags = 0);

private:

    static  GLubyte *flipImage( GLubyte *data, GLint width, GLint height, GLint channels);

};

#endif
