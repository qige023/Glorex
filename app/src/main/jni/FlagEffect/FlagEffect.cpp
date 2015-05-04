#include "FlagEffect.h"

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <cmath>

using std::cout;
using std::cerr;
using std::endl;

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/string_cast.hpp>

using glm::vec3;
using glm::vec4;

#include "esutil.h"
#include "vbos/vboshape.h"
#include "loader/stbloader.h"


FlagEffect::FlagEffect() {
}

FlagEffect::~FlagEffect() {
    glDeleteTextures(1, &flagTexture);
    delete flag;
}

void FlagEffect::initScene(ESContext *esContext) {
    cout << "exec FlagEffect::initScene" << endl;

    compileAndLinkShader();

    // Enables depth-buffer for hidden surface removal
    glEnable(GL_DEPTH_TEST);

    // The type of depth testing to do
    glDepthFunc(GL_LEQUAL);

    // init flag Grid
    for (int x = 0; x < 45; x++) {
        for (int y = 0; y < 45; y++) {
            points[x][y][0] = float((x / 5.0f) - 4.5f);
            points[x][y][1] = float((y / 5.0f) - 4.5f);
            points[x][y][2] = float(sin((((x / 5.0f) * 40.0f) / 360.0f) * ES_TWOPI));
        }
    }

    wiggle_count = 0;
    angle = 0.0f;

    GLfloat *flagVertexs = generateFlagVertexs();
    flag = new VBOShape(flagVertexs, 44 * 44 * 6, false, true, false, GL_DYNAMIC_DRAW);
    delete[] flagVertexs;

    // Load texture file
    flagTexture = STBLoader::loadTex("media/texture/thumb_mount.bmp");

    model = mat4(1.0f);
    cout << "Model Matrix" << glm::to_string(model) << endl;

    view = mat4(1.0f);
    view *= glm::lookAt(vec3(0.0f,0.0f,20.0f), vec3(0.0f,0.0f,0.0f), vec3(0.0f,1.0f,0.0f));
    cout << "View Matrix" << glm::to_string(view) << endl;

    projection = glm::perspective(glm::radians(45.0f),
            (float) esContext->width / esContext->height, 0.1f, 100.0f);
    cout << "Projection Matrix" << glm::to_string(projection) << endl;

    // Set background color
    glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
}

void FlagEffect::update(ESContext *esContext, float deltaTime) {

    GLfloat *flagVertexs = generateFlagVertexs();
    flag->setBufferSubData(GL_ARRAY_BUFFER, 0, 44 * 44 * 6 * 5 * sizeof(GLfloat), flagVertexs);
    delete[] flagVertexs;

    GLfloat temp;
    if (wiggle_count == 2) {
        for (int y = 0; y < 45; y++) {
            temp = points[0][y][2];
            for (int x = 0; x < 44; x++) {
                points[x][y][2] = points[x + 1][y][2];
            }
            points[44][y][2] = temp;
        }
        wiggle_count = 0;
    }
    wiggle_count++;

    // 40 degree per second
    angle += deltaTime * 40.0f;
    model = mat4(1.0f);
    model *= glm::rotate(glm::radians(angle), vec3(-1.0f,1.0f,0.0f));
}

void FlagEffect::render(ESContext *esContext) {

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Bind Textures using texture units
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, flagTexture);
    prog.setUniform("SampleTexture", 0);

    setMatrices();

    flag->render();
}

GLfloat *FlagEffect::generateFlagVertexs() {
    GLfloat *vertexArray = new GLfloat[44 * 44 * 6 * 5];
    GLfloat float_x, float_y, float_xb, float_yb;
    int face = 0;
    for (int x = 0; x < 44; x++) {
        for (int y = 0; y < 44; y++) {
            float_x = GLfloat(x) / 44.0f;
            float_y = GLfloat(y) / 44.0f;
            float_xb = GLfloat(x + 1) / 44.0f;
            float_yb = GLfloat(y + 1) / 44.0f;
            int i = face * 6 * 5;
            //first triangle
            //point 1
            vertexArray[i] = points[x][y][0];
            vertexArray[i + 1] = points[x][y][1];
            vertexArray[i + 2] = points[x][y][2];
            vertexArray[i + 3] = float_x;
            vertexArray[i + 4] = float_y;

            //point 2
            vertexArray[i + 5] = points[x + 1][y + 1][0];
            vertexArray[i + 6] = points[x + 1][y + 1][1];
            vertexArray[i + 7] = points[x + 1][y + 1][2];
            vertexArray[i + 8] = float_xb;
            vertexArray[i + 9] = float_yb;

            //point 3
            vertexArray[i + 10] = points[x][y + 1][0];
            vertexArray[i + 11] = points[x][y + 1][1];
            vertexArray[i + 12] = points[x][y + 1][2];
            vertexArray[i + 13] = float_x;
            vertexArray[i + 14] = float_yb;

            //second triangle
            //point 1
            vertexArray[i + 15] = points[x][y][0];
            vertexArray[i + 16] = points[x][y][1];
            vertexArray[i + 17] = points[x][y][2];
            vertexArray[i + 18] = float_x;
            vertexArray[i + 19] = float_y;

            //point 2
            vertexArray[i + 20] = points[x + 1][y][0];
            vertexArray[i + 21] = points[x + 1][y][1];
            vertexArray[i + 22] = points[x + 1][y][2];
            vertexArray[i + 23] = float_xb;
            vertexArray[i + 24] = float_y;

            //point 3
            vertexArray[i + 25] = points[x + 1][y + 1][0];
            vertexArray[i + 26] = points[x + 1][y + 1][1];
            vertexArray[i + 27] = points[x + 1][y + 1][2];
            vertexArray[i + 28] = float_xb;
            vertexArray[i + 29] = float_yb;

            face++;
        }
    }
    return vertexArray;
}

void FlagEffect::setMatrices() {
    prog.setUniform("MVP", projection * view * model);
}

void FlagEffect::resize(ESContext *esContext) {
    glViewport(0, 0, esContext->width, esContext->height);
    projection = glm::perspective(glm::radians(45.0f),
            (float) esContext->width / esContext->height, 0.1f, 100.0f);
}

void FlagEffect::compileAndLinkShader() {
    cout << "exec FlagEffect::compileAndLinkShader" << endl;
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
    cout << "exec esCreateScene -> create scene FlagEffect" << endl;
    return new FlagEffect();
}
