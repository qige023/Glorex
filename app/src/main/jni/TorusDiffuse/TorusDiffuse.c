//
// TorusDiffuse.c
//
#include <stdlib.h>
#include "esUtil.h"

typedef struct {
	// Handle to a program object
	GLuint programObject;

	// Uniform locations
	GLint mvpLoc;
	GLint mvLoc;
	GLint normalLoc;
	GLint projectionLoc;

	// Vertex data
	GLfloat *vertices;
	GLfloat *normals;
	GLfloat *texCoords;
	GLuint *indices;
	int numIndices;

	// Texture handle
	GLuint textureId;

	// Rotation angle
	GLfloat angle;

	// MVP matrix
	ESMatrix mvpMatrix;

	ESMatrix mvMatrix;

	ESMatrix projectionMatrix;

	ESNormalMatrix normalMatrix;
} UserData;

///
// Initialize the shader and program object
//
int Init(ESContext *esContext) {
	UserData *userData = esContext->userData;
	const char vShaderStr[] =
			"#version 300 es                                                                                \n"
					"layout (location = 0) in vec3 VertexPosition;                  \n"
					"layout (location = 2) in vec3 VertexNormal;                   \n"
					"out vec3 LightIntensity;                                                        \n"
					"const vec4 LightPosition =vec4(4.0f, 4.0f, 2.0f, 1.0f);    \n"
					"const vec3 Kd=vec3(0.9f, 0.5f, 0.3f);                                   \n"
					"const vec3 Ld=vec3(1.0f, 1.0f, 1.0f);                                   \n"
					"uniform mat4 ModelViewMatrix;                                      \n"
					"uniform mat3 NormalMatrix;                                             \n"
					"uniform mat4 ProjectionMatrix;                                        \n"
					"uniform mat4 MVP;                                                                \n"
					"void main()                                                                                 \n"
					"{                                                                                                      \n"
					"   vec3 tnorm = normalize( NormalMatrix * VertexNormal);                \n"
					"   vec4 eyeCoords = ModelViewMatrix * vec4(VertexPosition,1.0);    \n"
					"   vec3 s = normalize(vec3(LightPosition - eyeCoords));                          \n"
					"   LightIntensity = Ld * Kd * max( dot( s, tnorm ), 0.0 );                         \n"
					"   gl_Position = MVP * vec4(VertexPosition,1.0);                                       \n"
					"}                                                                                                                                 \n";

	const char fShaderStr[] =
			"#version 300 es                                                                    \n"
					"precision medium                                                       \n"
					"in vec3 LightIntensity;                                               \n"
					"layout(location = 0) out vec4 outColor;               \n"
					"void main()                                                                     \n"
					"{                                                                                         \n"
					"  outColor = vec4(LightIntensity, 1.0);                  \n"
					"}                                                                                         \n";

	// Load the shaders and get a linked program object
	userData->programObject = esLoadProgram(vShaderStr, fShaderStr);

	// Get the uniform locations
	userData->mvpLoc = glGetUniformLocation(userData->programObject, "MVP");

	// Get the uniform locations
	userData->mvLoc = glGetUniformLocation(userData->programObject,
			"ModelViewMatrix");

	// Get the uniform locations
	userData->normalLoc = glGetUniformLocation(userData->programObject,
			"NormalMatrix");

	userData->projectionLoc = glGetUniformLocation(userData->programObject,
			"ProjectionMatrix");

	// Load the texture
	//userData->textureId = CreateSimpleTextureCubemap ();
	// Generate the vertex data
	userData->numIndices = esGenTorus(&userData->vertices, &userData->normals,
			&userData->texCoords, &userData->indices, 0.5f, 0.2f, 40, 40);

	// Starting rotation angle for the cube
	userData->angle = 45.0f;

	// Set backgroundColor
	glClearColor(0.2f, 0.2f, 0.2f, 0.0f);
	return GL_TRUE;
}

