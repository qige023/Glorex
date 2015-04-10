// The MIT License (MIT)
//
// Copyright (c) 2013 Dan Ginsburg, Budirijanto Purnomo
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

//
// Book:      OpenGL(R) ES 3.0 Programming Guide, 2nd Edition
// Authors:   Dan Ginsburg, Budirijanto Purnomo, Dave Shreiner, Aaftab Munshi
// ISBN-10:   0-321-93388-5
// ISBN-13:   978-0-321-93388-1
// Publisher: Addison-Wesley Professional
// URLs:      http://www.opengles-book.com
//            http://my.safaribooksonline.com/book/animation-and-3d/9780133440133
//
// esUtil_Android.c
//
//    This file contains the Android implementation of the windowing functions.


///
// Includes
//
#include <android/log.h>
#include <android/keycodes.h>
#include <android/input.h>
#include <android_native_app_glue.h>
#include <time.h>
#include "esUtil.h"

#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "esUtil", __VA_ARGS__))


//////////////////////////////////////////////////////////////////
//
//  Private Functions
//
//

///
// GetCurrentTime()
//
static float GetCurrentTime()
{
   struct timespec clockRealTime;
   clock_gettime ( CLOCK_MONOTONIC, &clockRealTime );
   double curTimeInSeconds = clockRealTime.tv_sec + ( double ) clockRealTime.tv_nsec / 1e9;
   return ( float ) curTimeInSeconds;
}

///
// HandleCommand()
//
//    Android callback for onAppCmd
//
static void HandleCommand ( struct android_app *pApp, int32_t cmd )
{
   ESContext *esContext = ( ESContext * ) pApp->userData;

   switch ( cmd )
   {
      case APP_CMD_SAVE_STATE:
         // the OS asked us to save the state of the app
         break;

      case APP_CMD_INIT_WINDOW:
         esContext->platformData = ( void * ) pApp->activity->assetManager;

         esContext->eglNativeDisplay = EGL_DEFAULT_DISPLAY;
         esContext->eglNativeWindow = pApp->window;

         // Call the main entrypoint for the app
         if ( esMain ( esContext ) != GL_TRUE )
         {
            exit ( 0 ); //@TEMP better way to exit?
         }

         break;

      case APP_CMD_TERM_WINDOW:
         // Cleanup on shutdown
         if ( esContext->shutdownFunc != NULL )
         {
            esContext->shutdownFunc ( esContext );
         }

         if ( esContext->userData != NULL )
         {
            free ( esContext->userData );
         }

         memset ( esContext, 0, sizeof ( ESContext ) );
         break;

      case APP_CMD_LOST_FOCUS:
         // if the app lost focus, avoid unnecessary processing (like monitoring the accelerometer)
         break;

      case APP_CMD_GAINED_FOCUS:
         // bring back a certain functionality, like monitoring the accelerometer
         break;
   }
}

///
// HandleInputEvent()
//
//    Android callback for onAppInput
//
static int32_t onAppInput(struct android_app* app, AInputEvent* event)
{
    //获取消息的类型,消息有两种类型，一种是Motion类型一种是key类型。
    int32_t lEventType = AInputEvent_getType(event);
    switch(lEventType) {
        case AINPUT_EVENT_TYPE_MOTION:
            //motion类型的消息的来源有两种，所以要获取消息的来源
            switch(AInputEvent_getSource(event)) {
                case AINPUT_SOURCE_TOUCHSCREEN:
                     //消息来源于触摸屏，再判断具体是什么消息;
                    switch(AMotionEvent_getAction(event))
                    {
                        //触摸移动消息
                        case AMOTION_EVENT_ACTION_MOVE:
                            LOGI("touch move");
                        break;
                        //触摸按下消息
                        case AMOTION_EVENT_ACTION_DOWN:
                            LOGI("touch down");
                        break;
                        //触摸弹起消息
                        case AMOTION_EVENT_ACTION_UP:
                            LOGI("touch up");
                        break;
                    }
                break;
                case AINPUT_SOURCE_TRACKBALL:
                //消息来源于trackball
                break;
            }
            break;
        case AINPUT_EVENT_TYPE_KEY:
            //消息来源于物理键盘或虚拟键盘，这个处理是一样的
            switch (AKeyEvent_getKeyCode(event))
                case AKEYCODE_BACK:
                    return 0;
            break;
        default:
            return 0;
    }
}

///
//  Global extern.  The application must declare this function
//  that runs the application.
//
extern int esMain ( ESContext *esContext );

//////////////////////////////////////////////////////////////////
//
//  Public Functions
//
//

///
//  android_main()
//
//    Main entrypoint for Android application
//
void android_main ( struct android_app *pApp )
{
   ESContext esContext;
   float lastTime;

   // Make sure glue isn't stripped.
   app_dummy();

   // Initialize the context
   memset ( &esContext, 0, sizeof ( ESContext ) );

   pApp->onAppCmd = HandleCommand;
   pApp->userData = &esContext;

   //设置输入事件的处理函数，如触摸响应
   pApp->onInputEvent = &onAppInput;

   lastTime = GetCurrentTime();

   while ( 1 )
   {
      int ident;
      int events;
      struct android_poll_source *pSource;

      while ( ( ident = ALooper_pollAll ( 0, NULL, &events, ( void ** ) &pSource ) ) >= 0 )
      {

         if ( pSource != NULL )
         {
            pSource->process ( pApp, pSource );
         }

         if ( pApp->destroyRequested != 0 )
         {
            return;
         }

      }

      if ( esContext.eglNativeWindow == NULL )
      {
         continue;
      }

      // Call app update function
      if ( esContext.updateFunc != NULL )
      {
         float curTime = GetCurrentTime();
         float deltaTime =  ( curTime - lastTime );
         lastTime = curTime;
         esContext.updateFunc ( &esContext, deltaTime );
      }

      if ( esContext.drawFunc != NULL )
      {
         esContext.drawFunc ( &esContext );
         eglSwapBuffers ( esContext.eglDisplay, esContext.eglSurface );
      }
   }
}

///
//  WinCreate()
//
//      Create Win32 instance and window
//
GLboolean WinCreate ( ESContext *esContext, const char *title )
{
   // On Android, this does not need to do anything.  Everything happens from
   // android_main()
   return GL_TRUE;
}
