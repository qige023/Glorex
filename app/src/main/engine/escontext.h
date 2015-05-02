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

//Forward Declartion
class ESScene;

// Touch Event Interface
class IOnMotionListener {
public:
  virtual void onMotionDown(int32_t pointerId, float downX, float downY) = 0;
  virtual void onMotionMove(int32_t pointerId, float downX, float downY) = 0;
  virtual void onMotionUp(int32_t pointerId, float downX, float downY) = 0;
};

class ESContext {
public:
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

    IOnMotionListener *onMotionListener;
};

#endif //ESCONTEXT_H
