#include "RenderingText.h"

#include <iostream>
using std::cout;
using std::endl;

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/string_cast.hpp>
using glm::vec3;
using glm::vec4;

#include "esutil.h"
#include "esfile.h"

RenderingText::RenderingText() { }

RenderingText::~RenderingText() {
    delete charactersHolder;
}

void RenderingText::initScene(ESContext *esContext) {
    compileAndLinkShader();

    charactersHolder = new ESCharactersHolder();
    charactersHolder->loadFont("font/youyuan.ttf");

    // Set OpenGL options
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
}

void RenderingText::update(ESContext *esContext, float t ) {

}

void RenderingText::render(ESContext *esContext) {
    glClear(GL_COLOR_BUFFER_BIT);

    projection = mat4(1.0f);
    projection *= glm::ortho(0.0f, static_cast<GLfloat>(esContext->width), 0.0f, static_cast<GLfloat>(esContext-> height));

    //we omit model and view matrix , they are by default mat4(1.0f)
    prog.setUniform("projection", projection);

    prog.setUniform("textColor", glm::vec3(0.5, 0.8f, 0.2f));
    charactersHolder->displayText("こんにちは、新世界は、ここに来ます", 25.0f, 25.0f, 1.0f);
    prog.setUniform("textColor", glm::vec3(0.3, 0.7f, 0.9f));
    charactersHolder->displayText("(C) Glorex 3D Framework", 300.0f, 570.0f, 1.5f);
    prog.setUniform("textColor", glm::vec3(1.0, 1.0f, 0.0f));
    charactersHolder->displayText("格里斯三维影像渲染框架", 100.0f, 250.0f, 2.0f);
}

void RenderingText::resize(ESContext *esContext) {
    glViewport(0,0,esContext->width,esContext->height);
}

void RenderingText::compileAndLinkShader() {
    cout << "exec RenderingText::compileAndLinkShader" << endl;
    try {
        prog.compileShader("shader/rendertext.vert");
        prog.compileShader("shader/rendertext.frag");
        prog.link();
        prog.validate();
        prog.use();
    } catch (ESProgramException & e) {
        cerr << e.what() << endl;
        exit( EXIT_FAILURE);
    }
}

ESScene *esCreateScene(ESContext *esContext) {
    cout << "exec esCreateScene -> create scene RenderingText" << endl;
    return new RenderingText();
}
