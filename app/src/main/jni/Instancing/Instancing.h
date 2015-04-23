#ifndef Instancing_H
#define Instancing_H

#include "vbos/vbocube.h"

#include "esscene.h"
#include "esprogram.h"

#include <glm/glm.hpp>

using glm::vec3;
using glm::mat4;

class Instancing : public ESScene
{
private:
    static const int CUBE_NUM = 10;

    ESProgram prog;

    VBOCube *cube;

    GLuint texId;

    float *angle;

    mat4 model;
    mat4 view;
    mat4 projection;
    vec3 *cubePositions;

    void setMatrices();
    void compileAndLinkShader();

public:
    Instancing();
    ~Instancing();

    void initScene(ESContext *esContext);
    void update(ESContext *esContext, float t );
    void render(ESContext *esContext);
    void resize(ESContext *esContext);
};

#endif // Instancing_H
