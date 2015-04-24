#include "esutil.h"

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include "SimpleVertexShader.h"

using std::cout;
using std::cerr;
using std::endl;

using glm::vec3;
using glm::vec4;

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/string_cast.hpp>

SimpleVertexShader::SimpleVertexShader() {
}

SimpleVertexShader::~SimpleVertexShader() {
    delete cube;
}

void SimpleVertexShader::initScene(ESContext *esContext) {
    cout << "exec SimpleVertexShader::initScene" << endl;

    compileAndLinkShader();

    // Enables depth-buffer for hidden surface removal
    glEnable(GL_DEPTH_TEST);

    // The type of depth testing to do
    glDepthFunc(GL_LEQUAL);

    angle = 0.0f;
    cube = new VBOCube(1.0f);

    model = mat4(1.0f);
    model *= glm::rotate(glm::radians(angle), vec3(0.0f,1.0f,1.0f));
    cout << "Model Matrix" << glm::to_string(model) << endl;

    view = mat4(1.0f);
    view *= glm::lookAt(vec3(0.0f,0.0f,5.0f), vec3(0.0f,0.0f,0.0f), vec3(0.0f,1.0f,0.0f));
    cout << "View Matrix" << glm::to_string(view) << endl;

    projection = glm::perspective(glm::radians(45.0f), (float) esContext->width / esContext->height,
            0.1f, 100.0f);
    cout << "Projection Matrix" << glm::to_string(projection) << endl;

    prog.setUniform("SampleTexture", 0);

    // Set background color
    glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
}

void SimpleVertexShader::update(ESContext *esContext, float deltaTime) {

    // 40 degree per second
    angle += deltaTime * 40.0f;
    model = mat4(1.0f);
    model *= glm::rotate(glm::radians(angle), vec3(-1.0f,1.0f,0.0f));
}

void SimpleVertexShader::render(ESContext *esContext) {

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    setMatrices();

    cube->render();
}

void SimpleVertexShader::setMatrices() {
    prog.setUniform("MVP", projection * view * model);
}

void SimpleVertexShader::resize(ESContext *esContext) {
    glViewport(0, 0, esContext->width, esContext->height);
    projection = glm::perspective(glm::radians(45.0f), (float) esContext->width / esContext->height,
                0.1f, 100.0f);
}

void SimpleVertexShader::compileAndLinkShader() {
    cout << "exec SimpleVertexShader::compileAndLinkShader" << endl;
    try {
        prog.compileShader("shader/simplevertexshader.vert");
        prog.compileShader("shader/simplevertexshader.frag");
        prog.link();
        prog.validate();
        prog.use();
    } catch (ESProgramException & e) {
        cerr << e.what() << endl;
        exit( EXIT_FAILURE);
    }
}

ESScene *esCreateScene(ESContext *esContext) {
    cout << "exec esCreateScene -> create scene SimpleVertexShader" << endl;
    return new SimpleVertexShader();
}
