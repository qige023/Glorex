#ifndef Cubemap_H
#define Cubemap_H

#include "vbos/vbocube.h"

#include "esscene.h"
#include "esprogram.h"
#include "vbos/vboskybox.h"

#include <vector>
using std::vector;

#include <glm/glm.hpp>
using glm::mat4;

class Cubemap : public ESScene
{
private:
    ESProgram prog;

    VBOCube *cube;
    vector<const GLchar*> skyboxFaces;
    VBOSkybox *skybox;

    GLuint skyboxTexture;

    mat4 model;
    mat4 view;
    mat4 projection;

    void setMatrices();
    void compileAndLinkShader();

public:
    Cubemap();
    ~Cubemap();

    void initScene(ESContext *esContext);
    void update(ESContext *esContext, float t );
    void render(ESContext *esContext);
    void resize(ESContext *esContext);
};

#endif // Cubemap_H
