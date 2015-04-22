#ifndef ESSCENE_H
#define ESSCENE_H

#include "escontext.h"

class ESScene {
public:

    ESScene() :
            m_animate(true) {
    }

    virtual ~ESScene() {
    }

    /**
     Load textures, initialize shaders, etc.
     */
    virtual void initScene(ESContext *esContext) = 0;

    /**
     This is called prior to every frame.  Use this
     to update your animation.
     */
    virtual void update(ESContext *esContext, float t) = 0;

    /**
     Draw your scene.
     */
    virtual void render(ESContext *esContext) = 0;

    /**
     Called when screen is resized
     */
    virtual void resize(ESContext *esContext) = 0;

    void animate(bool value) {
        m_animate = value;
    }
    bool animating() {
        return m_animate;
    }

protected:
    bool m_animate;
};

#endif // ESSCENE_H
