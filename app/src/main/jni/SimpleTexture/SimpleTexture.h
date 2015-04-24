#ifndef SimpleTexture_H
#define SimpleTexture_H

#include "vbos/vbocube.h"

#include "esscene.h"
#include "esprogram.h"

#include <glm/glm.hpp>
using glm::mat4;

class SimpleTexture : public ESScene
{
private:
    ESProgram prog;

    VBOCube *cube;

    GLuint texId1;
    GLuint texId2;

    float angle;

    mat4 model;
    mat4 view;
    mat4 projection;

    void setMatrices();
    void compileAndLinkShader();

public:
    SimpleTexture();
    ~SimpleTexture();

    void initScene(ESContext *esContext);
    void update(ESContext *esContext, float t );
    void render(ESContext *esContext);
    void resize(ESContext *esContext);
};

#endif // SimpleTexture_H
