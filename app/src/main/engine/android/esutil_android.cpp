//
//    This file contains the Android implementation of the windowing functions.
#include <android/log.h>
#include <android/keycodes.h>
#include <android/input.h>
#include <android_native_app_glue.h>

#include <time.h>
#include <iostream>
#include <stdexcept>

using std::cout;
using std::cerr;
using std::endl;

#include "esutil.h"
#include "esfile.h"
#include "android/buf_android.hpp"

///
//  Global extern.  The application must declare this function
//  that runs the application.
//
extern ESScene *esCreateScene(ESContext *esContext);

static int esMain(ESContext *esContext) {
    cout << "exec esMain..." << endl;
    ESScene *scene = esCreateScene(esContext);
    AAssetManager *assetManager = (( ANativeActivity * ) esContext->platformData)->assetManager;
    ESFileWrapper::setAssetmanager(assetManager);

    if(scene != NULL) {
        esContext->scene = scene;
        GLboolean result = esCreateWindow(esContext, "Simple VertexShader", 320, 240,
        ES_WINDOW_RGB | ES_WINDOW_DEPTH);
        if (result != GL_TRUE) {
           throw new std::runtime_error("Unable to create EGL native window.");
           return false;
        }
        scene->initScene(esContext);
        return TRUE;
    }
    return FALSE;
}

//////////////////////////////////////////////////////////////////
//
//  Private Functions
//
//

///
// GetCurrentTime()
//
static float GetCurrentTime() {
    struct timespec clockRealTime;
    clock_gettime( CLOCK_MONOTONIC, &clockRealTime);
    double curTimeInSeconds = clockRealTime.tv_sec
            + (double) clockRealTime.tv_nsec / 1e9;
    return (float) curTimeInSeconds;
}

///
//    Android callback for onAppCmd
//
static void HandleCommand(struct android_app *pApp, int32_t cmd) {
    ESContext *esContext = (ESContext *) pApp->userData;

    switch (cmd) {
    case APP_CMD_SAVE_STATE:
        cout << "HandleCommand APP_CMD_SAVE_STATE" << endl;
        // the OS asked us to save the state of the app
        break;

    case APP_CMD_INIT_WINDOW:
        cout << "HandleCommand APP_CMD_INIT_WINDOW" << endl;
        esContext->platformData = (void *) pApp->activity;

        esContext->eglNativeDisplay = EGL_DEFAULT_DISPLAY;
        esContext->eglNativeWindow = pApp->window;

        cout.rdbuf(new outlogbuf());
        cerr.rdbuf(new outlogbuf());

        // Call the main entry point for the app
        if (esMain(esContext) != GL_TRUE) {
            exit(0); //@TEMP better way to exit?
        }

        break;

    case APP_CMD_TERM_WINDOW:
        cout << "HandleCommand APP_CMD_TERM_WINDOW" << endl;
        delete esContext->scene;

        cout.rdbuf(0);
        cerr.rdbuf(0);

        //reset esContext
        memset(esContext, 0, sizeof(ESContext));
        break;

    case APP_CMD_LOST_FOCUS:
        cout << "HandleCommand APP_CMD_LOST_FOCUS" << endl;
        // if the app lost focus, avoid unnecessary processing (like monitoring the accelerometer)
        break;

    case APP_CMD_GAINED_FOCUS:
        cout << "HandleCommand APP_CMD_GAINED_FOCUS" << endl;
        // bring back a certain functionality, like monitoring the accelerometer
        break;
    }
}

