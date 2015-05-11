#ifndef VBOSHAPE_H
#define VBOSHAPE_H

#include "esutil.h"
#include "drawable.h"

class VBOShape : public Drawable {

protected:
    GLuint vaoHandle;
    GLuint vboHandle;
    GLuint vboElementHandle;
    GLenum usage;
    GLsizei vertexLength;
    GLsizei vertexIndexLength;

    void init(GLfloat *vertexArray, GLsizei vertexLength, GLuint *vertexIndex, GLsizei vertexIndexLength,
            bool enableNormal, bool enableTexcoord, bool enableTexcolor);

public:

    VBOShape(GLfloat *vertexArray, GLsizei vertexLength, GLuint *vertexIndex, GLsizei vertexIndexLength,
            bool enableNormal, bool enableTexcoord, bool enableTexcolor, GLenum nusage = GL_STATIC_DRAW);

    VBOShape(GLfloat *vertexArray, GLsizei vertexLength,
                bool enableNormal, bool enableTexcoord, bool enableTexcolor, GLenum nusage = GL_STATIC_DRAW);

    ~VBOShape();

    void setBufferSubData(GLenum target, GLintptr offset, GLsizeiptr size, const GLvoid* data);

    void render() const;

    void render(GLenum mode) const;
};

#endif // VBOSHAPE_H
