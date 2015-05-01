#include "controllayer.h"
#include <iostream>

using std::cout;
using std::cerr;
using std::endl;

void ControlLayer::storePreProgram() {
    glGetIntegerv(GL_CURRENT_PROGRAM, &lastProgHandle);
}

void ControlLayer::recoverPreProgram() {
    glUseProgram(lastProgHandle);
}

ControlLayer::ControlLayer() {

}

ControlLayer::~ControlLayer() {
}

void ControlLayer::initLayer(ESContext *esContext) {
    storePreProgram();

    compileAndLinkShader();
    projection = mat4(1.0f);
    projection *= glm::ortho(0.0f, static_cast<GLfloat>(esContext->width), 0.0f,
            static_cast<GLfloat>(esContext-> height));



    recoverPreProgram();
}

void ControlLayer::update(ESContext *esContext, float deltaTime ) {

}

void RenderingText::render(ESContext *esContext) {

}

void ControlLayer::resize(ESContext *esContext) {
    glViewport(0,0,esContext->width,esContext->height);
}

void ControlLayer::compileAndLinkShader() {
    cout << "exec RenderingText::compileAndLinkShader" << endl;
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