///
//    Android callback for onAppInput
//
static int32_t HandleInput(struct android_app* pApp, AInputEvent* event) {
    ESContext *esContext = (ESContext *) pApp->userData;

    int32_t eventType = AInputEvent_getType(event);
    int32_t eventSource = AInputEvent_getSource(event);
    int32_t eventAction;

    float motionX, motionY;
    size_t pointerIndex;
    int32_t pointerId ,pointerCount;

    switch (eventType) {
    case AINPUT_EVENT_TYPE_MOTION:
        switch (eventSource) {
        case AINPUT_SOURCE_TOUCHSCREEN:
            eventAction = AMotionEvent_getAction(event) & AMOTION_EVENT_ACTION_MASK;
            pointerIndex = (AMotionEvent_getAction(event) & AMOTION_EVENT_ACTION_POINTER_INDEX_MASK) >> AMOTION_EVENT_ACTION_POINTER_INDEX_SHIFT;
            pointerId = AMotionEvent_getPointerId(event, pointerIndex);
            motionX  = AMotionEvent_getX(event, pointerId);
            motionY  = AMotionEvent_getY(event, pointerId);

            switch (eventAction) {
            case AMOTION_EVENT_ACTION_MOVE:
                //http://stackoverflow.com/questions/9028357/android-multitouch-second-finger-action-move-ignored
                cout << "HandleInput AMOTION_EVENT_ACTION_MOVE" << endl;
                pointerCount = AMotionEvent_getPointerCount(event);
                for(int i = 0; i < pointerCount; ++i) {
                    pointerIndex = i;
                    pointerId = AMotionEvent_getPointerId(event, pointerIndex);
                    motionX  = AMotionEvent_getX(event, pointerId);
                    motionY  = AMotionEvent_getY(event, pointerId);
                    if(esContext->onMotionListener != NULL) {
                        esContext->onMotionListener->onMotionMove(pointerId, motionX, motionY);
                        cout <<"pointerId: "<< pointerId << "  X: " << motionX << "  Y: " << motionY << endl;
                    }
                }
                break;
            case AMOTION_EVENT_ACTION_DOWN:
            case AMOTION_EVENT_ACTION_POINTER_DOWN:
                cout << "HandleInput AMOTION_EVENT_ACTION_POINTER_DOWN" << endl;
                if(esContext->onMotionListener != NULL) {
                    esContext->onMotionListener->onMotionDown(pointerId, motionX, motionY);
                    cout <<"pointerId: "<< pointerId << "  X: " << motionX << "  Y: " << motionY << endl;
                }
                break;
            case AMOTION_EVENT_ACTION_UP:
            case AMOTION_EVENT_ACTION_POINTER_UP:
                cout << "HandleInput AMOTION_EVENT_ACTION_UP" << endl;
                if(esContext->onMotionListener != NULL) {
                    esContext->onMotionListener->onMotionUp(pointerId, motionX, motionY);
                    cout <<"pointerId: "<< pointerId << "  X: " << motionX << "  Y: " << motionY << endl;
                }
                break;
            }
            break;
        case AINPUT_SOURCE_TRACKBALL:
            break;
        }
        return 0;
        break;
    case AINPUT_EVENT_TYPE_KEY:
        switch (AKeyEvent_getKeyCode(event))
        case AKEYCODE_BACK:
            return 0;
        break;
    default:
        return 0;
    }
}

//////////////////////////////////////////////////////////////////
//
//  Public Functions
//

///
//  android_main()
//
//    Main entrypoint for Android application
//
void android_main(struct android_app *pApp) {

    ESContext *esContext = new ESContext();
    float lastTime;

    // Make sure glue isn't stripped.
    app_dummy();

    // Initialize the context
    memset(esContext, 0, sizeof(ESContext));

    pApp->userData = esContext;
    pApp->onAppCmd = HandleCommand;
    //设置输入事件的处理函数，如触摸响应
    pApp->onInputEvent = HandleInput;

    lastTime = GetCurrentTime();

    while (TRUE) {
        int ident;
        int events;
        struct android_poll_source *pSource;

        while ((ident = ALooper_pollAll(0, NULL, &events, (void **) &pSource))
                >= 0) {

            if (pSource != NULL) {
                pSource->process(pApp, pSource);
            }

            if (pApp->destroyRequested != 0) {
                return;
            }
        }

        if (esContext->eglNativeWindow == NULL) {
            continue;
        }

        // Call app update function
        if (esContext->scene != NULL) {
            float curTime = GetCurrentTime();
            float deltaTime = (curTime - lastTime);
            lastTime = curTime;
            esContext->scene->update(esContext, deltaTime);
            esContext->scene->render(esContext);
            eglSwapBuffers(esContext->eglDisplay, esContext->eglSurface);
        }
    }
}

