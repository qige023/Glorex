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

TorusDiffuse::TorusDiffuse() {
}

TorusDiffuse::~TorusDiffuse() {
}

void TorusDiffuse::initScene(ESContext *esContext) {
    cout << "exec TorusDiffuse::initScene" << endl;

    compileAndLinkShader();

    torus = new VBOTorus(0.5f, 0.2f, 40, 40);

    model = mat4(1.0f);
//    model *= glm::rotate(glm::radians(-35.0f), vec3(1.0f, 0.0f, 0.0f));
//    model *= glm::rotate(glm::radians(35.0f), vec3(0.0f, 1.0f, 0.0f));
//    view = glm::lookAt(vec3(0.0f, 0.0f, 2.0f), vec3(0.0f, 0.0f, 0.0f),
//            vec3(0.0f, 1.0f, 0.0f));
    view = mat4(1.0f);
    projection = mat4(1.0f);

    prog.setUniform("Kd", 0.9f, 0.5f, 0.3f);
    prog.setUniform("Ld", 1.0f, 1.0f, 1.0f);
    prog.setUniform("LightPosition", view * vec4(5.0f, 5.0f, 2.0f, 1.0f));

    // Set background color
    glClearColor(0.2f, 0.2f, 0.2f, 0.0f);
}

void TorusDiffuse::update(ESContext *esContext, float deltaTime) {
}

void TorusDiffuse::render(ESContext *esContext) {

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Set depth's clear-value to farthest
    glClearDepthf(1.0f);

    // Enables depth-buffer for hidden surface removal
    glEnable(GL_DEPTH_TEST);

    // The type of depth testing to do
    glDepthFunc(GL_LEQUAL);

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

void TorusDiffuse::resize(ESContext *esContext, int w, int h) {
    glViewport(0, 0, w, h);
    width = w;
    height = h;
    projection = glm::perspective(glm::radians(70.0f), (float) w / h, 0.3f,
            100.0f);
}

void TorusDiffuse::compileAndLinkShader() {
    cout << "exec TorusDiffuse::compileAndLinkShader" << endl;
    try {
        cout << "Scene Torus Diffuse compile..." << endl;
        prog.compileShader("shaders/diffuse.vert");
        prog.compileShader("shaders/diffuse.frag");
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
