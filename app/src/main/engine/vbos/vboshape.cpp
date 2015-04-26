#include "vboshape.h"

VBOShape::VBOShape(GLfloat *vertexArray, GLsizei vertexLength,
        bool enableNormal, bool enableTexcoord, bool enableTexcolor){
    init(vertexArray, vertexLength, NULL, 0, enableNormal, enableTexcoord, enableTexcolor);

}

VBOShape::VBOShape(GLfloat *vertexArray, GLsizei vertexLength, GLuint *vertexIndex, GLsizei vertexIndexLength,
        bool enableNormal, bool enableTexcoord, bool enableTexcolor) {
    init(vertexArray, vertexLength, vertexIndex, vertexIndexLength, enableNormal, enableTexcoord, enableTexcolor);
}

void VBOShape::init(GLfloat *vertexArray, GLsizei vertexLength, GLuint *vertexIndex, GLsizei vertexIndexLength,
        bool enableNormal, bool enableTexcoord, bool enableTexcolor) {

    this->vertexLength = vertexLength;
    this->vertexIndexLength = vertexIndexLength;

    vaoHandle = vboHandle = vboElementHandle = 0;

    GLsizei stride, currentStride;
    stride = currentStride = 0;
    stride += 3 + (enableNormal ? 3 : 0) + (enableTexcoord ? 2 : 0) + (enableTexcolor ? 3 : 0);

    glGenVertexArrays(1, &vaoHandle);
    glGenBuffers(1, &vboHandle);
    glBindVertexArray(vaoHandle);
    glBindBuffer(GL_ARRAY_BUFFER, vboHandle);
    glBufferData(GL_ARRAY_BUFFER, vertexLength * stride * sizeof(GLfloat), vertexArray, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride * sizeof(GLfloat), (GLvoid*)currentStride);
    currentStride += 3;

    if(enableNormal) {
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride * sizeof(GLfloat), (GLvoid*)(currentStride * sizeof(GLfloat)));
        currentStride +=3;
    }


    if(enableTexcoord) {
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride * sizeof(GLfloat), (GLvoid*)(currentStride * sizeof(GLfloat)));
        currentStride +=2;
    }

    if(enableTexcolor) {
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, stride * sizeof(GLfloat), (GLvoid*)(currentStride * sizeof(GLfloat)));
        currentStride +=3;
    }

    if(vertexIndex != NULL) {
        glGenBuffers(1, &vboElementHandle);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboElementHandle);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, vertexIndexLength * sizeof(GLuint), vertexIndex, GL_STATIC_DRAW);
    }

    glBindVertexArray(0);
}

VBOShape::~VBOShape() {
    glDeleteVertexArrays(1, &vaoHandle);
    glDeleteBuffers(1, &vboHandle);
    glDeleteBuffers(1, &vboElementHandle);
}

void VBOShape::render() const {
    glBindVertexArray(vaoHandle);

    if (vboElementHandle != 0) {
        glDrawElements(GL_TRIANGLES, vertexIndexLength, GL_UNSIGNED_INT, ((GLubyte *)NULL + (0)));
    } else {
        glDrawArrays(GL_TRIANGLES, 0, vertexLength);
    }

}