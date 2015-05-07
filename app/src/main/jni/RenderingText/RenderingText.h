#ifndef RenderingText_H
#define RenderingText_H

#include <map>
using std::map;

#include "glm/glm.hpp"
using glm::mat4;

#include "esutil.h"
#include "esscene.h"
#include "esprogram.h"
#include "escharacter.h"

class RenderingText: public ESScene {

private:
    ESProgram prog;
    ESCharactersHolder *charactersHolder;

    mat4 projection;

    void compileAndLinkShader();

public:
    RenderingText();
    ~RenderingText();

    void initScene(ESContext *esContext);
    void update(ESContext *esContext, float t );
    void render(ESContext *esContext);
    void resize(ESContext *esContext);
};

#endif // RenderingText_H
