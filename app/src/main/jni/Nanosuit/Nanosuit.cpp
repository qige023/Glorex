#include "Nanosuit.h"

#include "esutil.h"
#include <cstdio>
#include <cstdlib>
#include <iostream>

#include "loader/stbloader.h"

using std::cout;
using std::cerr;
using std::endl;

using glm::vec3;
using glm::vec4;

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/string_cast.hpp>

Nanosuit::Nanosuit() {
}

Nanosuit::~Nanosuit() {
    delete nanosuit;
}

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
    view = camera->GetViewMatrix();
    cout << "View Matrix" << glm::to_string(view) << endl;

    projection = glm::perspective(glm::radians(camera->Zoom), (float) esContext->width / esContext->height,
            0.1f, 100.0f);
    cout << "Projection Matrix" << glm::to_string(projection) << endl;

    // Set background color
    glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
}

void Nanosuit::update(ESContext *esContext, float deltaTime) {
    float angle, factor, degree;
    ESboolean isLeftPanelActive = controlLayer->getPanelState(ControlLayer::LEFT_PANEL, angle, factor);
    if(isLeftPanelActive == TRUE) {
        degree = ES_TO_DEGREES(angle);
        if(degree > 45.0f && degree <= 135.0f) {
            camera->ProcessKeyboard(FORWARD, deltaTime);
        } else if(degree > 135.0f || degree <= -135.0f) {
            camera->ProcessKeyboard(RIGHT, deltaTime);
        } else if(degree > -135.0f && degree <= -45.0f) {
            camera->ProcessKeyboard(BACKWARD, deltaTime);
        } else if(degree > -45.0f && degree <= 45.0f) {
            camera->ProcessKeyboard(LEFT, deltaTime);
        }
    }

    ESboolean isRightPanelActive = controlLayer->getPanelState(ControlLayer::RIGHT_PANEL, angle, factor);
    if(isRightPanelActive == TRUE) {
        degree = ES_TO_DEGREES(angle);
        if(degree > 45.0f && degree <= 135.0f) {
            camera->ProcessMouseMovement(0, factor);
        } else if(degree > 135.0f || degree <= -135.0f) {
            camera->ProcessMouseMovement(factor, 0);
        } else if(degree > -135.0f && degree <= -45.0f) {
            camera->ProcessMouseMovement(0, -factor);
        } else if(degree > -45.0f && degree <= 45.0f) {
            camera->ProcessMouseMovement(-factor, 0);
        }
    }
}

void Nanosuit::render(ESContext *esContext) {

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    view = camera->GetViewMatrix();

    setMatrices();

    nanosuit->draw(prog);

    controlLayer->render(esContext);
}

void Nanosuit::setMatrices() {
    prog.setUniform("MVP", projection * view * model);
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
