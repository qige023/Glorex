#ifndef SimpleVertexShader_H
#define SimpleVertexShader_H

#include "vbos/vbocube.h"

#include "esscene.h"
#include "esprogram.h"

#include <glm/glm.hpp>
using glm::mat4;

class SimpleVertexShader : public ESScene
{
private:
    ESProgram prog;

    VBOCube *cube;

    GLuint texId;

    float angle;

    mat4 model;
    mat4 view;
    mat4 projection;

    void setMatrices();
    void compileAndLinkShader();

public:
    SimpleVertexShader();
    ~SimpleVertexShader();

    void initScene(ESContext *esContext);
    void update(ESContext *esContext, float t );
    void render(ESContext *esContext);
    void resize(ESContext *esContext);
};

#endif // SimpleVertexShader_H
