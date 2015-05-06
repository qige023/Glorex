#ifndef Nanosuit_H
#define Nanosuit_H

#include "esscene.h"
#include "esprogram.h"
#include "esmodel.h"

#include "escamera.h"
#include "layer/controllayer.h"

#include <glm/glm.hpp>
using glm::mat4;

class Nanosuit : public ESScene
{
private:
    ESProgram prog;
    ESModel *nanosuit;
    ESCamera *camera;
    ControlLayer *controlLayer;
    static vec3 pointLightPositions[2];

    mat4 model;
    mat4 view;
    mat4 projection;

    void setMatrices();
    void compileAndLinkShader();

public:
    Nanosuit();
    ~Nanosuit();

    void initScene(ESContext *esContext);
    void update(ESContext *esContext, float t );
    void render(ESContext *esContext);
    void resize(ESContext *esContext);
};

#endif // Nanosuit_H
