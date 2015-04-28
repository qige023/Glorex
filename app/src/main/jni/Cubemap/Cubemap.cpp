#include "Cubemap.h"

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

Cubemap::Cubemap() {
}

Cubemap::~Cubemap() {
//    delete cube;
    delete skybox;
}

void Cubemap::initScene(ESContext *esContext) {
    cout << "exec Cubemap::initScene" << endl;

    // Define the viewport dimensions
    glViewport(0, 0, esContext->width, esContext->height);

    compileAndLinkShader();

    // Enables depth-buffer for hidden surface removal
    glEnable(GL_DEPTH_TEST);

    // The type of depth testing to do
    glDepthFunc(GL_LEQUAL);

//    cube = new VBOCube(1.0f);
   skybox = new VBOSkybox();

   // Cubemap (Skybox)
   skyboxFaces.push_back("media/skybox/greenroad/right.jpg");
   skyboxFaces.push_back("media/skybox/greenroad/left.jpg");
   skyboxFaces.push_back("media/skybox/greenroad/top.jpg");
   skyboxFaces.push_back("media/skybox/greenroad/bottom.jpg");
   skyboxFaces.push_back("media/skybox/greenroad/back.jpg");
   skyboxFaces.push_back("media/skybox/greenroad/front.jpg");

   skyboxTexture = STBLoader::loadCubemap(skyboxFaces);

    model = mat4(1.0f);
    cout << "Model Matrix" << glm::to_string(model) << endl;

    view = mat4(1.0f);
    view *= glm::lookAt(vec3(0.0f,0.0f,0.0f), vec3(0.0f,0.0f,-1.0f), vec3(0.0f,1.0f,0.0f));
    cout << "View Matrix" << glm::to_string(view) << endl;

    projection = glm::perspective(glm::radians(45.0f), (float) esContext->width / esContext->height,
            0.1f, 100.0f);
    cout << "Projection Matrix" << glm::to_string(projection) << endl;

}

void Cubemap::update(ESContext *esContext, float deltaTime) {

}

void Cubemap::render(ESContext *esContext) {

    // Clear buffers
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //This way the skybox will always be drawn at the background of all the other objects.
    glDepthMask(GL_FALSE);
    glActiveTexture(GL_TEXTURE0);
    prog.setUniform("skybox", 0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTexture);
    setMatrices();
    skybox->render();
    glDepthMask(GL_TRUE);
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
