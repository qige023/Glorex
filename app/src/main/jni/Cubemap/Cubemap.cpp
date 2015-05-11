#include "Cubemap.h"

// Std. Includes
#include <cstdio>
#include <cstdlib>
#include <iostream>

using std::cout;
using std::cerr;
using std::endl;

//Glorex Includes
#include "esutil.h"
#include "loader/stbloader.h"

//GLM Includes
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/string_cast.hpp>

Cubemap::Cubemap() {
}

Cubemap::~Cubemap() {
//    delete cube;
    delete skybox;
    delete camera;
}

void Cubemap::initScene(ESContext *esContext) {
    cout << "exec Cubemap::initScene" << endl;

    compileAndLinkShader();

    controlLayer = new ControlLayer();
    controlLayer->initLayer(esContext);

    // Enables depth-buffer for hidden surface removal
    glEnable(GL_DEPTH_TEST);

    // The type of depth testing to do
    glDepthFunc(GL_LEQUAL);

//    cube = new VBOCube(1.0f);
   skybox = new VBOSkybox();

   camera = new ESCamera(glm::vec3(0.0f, 0.0f, 3.0f));

   // Cubemap (Skybox)
   skyboxFaces.push_back("media/skybox/valley/right.jpg");
   skyboxFaces.push_back("media/skybox/valley/left.jpg");
   skyboxFaces.push_back("media/skybox/valley/top.jpg");
   skyboxFaces.push_back("media/skybox/valley/bottom.jpg");
   skyboxFaces.push_back("media/skybox/valley/back.jpg");
   skyboxFaces.push_back("media/skybox/valley/front.jpg");
//   skyboxFaces.push_back("media/skybox/stonegods/right.tga");
//   skyboxFaces.push_back("media/skybox/stonegods/left.tga");
//   skyboxFaces.push_back("media/skybox/stonegods/top.tga");
//   skyboxFaces.push_back("media/skybox/stonegods/bottom.tga");
//   skyboxFaces.push_back("media/skybox/stonegods/back.tga");
//   skyboxFaces.push_back("media/skybox/stonegods/front.tga");

   skyboxTexture = STBLoader::loadCubemap(skyboxFaces);

    model = mat4(1.0f);
    cout << "Model Matrix" << glm::to_string(model) << endl;

    // Remove any translation component of the view matrix;
    view = glm::mat4(glm::mat3(camera->getViewMatrix()));
    cout << "View Matrix" << glm::to_string(view) << endl;

    projection = glm::perspective(glm::radians(camera->Zoom),
            (float) esContext->width / esContext->height, 0.1f, 100.0f);
    cout << "Projection Matrix" << glm::to_string(projection) << endl;

}

void Cubemap::update(ESContext *esContext, float deltaTime) {
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

void Cubemap::render(ESContext *esContext) {

    // Clear buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //This way the skybox will always be drawn at the background of all the other objects.
    glDepthMask(GL_FALSE);
    glActiveTexture(GL_TEXTURE0);
    prog.setUniform("skybox", 0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTexture);
    view = glm::mat4(glm::mat3(camera->getViewMatrix()));
    setMatrices();
    skybox->render();
    glDepthMask(GL_TRUE);

    controlLayer->render(esContext);
}

void Cubemap::setMatrices() {
    prog.setUniform("MVP", projection * view * model);
}

void Cubemap::resize(ESContext *esContext) {
    glViewport(0, 0, esContext->width, esContext->height);
    projection = glm::perspective(glm::radians(45.0f), (float) esContext->width / esContext->height,
                0.1f, 100.0f);
}

void Cubemap::compileAndLinkShader() {
    cout << "exec Cubemap::compileAndLinkShader" << endl;
    try {
        prog.compileShader("shader/cubemap_skybox.vert");
        prog.compileShader("shader/cubemap_skybox.frag");
        prog.link();
        prog.validate();
        prog.use();
    } catch (ESProgramException & e) {
        cerr << e.what() << endl;
        exit( EXIT_FAILURE);
    }
}

ESScene *esCreateScene(ESContext *esContext) {
    cout << "exec esCreateScene -> create scene Cubemap" << endl;
    return new Cubemap();
}
