#ifndef TeapotTwoSide_H
#define TeapotTwoSide_H

#include "vboteapot.h"

#include "esscene.h"
#include "esprogram.h"

#include <glm/glm.hpp>
using glm::mat4;

class TeapotTwoSide: public ESScene {
private:
    ESProgram prog;

    VBOTeapot *teapot;
    float angle;

    mat4 model;
    mat4 view;
    mat4 projection;

    void setMatrices();
    void compileAndLinkShader();

public:
    TeapotTwoSide();

    void initScene(ESContext *esContext);
    void update(ESContext *esContext, float t );
    void render(ESContext *esContext);
    void resize(ESContext *esContext);
};

#endif // TeapotTwoSide_H
