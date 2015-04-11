// The MIT License (MIT)
//
// Copyright (c) 2013 Dan Ginsburg, Budirijanto Purnomo
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

//
// Book:      OpenGL(R) ES 3.0 Programming Guide, 2nd Edition
// Authors:   Dan Ginsburg, Budirijanto Purnomo, Dave Shreiner, Aaftab Munshi
// ISBN-10:   0-321-93388-5
// ISBN-13:   978-0-321-93388-1
// Publisher: Addison-Wesley Professional
// URLs:      http://www.opengles-book.com
//            http://my.safaribooksonline.com/book/animation-and-3d/9780133440133
//
// Simple_VertexShader.c
//
//    This is a simple example that draws a rotating cube in perspective
//    using a vertex shader to transform the object
//
#include <stdlib.h>
#include "esUtil.h"

typedef struct
{
   // Handle to a program object
   GLuint programObject;

   // Uniform locations
   GLint  mvpLoc;

   // Vertex data
   GLfloat  *vertices;
   GLfloat  *normals;
   GLfloat  *texCoords;
   GLuint   *indices;
   int       numIndices;

   // Sampler location
   GLint samplerLoc;

   // Texture handle
   GLuint textureId;

   // Rotation angle
   GLfloat   angle;

   // MVP matrix
   ESMatrix  mvpMatrix;
} UserData;

///
// Create a simple cubemap with a 1x1 face with a different
// color for each face
GLuint CreateSimpleTextureCubemap( )
{
   GLuint textureId;
   // Six 1x1 RGB faces
   GLubyte cubePixels[6 * 3] =
   {
      // Face 0 - Red
      255, 0, 0,
      // Face 1 - Green,
      0, 255, 0,
      // Face 2 - Blue
      0, 0, 255,
      // Face 3 - Yellow
      255, 255, 0,
      // Face 4 - Purple
      255, 0, 255,
      // Face 5 - White
      255, 255, 255
   };

   // Use tightly packed data
   glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

   // Generate a texture object
   glGenTextures ( 1, &textureId );

   // Bind the texture object
   glBindTexture ( GL_TEXTURE_2D, textureId );

   // Load the cube face
   glTexImage2D ( GL_TEXTURE_2D, 0, GL_RGB, 1, 1, 0,
                  GL_RGB, GL_UNSIGNED_BYTE, cubePixels );

   // Set the filtering mode
   glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
   glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
   glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
   glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );

//   // load up a texture
//   textureId=loadTexture(@"texture.png", -1, -1);

   return textureId;

}

///
// Initialize the shader and program object
//
int Init ( ESContext *esContext )
{
   UserData *userData = esContext->userData;
   const char vShaderStr[] =
      "#version 300 es                             \n"
      "uniform mat4 u_mvpMatrix;                   \n"
      "layout(location = 0) in vec4 a_position;    \n"
      "layout(location = 1) in vec2 a_texCoord;    \n"
      "out vec2 v_texCoord;                        \n"
      "out vec4 color;                             \n"
      "void main()                                 \n"
      "{                                           \n"
      "   gl_Position = u_mvpMatrix * a_position;  \n"
      "   v_texCoord = a_texCoord;                 \n"
      "   color = a_position;                      \n"
      "}                                           \n";

   const char fShaderStr[] =
      "#version 300 es                                \n"
      "precision mediump float;                       \n"
      "in vec2 v_texCoord;                            \n"
      "in vec4 color;                                 \n"
      "layout(location = 0) out vec4 outColor;        \n"
      "void main()                                    \n"
      "{                                              \n"
      "  outColor = color;                            \n"
      "}                                              \n";

   // Load the shaders and get a linked program object
   userData->programObject = esLoadProgram ( vShaderStr, fShaderStr );

   // Get the uniform locations
   userData->mvpLoc = glGetUniformLocation ( userData->programObject, "u_mvpMatrix" );

   // Get the sampler locations
   userData->samplerLoc = glGetUniformLocation ( userData->programObject, "s_texture" );

   // Load the texture
   //userData->textureId = CreateSimpleTextureCubemap ();
   // Generate the vertex data
   userData->numIndices = esGenTorus (&userData->vertices, &userData->normals,
                                &userData->texCoords, &userData->indices,
                                0.5f, 0.2f, 40, 40);

   // Starting rotation angle for the cube
   userData->angle = 45.0f;

   glClearColor ( 1.0f, 1.0f, 1.0f, 0.0f );
   return GL_TRUE;
}


