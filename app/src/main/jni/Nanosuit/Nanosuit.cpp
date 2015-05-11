#include "Nanosuit.h"

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
using glm::vec4;

#include "esutil.h"
#include "loader/stbloader.h"

Nanosuit::Nanosuit() {
}

Nanosuit::~Nanosuit() {
    delete nanosuit;
}

// Point light positions
vec3 Nanosuit::pointLightPositions[] = {
   vec3(2.3f, -1.6f, -3.0f),
   vec3(-1.7f, 0.9f, 1.0f)
};

void Nanosuit::initScene(ESContext *esContext) {
    cout << "exec Nanosuit::initScene" << endl;

    compileAndLinkShader();

    controlLayer = new ControlLayer();
    controlLayer->initLayer(esContext);

    // Enables depth-buffer for hidden surface removal
    glEnable(GL_DEPTH_TEST);

    // The type of depth testing to do
    glDepthFunc(GL_LEQUAL);

    // Currently we just load the model from sdcard, not assets folder in android again
    nanosuit = new ESModel(esContext, "sdcard/Glorex/model/nanosuit/nanosuit.obj");

    model = mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0f));   // Translate it down a bit so it's at the center of the scene
    model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));         // It's a bit too big for our scene, so scale it down

    cout << "Model Matrix" << glm::to_string(model) << endl;

    camera = new ESCamera(glm::vec3(0.0f, 1.0f, 5.0f));
    view = camera->getViewMatrix();
    cout << "View Matrix" << glm::to_string(view) << endl;

    projection = glm::perspective(glm::radians(camera->Zoom), (float) esContext->width / esContext->height,
            0.1f, 100.0f);
    cout << "Projection Matrix" << glm::to_string(projection) << endl;

    // Set background color
    glClearColor(0.15f, 0.15f, 0.15f, 0.0f);
}

void Nanosuit::update(ESContext *esContext, float deltaTime) {
    float angle, factor, degree;
    ESboolean isLeftPanelActive = controlLayer->getPanelState(ControlLayer::LEFT_PANEL, angle, factor);
    if(isLeftPanelActive == TRUE) {
        degree = ES_TO_DEGREES(angle);
        if(degree > 45.0f && degree <= 135.0f) {
            camera->moveByDirection(ESCamera::FORWARD, deltaTime);
        } else if(degree > 135.0f || degree <= -135.0f) {
            camera->moveByDirection(ESCamera::RIGHT, deltaTime);
        } else if(degree > -135.0f && degree <= -45.0f) {
            camera->moveByDirection(ESCamera::BACKWARD, deltaTime);
        } else if(degree > -45.0f && degree <= 45.0f) {
            camera->moveByDirection(ESCamera::LEFT, deltaTime);
        }
    }

    ESboolean isRightPanelActive = controlLayer->getPanelState(ControlLayer::RIGHT_PANEL, angle, factor);
    if(isRightPanelActive == TRUE) {
        degree = ES_TO_DEGREES(angle);
        if(degree > 45.0f && degree <= 135.0f) {
            camera->rotate(0, factor);
        } else if(degree > 135.0f || degree <= -135.0f) {
            camera->rotate(factor, 0);
        } else if(degree > -135.0f && degree <= -45.0f) {
            camera->rotate(0, -factor);
        } else if(degree > -45.0f && degree <= 45.0f) {
            camera->rotate(-factor, 0);
        }
    }
}

void Nanosuit::render(ESContext *esContext) {

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    view = camera->getViewMatrix();

    setMatrices();

    nanosuit->draw(prog);

    controlLayer->render(esContext);
}

void Nanosuit::setMatrices() {
    prog.setUniform("model", model);
    prog.setUniform("view", view);
    prog.setUniform("projection", projection);

    // Point light 1
    prog.setUniform("viewPos", camera->Position.x, camera->Position.y, camera->Position.z);
    prog.setUniform("pointLights[0].position", pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z);
    prog.setUniform("pointLights[0].ambient", 0.05f, 0.05f, 0.05f);
    prog.setUniform("pointLights[0].diffuse", 1.0f, 1.0f, 1.0f);
    prog.setUniform("pointLights[0].specular", 1.0f, 1.0f, 1.0f);
    prog.setUniform("pointLights[0].constant", 1.0f);
    prog.setUniform("pointLights[0].linear", 0.009f);
    prog.setUniform("pointLights[0].quadratic", 0.0032f);

    // Point light 2
    prog.setUniform("pointLights[1].position", pointLightPositions[1].x, pointLightPositions[1].y, pointLightPositions[1].z);
    prog.setUniform("pointLights[1].ambient", 0.05f, 0.05f, 0.05f);
    prog.setUniform("pointLights[1].diffuse", 1.0f, 1.0f, 1.0f);
    prog.setUniform("pointLights[1].specular", 1.0f, 1.0f, 1.0f);
    prog.setUniform("pointLights[1].constant", 1.0f);
    prog.setUniform("pointLights[1].linear", 0.009f);
    prog.setUniform("pointLights[1].quadratic", 0.0032f);
}

void Nanosuit::resize(ESContext *esContext) {
    glViewport(0, 0, esContext->width, esContext->height);
    projection = glm::perspective(glm::radians(45.0f), (float) esContext->width / esContext->height,
                0.1f, 100.0f);
}

void Nanosuit::compileAndLinkShader() {
    cout << "exec Nanosuit::compileAndLinkShader" << endl;
    try {
        prog.compileShader("shader/nanosuit.vert");
        prog.compileShader("shader/nanosuit.frag");
        prog.link();
        prog.validate();
        prog.use();
    } catch (ESProgramException & e) {
        cerr << e.what() << endl;
        exit( EXIT_FAILURE);
    }
}

ESScene *esCreateScene(ESContext *esContext) {
    cout << "exec esCreateScene -> create scene Nanosuit" << endl;
    return new Nanosuit();
}
