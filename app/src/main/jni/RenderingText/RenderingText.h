#ifndef RenderingText_H
#define RenderingText_H

#include "esutil.h"
#include "esscene.h"
#include "esprogram.h"
#include <map>
using std::map;
#include <glm/glm.hpp>
using glm::mat4;

/// Holds all state information relevant to a character as loaded using FreeType
struct Character {
    GLuint TextureID;   // ID handle of the glyph texture
    glm::ivec2 Size;    // Size of glyph
    glm::ivec2 Bearing;  // Offset from baseline to left/top of glyph
    GLuint Advance;    // Horizontal offset to advance to next glyph
};

class RenderingText: public ESScene {
private:

    GLuint vboHandle;
    GLuint vaoHandle;
    ESProgram prog;

    mat4 projection;

    map<GLchar, Character> Characters;

    void compileAndLinkShader();
public:
    RenderingText();
    ~RenderingText();

    void initScene(ESContext *esContext);
    void update(ESContext *esContext, float t );
    void render(ESContext *esContext);
    void resize(ESContext *esContext);
    void renderText(std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color);
};

#endif // RenderingText_H
