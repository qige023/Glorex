#include "BlendingWindow.h"

#include "esutil.h"
#include <cstdio>
#include <cstdlib>
#include <iostream>

#include "loader/stbloader.h"

using std::cout;
using std::cerr;
using std::endl;

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/string_cast.hpp>

BlendingWindow::BlendingWindow() {
}

BlendingWindow::~BlendingWindow() {
    glDeleteTextures(1, &cubeTexture);
    glDeleteTextures(1, &floorTexture);
    glDeleteTextures(1, &windowTexture);
    delete cube;
    delete floor;
    delete window;
}

void BlendingWindow::initScene(ESContext *esContext) {
    cout << "exec BlendingWindow::initScene" << endl;

    GLfloat floorVertices[] = {
        // Positions
        // Texture Coords (note we set these higher than 1 that together with GL_REPEAT
        // as texture wrapping mode will cause the floor texture to repeat)
        5.0f, -0.5f,  5.0f,  2.0f,  0.0f,
        -5.0f, -0.5f,  5.0f,  0.0f,  0.0f,
        -5.0f, -0.5f, -5.0f,  0.0f,  2.0f,

        5.0f, -0.5f,  5.0f,  2.0f,  0.0f,
        -5.0f, -0.5f, -5.0f,  0.0f,  2.0f,
        5.0f, -0.5f, -5.0f,  2.0f,  2.0f
    };

    GLfloat windowVertices[] = {
        // Positions
        // Texture Coords (swapped y coordinates because texture is flipped upside down)
        0.0f,  0.5f,  0.0f,  0.0f,  0.0f,
        0.0f, -0.5f,  0.0f,  0.0f,  1.0f,
        1.0f, -0.5f,  0.0f,  1.0f,  1.0f,

        0.0f,  0.5f,  0.0f,  0.0f,  0.0f,
        1.0f, -0.5f,  0.0f,  1.0f,  1.0f,
        1.0f,  0.5f,  0.0f,  1.0f,  0.0f
    };

    windowPostions.push_back(glm::vec3(-1.5f,  0.0f, -0.48f));
    windowPostions.push_back(glm::vec3( 1.5f,  0.0f,  0.51f));
    windowPostions.push_back(glm::vec3( 0.0f,  0.0f,  0.7f));
    windowPostions.push_back(glm::vec3(-0.3f,  0.0f, -2.3f));
    windowPostions.push_back(glm::vec3( 0.5f,  0.0f, -0.6f));

    compileAndLinkShader();

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    cube = new VBOCube(1.0f);
    floor = new VBOShape(floorVertices, false, true, false, 5, NULL);
    window = new VBOShape(windowVertices, false, true, false, 5, NULL);

    cubeTexture = STBLoader::loadTex("media/texture/marble.jpg");
    floorTexture = STBLoader::loadTex("media/texture/metal.png");
    windowTexture = STBLoader::loadTex("media/texture/blending_transparent_window.png", true);

    vec3 cameraPosition = glm::vec3(0.0f, 0.0f, 3.0f);

    // Sort windows
    for (GLuint i = 0; i < windowPostions.size(); i++) {
        GLfloat distance = glm::length(cameraPosition - windowPostions[i]);
        sortedWindowPostions[distance] = windowPostions[i];
    }

    view = mat4(1.0f);
    view *= glm::lookAt(cameraPosition, vec3(0.0f,0.0f,0.0f), vec3(0.0f,1.0f,0.0f));
    cout << "View Matrix" << glm::to_string(view) << endl;

    projection = glm::perspective(glm::radians(45.0f), (float) esContext->width / esContext->height,
            0.1f, 100.0f);
    cout << "Projection Matrix" << glm::to_string(projection) << endl;

    glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
}

void BlendingWindow::update(ESContext *esContext, float deltaTime) {

}

void BlendingWindow::render(ESContext *esContext) {

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    model = mat4(1.0f);

    // We omit the glActiveTexture part since TEXTURE0 is already the default active texture unit.
    //(a single sampler used in fragment is set to 0 as well by default)
    glBindTexture(GL_TEXTURE_2D, cubeTexture);

    //first cube
    model = glm::translate(model, glm::vec3(-1.0f, 0.0f, -1.0f));
    setMatrices();
    cube->render();

    //second cube
    model = glm::mat4();
    model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f));
    setMatrices();
    cube->render();

    //floor
    glBindTexture(GL_TEXTURE_2D, floorTexture);
    model = mat4(1.0f);
    setMatrices();
    floor->render();

    // Render windows (from furthest to nearest)
    glBindTexture(GL_TEXTURE_2D, windowTexture);
    for (map<float, glm::vec3>::reverse_iterator it = sortedWindowPostions.rbegin();
            it != sortedWindowPostions.rend(); ++it) {
        model = mat4(1.0f);
        model = glm::translate(model, it->second);
        setMatrices();
        window->render();
    }
}

void BlendingWindow::setMatrices() {
    prog.setUniform("MVP", projection * view * model);
}

void BlendingWindow::resize(ESContext *esContext) {
    glViewport(0, 0, esContext->width, esContext->height);
    projection = glm::perspective(glm::radians(60.0f), (float) esContext->width / esContext->height,
                0.1f, 100.0f);
}

void BlendingWindow::compileAndLinkShader() {
    cout << "exec BlendingWindow::compileAndLinkShader" << endl;
    try {
        prog.compileShader("shader/simpletexture.vert");
        prog.compileShader("shader/simpletexture.frag");
        prog.link();
        prog.validate();
        prog.use();
    } catch (ESProgramException & e) {
        cerr << e.what() << endl;
        exit( EXIT_FAILURE);
    }
}

ESScene *esCreateScene(ESContext *esContext) {
    cout << "exec esCreateScene -> create scene BlendingWindow" << endl;
    return new BlendingWindow();
}
