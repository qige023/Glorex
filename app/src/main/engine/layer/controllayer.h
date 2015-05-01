#ifndef CONTROLLAYER_H
#define CONTROLLAYER_H

#include "escontext.h"
#include "vboshape.h"

#include <glm/glm.hpp>
using glm::mat4;

class ControlLayer {
private:

    VBOShape leftPanel;
    VBOShape rightPanel;
    ESProgram prog;

    mat4 projection;

    // save last program handle, we will store it and recover it
    // on each call of public function
    GLint lastProgHandle;

    void storePreProgram();
    void recoverPreProgram();
    void compileAndLinkShader();
public:

    ControlLayer() {
    }

    ~ControlLayer() {
    }

    /**
     Load textures, initialize shaders, etc.
     */
    void initLayer(ESContext *esContext) = 0;

    /**
     This is called prior to every frame.  Use this
     to update your animation.
     */
    void update(ESContext *esContext, float t) = 0;

    /**
     Draw your layer.
     */
    void render(ESContext *esContext) = 0;

    /**
     Called when layer is resized
     */
    void resize(ESContext *esContext) = 0;

};

#endif // CONTROLLAYER_H
