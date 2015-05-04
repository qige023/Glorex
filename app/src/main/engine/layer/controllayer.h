#ifndef CONTROLLAYER_H
#define CONTROLLAYER_H

#include <string>
#include <map>
using std::string;
using std::map;

#include <glm/glm.hpp>
using glm::mat4;

#include "escontext.h"
#include "esprogram.h"
#include "vboshape.h"

class ControlLayer;

class OnMotionControlLayerListener : public IOnMotionListener {
    ControlLayer *controlLayer;
public:
    OnMotionControlLayerListener(ControlLayer *controlLayer);
    void onMotionDown(int32_t pointerId, float downX, float downY);
    void onMotionMove(int32_t pointerId, float downX, float downY);
    void onMotionUp(int32_t pointerId, float downX, float downY);
};

class MotionArea {
    static GLint Circle_Precition;
public:
    MotionArea(GLfloat posx, GLfloat posy, GLfloat radius);
    // motion area for detect
    VBOShape *panel;
    GLfloat posx, posy, radius;
    // hitPoint area for showing where current user touching
    VBOShape *hitPoint;
    GLfloat posx1, posy1, radius1;
    // mark is user hitting in the motion area
    ESboolean isActive;
    // angle of current hitting point
    GLfloat angle;
    GLfloat factor;
    // pointerId from multi-touch
    int32_t pointerId;

    void render();
    ESboolean checkPoint(GLfloat rx, GLfloat ry);
    void addHitPoint(GLfloat rx, GLfloat ry);
    void updateHitPoint(GLfloat rx, GLfloat ry);
    void removeHitPoint();

private:
    void updateMotionResult();
    GLfloat *generateCircleVertexs(GLfloat radius,GLfloat rx,GLfloat ry,GLint divider);
};

class ControlLayer {
    friend class OnMotionControlLayerListener;

private:
    vec4 motionArearColor;
    vec4 hitPointColor;

    // Circle area where can be hit
    MotionArea *leftPanel;
    MotionArea *rightPanel;

    // layer basic info
    float width, height;
    ESboolean verticalMode;

    ESProgram prog;

    mat4 projection;

    //save and restore GL state
    map<GLenum, GLboolean> glEnables;
    map<string, GLint> glFuncAttrs;

    // we will store current opengl state and recover it
    // on each call of public function
    void storeState();
    void recoverState();

    void compileAndLinkShader();

public:
    const static int LEFT_PANEL = 0;
    const static int RIGHT_PANEL = 1;

    ControlLayer();

    ~ControlLayer();

    /**
     Load textures, initialize shaders, etc.
     */
    void initLayer(ESContext *esContext);

    /**
     This is called prior to every frame.  Use this
     to update your animation.
     */
    void update(ESContext *esContext, float t);

    /**
     Draw your layer.
     */
    void render(ESContext *esContext);

    /**
     Called when layer is resized
     */
    void resize(ESContext *esContext);

    ESboolean getPanelState(int panelNum, float &angle, float &factor);
};

#endif // CONTROLLAYER_H
