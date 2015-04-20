#ifndef TORUSDIFFUSE_H
#define TORUSDIFFUSE_H

#include "vbotorus.h"

#include "util.h"
#include "esscene.h"
#include "esprogram.h"

#include <glm/glm.hpp>
using glm::mat4;

class TorusDiffuse : public ESScene
{
private:
    ESProgram prog;

    VBOTorus *torus;

    float angle;

    mat4 model;
    mat4 view;
    mat4 projection;

    void setMatrices();
    void compileAndLinkShader();

public:
    TorusDiffuse();
    ~TorusDiffuse();

    void initScene(ESContext *esContext);
    void update(ESContext *esContext, float t );
    void render(ESContext *esContext);
    void resize(ESContext *esContext);
};

#endif // TORUSDIFFUSE_H
