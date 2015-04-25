#ifndef BlendingWindow_H
#define BlendingWindow_H

#include "vbos/vbocube.h"
#include "vboshape.h"

#include "esscene.h"
#include "esprogram.h"

#include <vector>
#include <map>

#include <glm/glm.hpp>

using std::vector;
using std::map;

using glm::mat4;
using glm::vec3;
using glm::vec4;

class BlendingWindow : public ESScene {

private:
    ESProgram prog;

    VBOCube *cube;
    VBOShape *floor;
    VBOShape *window;

    GLuint cubeTexture;
    GLuint floorTexture;
    GLuint windowTexture;

    mat4 model;
    mat4 view;
    mat4 projection;

    vec3 cameraPosition;

    vector<vec3> windowPostions;
    map<GLfloat, vec3> sortedWindowPostions;

    void setMatrices();
    void compileAndLinkShader();

public:
    BlendingWindow();
    ~BlendingWindow();

    void initScene(ESContext *esContext);
    void update(ESContext *esContext, float t );
    void render(ESContext *esContext);
    void resize(ESContext *esContext);
};

#endif // BlendingWindow_H