///
// Update MVP matrix based on time
//
void Update(ESContext *esContext, float deltaTime) {
	UserData *userData = esContext->userData;
	float aspect;

	// Compute a rotation angle based on time to rotate the cube
	userData->angle += (deltaTime * 40.0f);

	if (userData->angle >= 360.0f) {
		userData->angle -= 360.0f;
	}

	// Compute the window aspect ratio
	aspect = (GLfloat) esContext->width / (GLfloat) esContext->height;

	// Generate a perspective matrix with a 60 degree FOV
	esMatrixLoadIdentity(&userData->projectionMatrix);
	esPerspective(&userData->projectionMatrix, 60.0f, aspect, 1.0f, 20.0f);

	// Generate a model view matrix to rotate/translate the cube
	esMatrixLoadIdentity(&userData->mvMatrix);

	// Translate away from the viewer
	esTranslate(&userData->mvMatrix, 0.0, 0.0, -2.0);

	// Rotate the cube
	esRotate(&userData->mvMatrix, userData->angle, 1.0, 0.0, 1.0);

	esCopyToNormalMatrix(&userData->normalMatrix, &userData->mvMatrix);

	// Compute the final MVP by multiplying the
	// modevleiw and perspective matrices together
	esMatrixMultiply(&userData->mvpMatrix, &userData->mvMatrix,
			&userData->projectionMatrix);
}

///
// Draw a triangle using the shader pair created in Init()
//
void Draw(ESContext *esContext) {
	UserData *userData = esContext->userData;

	// Set the viewport
	glViewport(0, 0, esContext->width, esContext->height);

	// Clear the color buffer
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Set depth's clear-value to farthest
	glClearDepthf(1.0f);

	// Enables depth-buffer for hidden surface removal
	glEnable(GL_DEPTH_TEST);

	// The type of depth testing to do
	glDepthFunc(GL_LEQUAL);

	glEnable(GL_TEXTURE_2D);

	// Use the program object
	glUseProgram(userData->programObject);

	// Load the vertex position
	glVertexAttribPointer(0, 3, GL_FLOAT,
	GL_FALSE, 3 * sizeof(GLfloat), userData->vertices);

	// Load the texture coordinate
	glVertexAttribPointer(1, 2, GL_FLOAT,
	GL_FALSE, 2 * sizeof(GLfloat), userData->texCoords);

	// Load the normal
	glVertexAttribPointer(2, 3, GL_FLOAT,
	GL_FALSE, 3 * sizeof(GLfloat), userData->normals);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	// Load the MVP matrix
	glUniformMatrix4fv(userData->mvpLoc, 1, GL_FALSE,
			(GLfloat *) &userData->mvpMatrix.m[0][0]);

	// Load the MV matrix
	glUniformMatrix4fv(userData->mvLoc, 1, GL_FALSE,
			(GLfloat *) &userData->mvMatrix.m[0][0]);

	// Load the normal matrix
	glUniformMatrix3fv(userData->normalLoc, 1, GL_FALSE,
			(GLfloat *) &userData->normalMatrix.m[0][0]);

	glUniformMatrix4fv(userData->projectionLoc, 1, GL_FALSE,
			(GLfloat *) &userData->projectionMatrix.m[0][0]);

	// Draw the torus
	glDrawElements( GL_TRIANGLES, userData->numIndices, GL_UNSIGNED_INT,
			userData->indices);
}

///
// Cleanup
//
void Shutdown(ESContext *esContext) {
	UserData *userData = esContext->userData;

	// Delete texture object
	//glDeleteTextures ( 1, &userData->textureId );

	if (userData->vertices != NULL) {
		free(userData->vertices);
	}

	if (userData->indices != NULL) {
		free(userData->indices);
	}

	free(userData->normals);

	free(userData->texCoords);

	// Delete program object
	glDeleteProgram(userData->programObject);
}

int esMain(ESContext *esContext) {
	esContext->userData = malloc(sizeof(UserData));

	esCreateWindow(esContext, "Simple VertexShader", 320, 240,
			ES_WINDOW_RGB | ES_WINDOW_DEPTH);

	if (!Init(esContext)) {
		return GL_FALSE;
	}

	esRegisterShutdownFunc(esContext, Shutdown);
	esRegisterUpdateFunc(esContext, Update);
	esRegisterDrawFunc(esContext, Draw);

	return GL_TRUE;
}

