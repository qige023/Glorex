#ifndef HELLOTRIANGLE_H
#define HELLOTRIANGLE_H

#include "esutil.h"
#include "esscene.h"
#include "esprogram.h"

#include <glm/glm.hpp>
using glm::mat4;

class HelloTriangle: public ESScene {
private:

    GLuint vboHandles[2];
    GLuint vaoHandle;
    ESProgram prog;

    mat4 projection;

    void compileAndLinkShader();
public:
    HelloTriangle();
    ~HelloTriangle();

    void initScene(ESContext *esContext);
    void update(ESContext *esContext, float t );
    void render(ESContext *esContext);
    void resize(ESContext *esContext);
};

#endif // HELLOTRIANGLE_H
