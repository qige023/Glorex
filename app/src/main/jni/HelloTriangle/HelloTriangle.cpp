#include "HelloTriangle.h"

#include "esutil.h"

#include <cstdio>
#include <cstdlib>
#include <iostream>

using std::cout;
using std::cerr;
using std::endl;

using glm::vec3;
using glm::vec4;

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/string_cast.hpp>

HelloTriangle::HelloTriangle() { }

HelloTriangle::~HelloTriangle() { }

void HelloTriangle::compileAndLinkShader() {
    cout << "exec HelloTriangle::compileAndLinkShader" << endl;
    try {
        prog.compileShader("shader/basic.vert");
        prog.compileShader("shader/basic.frag");
        prog.link();
        prog.validate();
        prog.use();
    } catch (ESProgramException & e) {
        cerr << e.what() << endl;
        exit( EXIT_FAILURE);
    }
}

void HelloTriangle::initScene(ESContext *esContext) {

    compileAndLinkShader();
    /////////////////// Create the VBO ////////////////////
    float positionData[] = {
        -0.8f, -0.8f, 0.0f,
         0.8f, -0.8f, 0.0f,
         0.0f,  0.8f, 0.0f
    };

    float colorData[] = {
        1.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 1.0f
    };

    // Create and populate the buffer objects
    GLuint vboHandles[2];
    glGenBuffers(2, vboHandles);
    GLuint positionBufferHandle = vboHandles[0];
    GLuint colorBufferHandle = vboHandles[1];

    glBindBuffer(GL_ARRAY_BUFFER, positionBufferHandle);
    glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(float), positionData, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, colorBufferHandle);
    glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(float), colorData, GL_STATIC_DRAW);

    // Create and set-up the vertex array object
    glGenVertexArrays( 1, &vaoHandle );
    glBindVertexArray(vaoHandle);

    glEnableVertexAttribArray(0);  // Vertex position
    glEnableVertexAttribArray(1);  // Vertex color

    glBindBuffer(GL_ARRAY_BUFFER, positionBufferHandle);
    glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte *)NULL );

    glBindBuffer(GL_ARRAY_BUFFER, colorBufferHandle);
    glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte *)NULL );

    glClearColor ( 1.0f, 1.0f, 1.0f, 0.0f );
}

void HelloTriangle::update(ESContext *esContext, float t )
{

}

void HelloTriangle::render(ESContext *esContext)
{
    glClear(GL_COLOR_BUFFER_BIT);
    prog.use();

    float aspect = (float) esContext->width / esContext->height;

    projection = mat4(1.0f);

    //make sure the whole triangle is in the scene and keep its ratio
    if (esContext->width > esContext-> height) {
        projection *= glm::ortho(-((float)esContext->width / esContext-> height)
                , ((float)esContext->width / esContext-> height), -1.0f, 1.0f, -1.0f, 1.0f);
    } else {
        projection *= glm::ortho(-1.0f, 1.0f, -((float)esContext->height / esContext-> width)
                , ((float)esContext->height / esContext-> width), -1.0f, 1.0f);
    }

    //we omit model and view matrix , they are by default mat4(1.0f)
    prog.setUniform("MVP", projection);

    glBindVertexArray(vaoHandle);

    glDrawArrays(GL_TRIANGLES, 0, 3 );
}

void HelloTriangle::resize(ESContext *esContext)
{
    glViewport(0,0,esContext->width,esContext->height);
}

ESScene *esCreateScene(ESContext *esContext) {
    cout << "exec esCreateScene -> create scene HelloTriangle" << endl;
    return new HelloTriangle();
}
