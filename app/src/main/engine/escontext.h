/// \file esContext.h
/// \brief
//
#ifndef ESCONTEXT_H
#define ESCONTEXT_H

#ifdef __APPLE__
#include <OpenGLES/ES3/gl.h>
#else
#include <GLES3/gl3.h>
#include <EGL/egl.h>
#include <EGL/eglext.h>
#endif

typedef struct ESContext ESContext;

//Forward Declartion
class ESScene;

struct ESContext {
    /// Put platform specific data here
    void *platformData;

    /// Window width
    GLint width;

    /// Window height
    GLint height;

#ifndef __APPLE__
    /// Display handle
    EGLNativeDisplayType eglNativeDisplay;

    /// Window handle
    EGLNativeWindowType eglNativeWindow;

    /// EGL display
    EGLDisplay eglDisplay;

    /// EGL context
    EGLContext eglContext;

    /// EGL surface
    EGLSurface eglSurface;
#endif

    /// ESScene scene
    ESScene *scene;
};

#endif //ESCONTEXT_H
