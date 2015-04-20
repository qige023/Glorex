//
//    This file contains the Android implementation of the windowing functions.

///
// Includes
//

#include "esutil.h"
#include <android/fopen_android.h>
#include "android/buf_android.hpp"

#include <android/log.h>
#include <android/keycodes.h>
#include <android/input.h>
#include <android_native_app_glue.h>
#include <time.h>
#include <stdexcept>

using std::cout;
using std::cerr;
using std::endl;

///
//  Global extern.  The application must declare this function
//  that runs the application.
//
extern ESScene *esCreateScene(ESContext *esContext);

static int esMain(ESContext *esContext) {
    cout << "exec esMain..." << endl;
    ESScene *scene = esCreateScene(esContext);
    AAssetManager *assetManager = ( AAssetManager * ) esContext->platformData;
    android_fopen_set_asset_manager(assetManager);

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
        esContext->platformData = (void *) pApp->activity->assetManager;

        esContext->eglNativeDisplay = EGL_DEFAULT_DISPLAY;
        esContext->eglNativeWindow = pApp->window;

        std::cout.rdbuf(new outlogbuf());
        std::cerr.rdbuf(new outlogbuf());

        // Call the main entry point for the app
        if (esMain(esContext) != GL_TRUE) {
            exit(0); //@TEMP better way to exit?
        }

        break;

    case APP_CMD_TERM_WINDOW:
        cout << "HandleCommand APP_CMD_TERM_WINDOW" << endl;
        delete esContext->scene;

        std::cout.rdbuf(0);
        std::cerr.rdbuf(0);

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
static int32_t HandleInput(struct android_app* app, AInputEvent* event) {
    int32_t lEventType = AInputEvent_getType(event);

    switch (lEventType) {
    case AINPUT_EVENT_TYPE_MOTION:
        switch (AInputEvent_getSource(event)) {
        case AINPUT_SOURCE_TOUCHSCREEN:
            switch (AMotionEvent_getAction(event)) {
            case AMOTION_EVENT_ACTION_MOVE:
                cout << "HandleInput AMOTION_EVENT_ACTION_MOVE" << endl;
                break;
            case AMOTION_EVENT_ACTION_DOWN:
                cout << "HandleInput AMOTION_EVENT_ACTION_DOWN" << endl;
                break;
            case AMOTION_EVENT_ACTION_UP:
                cout << "HandleInput AMOTION_EVENT_ACTION_UP" << endl;
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

    while (1) {
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

