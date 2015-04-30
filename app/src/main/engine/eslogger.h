#pragma once

#include "esutil.h"

#include <iostream>

using std::cout;
using std::endl;

class ESLogger {

public:
    static int checkForOpenGLError(const char * file, int line) {
        //
        // Returns 1 if an OpenGL error occurred, 0 otherwise.
        //
        GLenum glErr;
        int retCode = 0;

        glErr = glGetError();
        while (glErr != GL_NO_ERROR) {
            const char * message = "";
            switch (glErr) {
            case GL_INVALID_ENUM:
                message = "Invalid enum";
                break;
            case GL_INVALID_VALUE:
                message = "Invalid value";
                break;
            case GL_INVALID_OPERATION:
                message = "Invalid operation";
                break;
            case GL_INVALID_FRAMEBUFFER_OPERATION:
                message = "Invalid framebuffer operation";
                break;
            case GL_OUT_OF_MEMORY:
                message = "Out of memory";
                break;
            default:
                message = "Unknown error";
            }

            cout << ("glError in file %s @ line %d: %s\n", file, line, message) << endl;
            retCode = 1;
            glErr = glGetError();
        }
        return retCode;
    }

    static void dumpGLInfo(bool dumpExtensions) {
        const GLubyte *renderer = glGetString(GL_RENDERER);
        const GLubyte *vendor = glGetString(GL_VENDOR);
        const GLubyte *version = glGetString(GL_VERSION);
        const GLubyte *glslVersion = glGetString(GL_SHADING_LANGUAGE_VERSION);

        GLint major, minor;
        glGetIntegerv(GL_MAJOR_VERSION, &major);
        glGetIntegerv(GL_MINOR_VERSION, &minor);

        cout << ("-------------------------------------------------------------\n");
        cout << ("GL Vendor    : %s\n", vendor);
        cout << ("GL Renderer  : %s\n", renderer);
        cout << ("GL Version   : %s\n", version);
        cout << ("GL Version   : %d.%d\n", major, minor);
        cout << ("GLSL Version : %s\n", glslVersion);
        cout << ("-------------------------------------------------------------\n") << endl;

        if (dumpExtensions) {
            GLint nExtensions;
            glGetIntegerv(GL_NUM_EXTENSIONS, &nExtensions);
            for (int i = 0; i < nExtensions; i++) {
                cout << ("%s\n", glGetStringi(GL_EXTENSIONS, i)) << endl;
            }
        }
    }

};
