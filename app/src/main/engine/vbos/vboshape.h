#ifndef VBOSHAPE_H
#define VBOSHAPE_H

class VBOShape {

private:
    unsigned int vaoHandle;
    unsigned int vboHandle;
    unsigned int vboElementHandle;
    unsigned int vertexLength;
    unsigned int vertexIndexLength;

public:

    VBOShape::VBOShape(float *vertexArray, bool enableNormal,
            bool enableTexcoord, bool enableTexcolor, GLsizei stride,
            GLuint *vertexIndex);
    ~VBOShape();

    void render();
};

#endif // VBOSHAPE_H
