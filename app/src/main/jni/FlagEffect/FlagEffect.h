#ifndef FlagEffect_H
#define FlagEffect_H

#include "esscene.h"
#include "esprogram.h"
#include "vboshape.h"

#include <glm/glm.hpp>
using glm::mat4;

class FlagEffect : public ESScene
{
private:
    ESProgram prog;

    VBOShape *flag;

    GLuint flagTexture;
    GLfloat points[45][45][3];    // The Array For The Points On The Grid Of Our "Wave"
    GLint wiggle_count;       // every two frame ,we move the flag

    GLfloat angle;

    mat4 model;
    mat4 view;
    mat4 projection;

    void setMatrices();
    void compileAndLinkShader();
    GLfloat *generateFlagVertexs();

public:
    FlagEffect();
    ~FlagEffect();

    void initScene(ESContext *esContext);
    void update(ESContext *esContext, float t );
    void render(ESContext *esContext);
    void resize(ESContext *esContext);
};

#endif // FlagEffect_H
