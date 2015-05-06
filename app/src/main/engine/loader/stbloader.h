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

	/**
	 * Loads a image file into an array suitable for loading into an
	 * OpenGL texture.  This method only supports 24 or 32 bpp images.
	 * The array that is returned should be deleted when one is finished
	 * with it.  Data is stored in the array as RGBA, 4 bytes per
	 * pixel.
	 * @param fileName the file name of the BMP file.
	 * @param width the width in pixels of the image is stored here.
	 * @param height the height in pixels of the image is stored here.
	 * @return a pointer to the image data
	 */
	static GLubyte *load( const char *fileName, GLint &width /*out*/, GLint &height /*out*/ ,
	        GLint &channels /*out*/ , GLint req_comp, int fOpenMode, GLboolean needFlip = TRUE);

	/**
	 * Loads a image into an OpenGL texture.  This method only supports
	 * 24 or 32 bpp images.
	 * @param fileName the file name of the BMP file.
	 * @param width the width in pixels of the image is stored here.
	 * @param height the height in pixels of the image is stored here.
	 * @return the texture ID
	 */
	static GLuint loadTex( const char * fileName, GLint &width /*out*/, GLint &height /*out*/, GLint &channels /*out*/ ,
	        GLboolean alpha, int fOpenMode);

	static GLuint loadTex( const char * fileName, GLboolean alpha = false,
	        int fOpenMode = ESFileWrapper::FOPEN_DEFAULT_MODE);
	// Loads a cubemap texture from 6 individual texture faces
	// Order should be:
	// +X (right)
	// -X (left)
	// +Y (top)
	// -Y (bottom)
	// +Z (front)
	// -Z (back)
	static GLuint loadCubemap(vector<const GLchar*> faces);

private:

    static  GLubyte * flipImage( GLubyte *data, GLint width, GLint height, GLint channels);

};

#endif
