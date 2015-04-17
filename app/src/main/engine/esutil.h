/// \file esutil.h
/// \brief A utility library for OpenGL ES.
//
#ifndef ESUTIL_H
#define ESUTIL_H

///
//  Includes
//
#include <stdlib.h>
#include "escontext.h"
#include "esscene.h"

///
//  Macros
//
#define ESUTIL_API
#define ESCALLBACK


/// esCreateWindow flag - RGB color buffer
#define ES_WINDOW_RGB           0
/// esCreateWindow flag - ALPHA color buffer
#define ES_WINDOW_ALPHA         1
/// esCreateWindow flag - depth buffer
#define ES_WINDOW_DEPTH         2
/// esCreateWindow flag - stencil buffer
#define ES_WINDOW_STENCIL       4
/// esCreateWindow flat - multi-sample buffer
#define ES_WINDOW_MULTISAMPLE   8


///
// Types
//
#ifndef FALSE
#define FALSE 0
#endif
#ifndef TRUE
#define TRUE 1
#endif

///
//  Public Functions
//

//
/// \brief Create a window with the specified parameters
/// \param esContext Application context
/// \param title Name for title bar of window
/// \param width Width in pixels of window to create
/// \param height Height in pixels of window to create
/// \param flags Bitfield for the window creation flags
///         ES_WINDOW_RGB     - specifies that the color buffer should have R,G,B channels
///         ES_WINDOW_ALPHA   - specifies that the color buffer should have alpha
///         ES_WINDOW_DEPTH   - specifies that a depth buffer should be created
///         ES_WINDOW_STENCIL - specifies that a stencil buffer should be created
///         ES_WINDOW_MULTISAMPLE - specifies that a multi-sample buffer should be created
/// \return GL_TRUE if window creation is succesful, GL_FALSE otherwise
GLboolean ESUTIL_API esCreateWindow ( ESContext *esContext, const char *title, GLint width, GLint height, GLuint flags );

//
/// \brief Log a message to the debug output for the platform
/// \param formatStr Format string for error log.
//
void ESUTIL_API esLogMessage ( const char *formatStr, ... );

//
/// \brief Generates geometry for a sphere.  Allocates memory for the vertex data and stores
///        the results in the arrays.  Generate index list for a TRIANGLE_STRIP
/// \param numSlices The number of slices in the sphere
/// \param vertices If not NULL, will contain array of float3 positions
/// \param normals If not NULL, will contain array of float3 normals
/// \param texCoords If not NULL, will contain array of float2 texCoords
/// \param indices If not NULL, will contain the array of indices for the triangle strip
/// \return The number of indices required for rendering the buffers (the number of indices stored in the indices array
///         if it is not NULL ) as a GL_TRIANGLE_STRIP
//
int ESUTIL_API esGenSphere ( int numSlices, float radius, GLfloat **vertices, GLfloat **normals,
                             GLfloat **texCoords, GLuint **indices );

//
/// \brief Generates geometry for a cube.  Allocates memory for the vertex data and stores
///        the results in the arrays.  Generate index list for a TRIANGLES
/// \param scale The size of the cube, use 1.0 for a unit cube.
/// \param vertices If not NULL, will contain array of float3 positions
/// \param normals If not NULL, will contain array of float3 normals
/// \param texCoords If not NULL, will contain array of float2 texCoords
/// \param indices If not NULL, will contain the array of indices for the triangle strip
/// \return The number of indices required for rendering the buffers (the number of indices stored in the indices array
///         if it is not NULL ) as a GL_TRIANGLES
//
int ESUTIL_API esGenCube ( float scale, GLfloat **vertices, GLfloat **normals,
                           GLfloat **texCoords, GLuint **indices );

//
/// \brief Generates a square grid consisting of triangles.  Allocates memory for the vertex data and stores
///        the results in the arrays.  Generate index list as TRIANGLES.
/// \param size create a grid of size by size (number of triangles = (size-1)*(size-1)*2)
/// \param vertices If not NULL, will contain array of float3 positions
/// \param indices If not NULL, will contain the array of indices for the triangle strip
/// \return The number of indices required for rendering the buffers (the number of indices stored in the indices array
///         if it is not NULL ) as a GL_TRIANGLES
//
int ESUTIL_API esGenSquareGrid ( int size, GLfloat **vertices, GLuint **indices );

int ESUTIL_API esGenTorus ( GLfloat **vertices, GLfloat **normals, GLfloat **texCoords,
                    GLuint **indices, float outerRadius, float innerRadius, int rings, int sides );

//
/// \brief Loads a 8-bit, 24-bit or 32-bit TGA image from a file
/// \param ioContext Context related to IO facility on the platform
/// \param fileName Name of the file on disk
/// \param width Width of loaded image in pixels
/// \param height Height of loaded image in pixels
///  \return Pointer to loaded image.  NULL on failure.
//
char *ESUTIL_API esLoadTGA ( void *ioContext, const char *fileName, int *width, int *height );


#endif // ESUTIL_H
