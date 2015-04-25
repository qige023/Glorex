#ifndef VBOSHAPE_H
#define VBOSHAPE_H

#include "esutil.h"
#include "drawable.h"

class VBOShape : public Drawable {

private:
    GLuint vaoHandle;
    GLuint vboHandle;
    GLuint vboElementHandle;
    GLsizei vertexLength;
    GLsizei vertexIndexLength;

    void init(GLfloat *vertexArray, GLsizei vertexLength, GLuint *vertexIndex, GLsizei vertexIndexLength,
            bool enableNormal, bool enableTexcoord, bool enableTexcolor);

public:

    VBOShape(GLfloat *vertexArray, GLsizei vertexLength, GLuint *vertexIndex, GLsizei vertexIndexLength,
            bool enableNormal, bool enableTexcoord, bool enableTexcolor);

    VBOShape(GLfloat *vertexArray, GLsizei vertexLength,
                bool enableNormal, bool enableTexcoord, bool enableTexcolor);

    ~VBOShape();

    void render() const;
};

#endif // VBOSHAPE_H
