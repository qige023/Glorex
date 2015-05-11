#include "stbloader.h"

#include "esutil.h"
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include "Instancing.h"

using std::cout;
using std::cerr;
using std::endl;

using glm::vec3;
using glm::vec4;

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/string_cast.hpp>

Instancing::Instancing() {
}

Instancing::~Instancing() {
    glDeleteTextures(1, &texId);
    delete cube;
}

void Instancing::initScene(ESContext *esContext) {
    cout << "exec Instancing::initScene" << endl;

    compileAndLinkShader();

    // Enables depth-buffer for hidden surface removal
    glEnable(GL_DEPTH_TEST);

    // The type of depth testing to do
    glDepthFunc(GL_LEQUAL);

    // World space positions of our cubes
    vec3 cubePositions[10] = {
         vec3( 0.0f,  0.0f,  0.0f),
         vec3( 2.0f,  1.5f,  1.0f),
         vec3( 1.3f,  1.0f, -4.5f),
         vec3( 1.5f, -2.3f,  2.5f),
         vec3( 1.3f, -2.0f, -1.7f),
         vec3(-1.5f,  2.5f,  3.0f),
         vec3(-1.7f,  3.0f, -6.5f),
         vec3(-1.5f, -0.2f,  1.5f),
         vec3(-3.8f, -2.0f, -5.3f),
         vec3( 3.4f, -0.6f, -3.5f),
    };

    this->cubePositions = new vec3[CUBE_NUM];
    for (int i = 0; i < CUBE_NUM; i ++) {
        this->cubePositions[i] = cubePositions[i];
    }

    angle = new float[CUBE_NUM];

    srand (static_cast <unsigned> (time(0)));
    // random angle
    for (int i = 0; i < CUBE_NUM; i ++) {
        float r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
        angle[i] = r * 360.0f;
    }

    cube = new VBOCube(1.0f);

    // Load texture file
    texId = STBLoader::loadTex("media/texture/crate.bmp");

    view = mat4(1.0f);
    view *= glm::lookAt(vec3(0.0f,0.0f,10.0f), vec3(0.0f,0.0f,0.0f), vec3(0.0f,1.0f,0.0f));
    cout << "View Matrix" << glm::to_string(view) << endl;

    projection = glm::perspective(glm::radians(60.0f), (float) esContext->width / esContext->height,
            0.1f, 100.0f);
    cout << "Projection Matrix" << glm::to_string(projection) << endl;

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texId);
    prog.setUniform("SampleTexture", 0);


    // Set background color
    glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
}

void Instancing::update(ESContext *esContext, float deltaTime) {

    // 40 degree per second
    for (int i = 0; i < CUBE_NUM; i ++) {
        angle[i] += deltaTime * 40.0f;
    }
}

void Instancing::render(ESContext *esContext) {

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    for (int i = 0; i < CUBE_NUM; i ++) {
        model = mat4(1.0f);
        model = glm::translate(model, cubePositions[i]);
        model *= glm::rotate(glm::radians(angle[i]), vec3(0.0f,1.0f,1.0f));
        setMatrices();
        cube->render();
    }
}

void Instancing::setMatrices() {
    prog.setUniform("MVP", projection * view * model);
}

void Instancing::resize(ESContext *esContext) {
    glViewport(0, 0, esContext->width, esContext->height);
    projection = glm::perspective(glm::radians(45.0f), (float) esContext->width / esContext->height,
                0.1f, 100.0f);
}

void Instancing::compileAndLinkShader() {
    cout << "exec Instancing::compileAndLinkShader" << endl;
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
    cout << "exec esCreateScene -> create scene Instancing" << endl;
    return new Instancing();
}
