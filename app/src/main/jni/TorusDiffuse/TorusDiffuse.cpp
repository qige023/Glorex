#include "TorusDiffuse.h"

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

TorusDiffuse::TorusDiffuse() {
}

TorusDiffuse::~TorusDiffuse() {
    delete torus;
}

void TorusDiffuse::initScene(ESContext *esContext) {
    cout << "exec TorusDiffuse::initScene" << endl;

    compileAndLinkShader();

    // Enables depth-buffer for hidden surface removal
    glEnable(GL_DEPTH_TEST);

    // The type of depth testing to do
    glDepthFunc(GL_LEQUAL);

    angle = 45.0f;
    torus = new VBOTorus(0.5f, 0.2f, 60, 60);

    model = mat4(1.0f);
//    model *= glm::translate(vec3(1.0,1.0,0.0));
    model *= glm::rotate(glm::radians(angle), vec3(0.0f,1.0f,1.0f));
    cout << "Model Matrix" << glm::to_string(model) << endl;

    view = mat4(1.0f);
    view *= glm::lookAt(vec3(0.0f,0.0f,2.5f), vec3(0.0f,0.0f,0.0f), vec3(0.0f,1.0f,0.0f));
    cout << "View Matrix" << glm::to_string(view) << endl;

    projection = glm::perspective(glm::radians(45.0f), (float) esContext->width / esContext->height,
            0.1f, 100.0f);
    cout << "Projection Matrix" << glm::to_string(projection) << endl;

    prog.setUniform("Kd", 0.9f, 0.5f, 0.3f);
    prog.setUniform("Ld", 1.0f, 1.0f, 1.0f);
    prog.setUniform("LightPosition", view * vec4(5.0f,5.0f,2.0f,1.0f) );

    // Set background color
    glClearColor(0.2f, 0.2f, 0.2f, 0.0f);
}

void TorusDiffuse::update(ESContext *esContext, float deltaTime) {
    // 40 degree per second
    angle += deltaTime * 40.0f;
    model = mat4(1.0f);
    model *= glm::rotate(glm::radians(angle), vec3(-1.0f,1.0f,0.0f));
}

void TorusDiffuse::render(ESContext *esContext) {

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    setMatrices();

    torus->render();
}

void TorusDiffuse::setMatrices() {
    mat4 mv = view * model;
    prog.setUniform("ModelViewMatrix", mv);
    prog.setUniform("NormalMatrix",
            mat3(vec3(mv[0]), vec3(mv[1]), vec3(mv[2])));
    prog.setUniform("MVP", projection * mv);
}

void TorusDiffuse::resize(ESContext *esContext) {
    glViewport(0, 0, esContext->width, esContext->height);
    projection = glm::perspective(glm::radians(45.0f), (float) esContext->width / esContext->height,
                0.1f, 100.0f);
}

void TorusDiffuse::compileAndLinkShader() {
    try {
        cout << "Scene TorusDiffuse compile..." << endl;
        prog.compileShader("shader/diffuse.vert");
        prog.compileShader("shader/diffuse.frag");
        prog.link();
        prog.validate();
        prog.use();
    } catch (ESProgramException & e) {
        cerr << e.what() << endl;
        exit( EXIT_FAILURE);
    }
}

ESScene *esCreateScene(ESContext *esContext) {
    cout << "exec esCreateScene -> create scene TorusDiffuse" << endl;
    return new TorusDiffuse();
}
