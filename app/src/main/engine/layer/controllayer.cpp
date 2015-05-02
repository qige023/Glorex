#include "controllayer.h"

#include "defines.h"
#include <iostream>
#include <algorithm>

using std::cout;
using std::cerr;
using std::endl;

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/string_cast.hpp>

using glm::vec3;
using glm::vec4;

ControlLayer::ControlLayer() {

}

ControlLayer::~ControlLayer() {
    delete[] leftPanel;
}

void ControlLayer::initLayer(ESContext *esContext) {
    cout << "exec ControlLayer::initLayer" << endl;

    esContext->onMotionListener = new OnMotionControlLayerListener();

    storePreProgram();
    compileAndLinkShader();

    projection = mat4(1.0f);
    projection *= glm::ortho(0.0f, static_cast<GLfloat>(esContext->width), 0.0f,
            static_cast<GLfloat>(esContext-> height));
    prog.setUniform("MVP", projection);

    //Generate panel params based on scene width/height
    GLfloat panelMargin, panelPositionBase, panelRadius;

    if(esContext->width >  esContext->height) {
        panelPositionBase = std::min(esContext->width/3, esContext->height/2) / 2;
        panelMargin = esContext->height/15.0f;
    } else {
        panelPositionBase = std::min(esContext->width/2, esContext->height/3) / 2;
        panelMargin = esContext->width/15.0f;
    }
    panelRadius = panelPositionBase - panelMargin;

    GLint circlePrecition = 50;
    //draw first Circle
    GLfloat *leftCircleVertexs = generateCircleVertexs(panelRadius, panelPositionBase, panelPositionBase, circlePrecition);
    leftPanel = new VBOShape(leftCircleVertexs, circlePrecition + 2, false, false, false);
    delete[] leftCircleVertexs;

    //draw second Circle
    GLfloat *rightCircleVertexs = generateCircleVertexs(panelRadius, esContext->width - panelPositionBase, panelPositionBase, circlePrecition);
    rightPanel = new VBOShape(rightCircleVertexs, circlePrecition + 2, false, false, false);
    delete[] rightCircleVertexs;

    recoverPreProgram();
}

void ControlLayer::update(ESContext *esContext, float deltaTime ) {

}

void ControlLayer::render(ESContext *esContext) {
    storePreProgram();

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    if(glIsEnabled(GL_DEPTH_TEST)) {
        glDepthFunc(GL_ALWAYS);
    }

    // do real render
    leftPanel->render(GL_TRIANGLE_FAN);
    rightPanel->render(GL_TRIANGLE_FAN);

    // restore previous state of previous program
    if(glIsEnabled(GL_DEPTH_TEST)) {
        glDepthFunc(GL_LEQUAL);
    }
    recoverPreProgram();
}

void ControlLayer::resize(ESContext *esContext) {
    glViewport(0,0,esContext->width,esContext->height);
}

void ControlLayer::compileAndLinkShader() {
    cout << "exec ControlLayer::compileAndLinkShader" << endl;
    try {
        prog.compileShader("shader/controllayer.vert");
        prog.compileShader("shader/controllayer.frag");
        prog.link();
        prog.validate();
        prog.use();
    } catch (ESProgramException & e) {
        cerr << e.what() << endl;
        exit( EXIT_FAILURE);
    }
}

void ControlLayer::storePreProgram() {
    glGetIntegerv(GL_CURRENT_PROGRAM, &lastProgHandle);
    if(prog.isLinked()) {
        prog.use();
    }
}

void ControlLayer::recoverPreProgram() {
    glUseProgram(lastProgHandle);
}

GLfloat *ControlLayer::generateCircleVertexs(GLfloat radius,GLfloat rx,GLfloat ry,GLint divider) {
    GLfloat *circleVertexs = new GLfloat[(divider + 2) * 3];
    circleVertexs[0] = rx;
    circleVertexs[1] = ry;
    circleVertexs[2] = 0;

    const GLfloat anglePerSegment = ((360.0f/divider) / 360.0f) * TWOPI;
    GLfloat angle = 0.0f;
    for (int i = 1; i < (divider + 2); i ++) {
        circleVertexs[i * 3] = rx + cos(angle) * radius;
        circleVertexs[i * 3 + 1] = ry + sin(angle) * radius;
        circleVertexs[i * 3 + 2] = 0.0f;
        angle += anglePerSegment;
    }
    return circleVertexs;
}

void OnMotionControlLayerListener::onMotionDown(int32_t pointerId, float downX, float downY) {
}

void OnMotionControlLayerListener::onMotionMove(int32_t pointerId, float downX, float downY) {

}

void OnMotionControlLayerListener::onMotionUp(int32_t pointerId, float downX, float downY) {
}
