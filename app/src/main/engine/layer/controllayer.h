#ifndef CONTROLLAYER_H
#define CONTROLLAYER_H

#include "escontext.h"
#include "esprogram.h"
#include "vboshape.h"

#include <glm/glm.hpp>
using glm::mat4;

class OnMotionControlLayerListener : public IOnMotionListener {
public:
    void onMotionDown(int32_t pointerId, float downX, float downY);
    void onMotionMove(int32_t pointerId, float downX, float downY);
    void onMotionUp(int32_t pointerId, float downX, float downY);
};

class ControlLayer {
    friend class OnMotionControlLayerListener;

private:
    // Circle area where can be hit
    VBOShape *leftPanel;
    VBOShape *rightPanel;
    // Hit point position
    VBOShape *leftHitPoint;
    VBOShape *rightHitPoint;
    ESProgram prog;

    mat4 projection;

    // save last program handle, we will store it and recover it
    // on each call of public function
    GLint lastProgHandle;

    void storePreProgram();
    void recoverPreProgram();
    void compileAndLinkShader();
    GLfloat *generateCircleVertexs(GLfloat radius,GLfloat rx,GLfloat ry,GLint divider);

    friend class Rectangle;
public:

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

};

#endif // CONTROLLAYER_H
