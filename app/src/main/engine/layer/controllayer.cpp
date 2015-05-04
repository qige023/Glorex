#include "controllayer.h"

#include <iostream>
#include <algorithm>

using std::cout;
using std::cerr;
using std::endl;

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <glm/gtx/string_cast.hpp>

using glm::vec2;
using glm::vec3;
using glm::vec4;

#include "esutil.h"

ControlLayer::ControlLayer() {

}

ControlLayer::~ControlLayer() {
    delete leftPanel;
}

void ControlLayer::initLayer(ESContext *esContext) {
    cout << "exec ControlLayer::initLayer" << endl;

    esContext->onMotionListener = new OnMotionControlLayerListener(this);

    compileAndLinkShader();
    storeState();

    projection = mat4(1.0f);
    projection *= glm::ortho(0.0f, static_cast<GLfloat>(esContext->width), 0.0f,
            static_cast<GLfloat>(esContext-> height));
    prog.setUniform("MVP", projection);

    //Generate panel params based on scene width/height
    GLfloat panelMargin, panelPositionBase, panelRadius;

    width = esContext->width;
    height = esContext->height;
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
    leftPanel = new MotionArea(panelRadius, panelPositionBase, panelPositionBase);

    //draw second Circle
    rightPanel = new MotionArea(panelRadius, esContext->width - panelPositionBase, panelPositionBase);

    recoverState();
}

void ControlLayer::update(ESContext *esContext, float deltaTime ) {

}

void ControlLayer::render(ESContext *esContext) {
    storeState();

    // do real render
    prog.setUniform("CircleColor", vec4(0.0f, 0.0f, 0.0f, 0.2f));
    leftPanel->panel->render(GL_TRIANGLE_FAN);
    rightPanel->panel->render(GL_TRIANGLE_FAN);

    prog.setUniform("CircleColor", vec4(0.0f, 0.0f, 1.0f, 0.8f));
    if(leftPanel->isActive == TRUE) {
        leftPanel->hitPoint->render(GL_TRIANGLE_FAN);
    }
    if(rightPanel->isActive == TRUE) {
        rightPanel->hitPoint->render(GL_TRIANGLE_FAN);
    }

    recoverState();
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
    } catch (ESProgramException & e) {
        cerr << e.what() << endl;
        exit( EXIT_FAILURE);
    }
}

void ControlLayer::storeState() {

    glEnables[GL_BLEND] = glIsEnabled(GL_BLEND);
    if(glEnables[GL_BLEND] == TRUE) {
        // save off current state of src / dst blend functions
        GLint blendSrc;
        GLint blendDst;
        glGetIntegerv(GL_BLEND_SRC_ALPHA, &blendSrc);
        glGetIntegerv(GL_BLEND_DST_ALPHA, &blendDst);
        glFuncAttrs["GL_BLEND_SRC"] = blendSrc;
        glFuncAttrs["GL_BLEND_DST"] = blendDst;
    } else {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }

    glEnables[GL_DEPTH_TEST] = glIsEnabled(GL_DEPTH_TEST);
    if(glEnables[GL_DEPTH_TEST] == TRUE) {
        GLint depthFunc;
        glGetIntegerv(GL_DEPTH_FUNC, &depthFunc);
        glFuncAttrs["GL_DEPTH_FUNC"] = depthFunc;
    } else {
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_ALWAYS);
    }

    GLint lastProgHandle;
    glGetIntegerv(GL_CURRENT_PROGRAM, &lastProgHandle);
    glFuncAttrs["GL_CURRENT_PROGRAM"] = lastProgHandle;

    if(prog.isLinked()) {
        prog.use();
    }
}

void ControlLayer::recoverState() {
    if(glEnables[GL_BLEND] == TRUE) {
        glBlendFunc(glFuncAttrs["GL_BLEND_SRC"] , glFuncAttrs["GL_BLEND_DST"]);
    } else {
        glDisable(GL_BLEND);
    }
    if(glEnables[GL_DEPTH_TEST] == TRUE) {
        glDepthFunc(glFuncAttrs["GL_DEPTH_FUNC"]);
    } else {
        glDisable(GL_DEPTH_TEST);
    }
    glUseProgram(glFuncAttrs["GL_CURRENT_PROGRAM"]);
}

ESboolean ControlLayer::getPanelState(int panelNum, float &angle, float &factor) {
    if(panelNum == LEFT_PANEL) {
        if (leftPanel->isActive) {
            angle = leftPanel->angle;
            factor = leftPanel->factor;
            return TRUE;
        } else {
            return FALSE;
        }
    } else {
        if (rightPanel->isActive) {
            angle = rightPanel->angle;
            factor = rightPanel->factor;
            return TRUE;
        } else {
            return FALSE;
        }
    }
}

//---------------------MotionArea---------------------------

GLint MotionArea::Circle_Precition = 50;

MotionArea::MotionArea(GLfloat nradius, GLfloat nposx, GLfloat nposy): radius(nradius),
        posx(nposx), posy(nposy), isActive(FALSE), pointerId(-1){
    GLfloat *circleVertexs = MotionArea::generateCircleVertexs(radius, posx, posy, Circle_Precition);
    panel = new VBOShape(circleVertexs, Circle_Precition + 2, false, false, false);
    delete[] circleVertexs;
}

