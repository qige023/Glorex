#include "TeapotTwoSide.h"
#include "esutil.h"

#include <cstdio>
#include <cstdlib>
#include <iostream>
using std::cout;
using std::cerr;
using std::endl;

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/string_cast.hpp>
using glm::vec3;

TeapotTwoSide::TeapotTwoSide() : angle(0.0f) {
}

void TeapotTwoSide::initScene(ESContext *esContext) {
    cout << "exec TeapotTwoSide::initScene" << endl;

    compileAndLinkShader();

    // Enables depth-buffer for hidden surface removal
    glEnable(GL_DEPTH_TEST);

    // The type of depth testing to do
    glDepthFunc(GL_LEQUAL);

    mat4 transform = glm::translate(mat4(1.0f), vec3(0.0f, 1.5f, 0.25f));
    teapot = new VBOTeapot(13, transform);

    model = mat4(1.0f);
    model *= glm::translate(vec3(0.0,-1.0,0.0));

    view = glm::lookAt(vec3(0.0f,0.0f,8.0f), vec3(0.0f,0.0f,0.0f), vec3(0.0f,1.0f,0.0f));
    cout << "View Matrix" << glm::to_string(view) << endl;

    projection = mat4(1.0f);
    projection = glm::perspective(glm::radians(45.0f), (float) esContext->width / esContext->height,
            0.1f, 100.0f);
    cout << "Projection Matrix" << glm::to_string(projection) << endl;

    prog.setUniform("Material.Kd", 0.9f, 0.5f, 0.3f);
    prog.setUniform("Light.Ld", 1.0f, 1.0f, 1.0f);
    prog.setUniform("Material.Ka", 0.9f, 0.5f, 0.3f);
    prog.setUniform("Light.La", 0.4f, 0.4f, 0.4f);
    prog.setUniform("Material.Ks", 0.8f, 0.8f, 0.8f);
    prog.setUniform("Light.Ls", 1.0f, 1.0f, 1.0f);
    prog.setUniform("Material.Shininess", 100.0f);

    // Set background color
    glClearColor(0.2f, 0.2f, 0.2f, 0.0f);
}

void TeapotTwoSide::update(ESContext *esContext, float deltaTime) {
    // 40 degree per second
    angle += deltaTime * 40.0f;
    model *= glm::rotate(glm::radians(deltaTime * 40.0f), vec3(-1.0f,1.0f,0.0f));
}

void TeapotTwoSide::render(ESContext *esContext) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    vec4 worldLight = vec4(2.0f, 4.0f, 2.0f, 1.0f);
    prog.setUniform("Light.Position", view * model * worldLight);

    setMatrices();
    teapot->render();
}

void TeapotTwoSide::setMatrices() {
    mat4 mv = view * model;
    prog.setUniform("ModelViewMatrix", mv);
    prog.setUniform("NormalMatrix",
            mat3(vec3(mv[0]), vec3(mv[1]), vec3(mv[2])));
    prog.setUniform("MVP", projection * mv);
}

void TeapotTwoSide::resize(ESContext *esContext) {
    glViewport(0, 0, esContext->width, esContext->height);
    projection = glm::perspective(glm::radians(45.0f), (float) esContext->width / esContext->height,
                0.1f, 100.0f);
}

void TeapotTwoSide::compileAndLinkShader() {
    try {
        cout << "Scene TeapotTwoSide compile..." << endl;
        prog.compileShader("shader/twoside.vert");
        prog.compileShader("shader/twoside.frag");
        prog.link();
        prog.validate();
        prog.use();
    } catch (ESProgramException & e) {
        cerr << e.what() << endl;
        exit( EXIT_FAILURE);
    }
}

ESScene *esCreateScene(ESContext *esContext) {
    cout << "exec esCreateScene -> create scene TeapotTwoSide" << endl;
    return new TeapotTwoSide();
}