///
// Update MVP matrix based on time
//
void Update ( ESContext *esContext, float deltaTime )
{
   UserData *userData = esContext->userData;
   ESMatrix perspective;
   ESMatrix modelview;
   float    aspect;

   // Compute a rotation angle based on time to rotate the cube
   userData->angle += ( deltaTime * 40.0f );

   if ( userData->angle >= 360.0f )
   {
      userData->angle -= 360.0f;
   }

   // Compute the window aspect ratio
   aspect = ( GLfloat ) esContext->width / ( GLfloat ) esContext->height;

   // Generate a perspective matrix with a 60 degree FOV
   esMatrixLoadIdentity ( &perspective );
   esPerspective ( &perspective, 60.0f, aspect, 1.0f, 20.0f );

   // Generate a model view matrix to rotate/translate the cube
   esMatrixLoadIdentity ( &modelview );

   // Translate away from the viewer
   esTranslate ( &modelview, 0.0, 0.0, -2.0 );

   // Rotate the cube
   esRotate ( &modelview, userData->angle, 1.0, 0.0, 1.0 );

   // Compute the final MVP by multiplying the
   // modevleiw and perspective matrices together
   esMatrixMultiply ( &userData->mvpMatrix, &modelview, &perspective );
}

///
// Draw a triangle using the shader pair created in Init()
//
void Draw ( ESContext *esContext )
{
   UserData *userData = esContext->userData;

   // Set the viewport
   glViewport ( 0, 0, esContext->width, esContext->height );

   // Clear the color buffer
   glClear ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

   // Set depth's clear-value to farthest
   glClearDepthf(1.0f);

   // Enables depth-buffer for hidden surface removal
   glEnable(GL_DEPTH_TEST);

   // The type of depth testing to do
   glDepthFunc(GL_LEQUAL);

   // Enable smooth shading of color
//   glShadeModel(GL_SMOOTH);

   // Disable dithering for better performance
//   glDisable(GL_DITHER);

   glEnable(GL_TEXTURE_2D);

   // Use the program object
   glUseProgram ( userData->programObject );

   // Load the vertex position
   glVertexAttribPointer ( 0, 3, GL_FLOAT,
                           GL_FALSE, 3 * sizeof ( GLfloat ), userData->vertices );

   // Load the texture coordinate
   glVertexAttribPointer ( 1, 2, GL_FLOAT,
                          GL_FALSE, 2 * sizeof ( GLfloat ), userData->texCoords );

   // Load the normal
   glVertexAttribPointer ( 2, 3, GL_FLOAT,
                          GL_FALSE, 3 * sizeof ( GLfloat ),  userData->normals );

   glEnableVertexAttribArray ( 0 );
   glEnableVertexAttribArray ( 1 );
   glEnableVertexAttribArray ( 2 );

   // Set the vertex color to red
   // glVertexAttrib4f ( 1, 1.0f, 0.0f, 0.0f, 1.0f );


   // Bind the texture
   //glActiveTexture ( GL_TEXTURE0 );
   //glBindTexture ( GL_TEXTURE_2D, userData->textureId );

   // Load the MVP matrix
   glUniformMatrix4fv ( userData->mvpLoc, 1, GL_FALSE, ( GLfloat * ) &userData->mvpMatrix.m[0][0] );

   // Set the sampler texture unit to 0
   glUniform1i ( userData->samplerLoc, 0 );

   // Draw the cube
   glDrawElements ( GL_TRIANGLES, userData->numIndices, GL_UNSIGNED_INT, userData->indices );
}

///
// Cleanup
//
void Shutdown ( ESContext *esContext )
{
   UserData *userData = esContext->userData;

   // Delete texture object
   //glDeleteTextures ( 1, &userData->textureId );

   if ( userData->vertices != NULL )
   {
      free ( userData->vertices );
   }

   if ( userData->indices != NULL )
   {
      free ( userData->indices );
   }

   free ( userData->normals );

   free ( userData->texCoords );

   // Delete program object
   glDeleteProgram ( userData->programObject );
}


int esMain ( ESContext *esContext )
{
   esContext->userData = malloc ( sizeof ( UserData ) );

   esCreateWindow ( esContext, "Simple VertexShader", 320, 240, ES_WINDOW_RGB | ES_WINDOW_DEPTH );

   if ( !Init ( esContext ) )
   {
      return GL_FALSE;
   }

   esRegisterShutdownFunc ( esContext, Shutdown );
   esRegisterUpdateFunc ( esContext, Update );
   esRegisterDrawFunc ( esContext, Draw );

   return GL_TRUE;
}