ESboolean MotionArea::checkPoint(GLfloat rx, GLfloat ry) {
    if(radius > sqrt(pow((rx - posx), 2.0f) + pow((ry - posy), 2.0f))){
        return TRUE;
    }
    return FALSE;
}

void MotionArea::addHitPoint(GLfloat rx, GLfloat ry) {
    isActive = TRUE;
    posx1 = rx;
    posy1 = ry;
    radius1 = 70.0f;
    GLfloat *hitPointVertexs = MotionArea::generateCircleVertexs(radius1, posx1, posy1, Circle_Precition);
    hitPoint = new VBOShape(hitPointVertexs, Circle_Precition + 2, false, false, false, GL_DYNAMIC_DRAW);
    delete[] hitPointVertexs;
    updateMotionResult();
}

void MotionArea::removeHitPoint() {
    isActive = FALSE;
    pointerId = -1;
    if(hitPoint != NULL) {
        delete hitPoint;
        hitPoint = NULL;
    }
}

void MotionArea::updateHitPoint(GLfloat rx, GLfloat ry) {
    posx1 = rx;
    posy1 = ry;
    radius1 = 70.0f;
    GLfloat *hitPointVertexs = MotionArea::generateCircleVertexs(radius1, posx1, posy1, Circle_Precition);
    hitPoint->setBufferSubData(GL_ARRAY_BUFFER, 0, (Circle_Precition + 2) * 3 * sizeof(GLfloat), hitPointVertexs);
    delete[] hitPointVertexs;
    updateMotionResult();
}

GLfloat *MotionArea::generateCircleVertexs(GLfloat radius,GLfloat rx,GLfloat ry,GLint divider) {
    GLfloat *circleVertexs = new GLfloat[(divider + 2) * 3];
    circleVertexs[0] = rx;
    circleVertexs[1] = ry;
    circleVertexs[2] = 0;

    const GLfloat anglePerSegment = ((360.0f/divider) / 360.0f) * ES_TWOPI;
    GLfloat angle = 0.0f;
    for (int i = 1; i < (divider + 2); i ++) {
        circleVertexs[i * 3] = rx + cos(angle) * radius;
        circleVertexs[i * 3 + 1] = ry + sin(angle) * radius;
        circleVertexs[i * 3 + 2] = 0.0f;
        angle += anglePerSegment;
    }
    return circleVertexs;
}

void MotionArea::render() {
    panel->render(GL_TRIANGLE_FAN);
    if(isActive) {
        hitPoint->render(GL_TRIANGLE_FAN);
    }
}

void MotionArea::updateMotionResult() {
    vec2 hitpointVec = glm::normalize(vec2((posx1 - posx), (posy1 - posy)));
    angle = glm::orientedAngle(hitpointVec, vec2(-1.0, 0.0));
    factor = sqrt(pow((posx1 - posx), 2.0f) + pow((posy1 - posy), 2.0f)) / radius;
//    cout << "angle=" << angle << "  degrees=" <<
//            ES_TO_DEGREES(angle) << "  factor=" << factor << endl;
}

//---------------------OnMotionControlLayerListener---------------------------

OnMotionControlLayerListener::OnMotionControlLayerListener(ControlLayer *nControlLayer):controlLayer(nControlLayer) {

}

void OnMotionControlLayerListener::onMotionDown(int32_t pointerId, float downX, float downY) {
    downY = controlLayer->height - downY;
    if(controlLayer->leftPanel->checkPoint(downX, downY) == TRUE) {
        controlLayer->leftPanel->pointerId = pointerId;
        controlLayer->leftPanel->addHitPoint(downX, downY);
    }
    if(controlLayer->rightPanel->checkPoint(downX, downY) == TRUE) {
        controlLayer->rightPanel->pointerId = pointerId;
        controlLayer->rightPanel->addHitPoint(downX, downY);
    }
}

void OnMotionControlLayerListener::onMotionMove(int32_t pointerId, float downX, float downY) {
    downY = controlLayer->height - downY;
    if(controlLayer->leftPanel->isActive == TRUE && controlLayer->leftPanel->pointerId == pointerId){
        if(controlLayer->leftPanel->checkPoint(downX, downY) == TRUE) {
            controlLayer->leftPanel->updateHitPoint(downX, downY);
        } else {
            // do nothing
//            controlLayer->leftPanel->removeHitPoint();
        }
    }
    if(controlLayer->rightPanel->isActive == TRUE && controlLayer->rightPanel->pointerId == pointerId){
        if(controlLayer->rightPanel->checkPoint(downX, downY) == TRUE) {
            controlLayer->rightPanel->updateHitPoint(downX, downY);
        } else {
            // do nothing
//            controlLayer->rightPanel->removeHitPoint();
        }
    }
}

void OnMotionControlLayerListener::onMotionUp(int32_t pointerId, float downX, float downY) {
    downY = controlLayer->height - downY;
    if(controlLayer->leftPanel->isActive == TRUE && controlLayer->leftPanel->pointerId == pointerId){
        controlLayer->leftPanel->removeHitPoint();
    }
    if(controlLayer->rightPanel->isActive == TRUE && controlLayer->rightPanel->pointerId == pointerId){
        controlLayer->rightPanel->removeHitPoint();
    }
}

