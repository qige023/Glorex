#include "vboshape.h"

VBOShape::VBOShape(float *vertexArray, bool enableNormal, bool enableTexcoord, bool enableTexcolor, GLsizei stride, GLuint *vertexIndex) {

    vaoHandle = vboHandle = vboElementHandle = NULL;

    vertexLength = sizeof(vertexArray) / (stride * sizeof(GLfloat));
    vertexIndexLength = sizeof(vertexIndex) / (3 * sizeof(GLfloat));

    GLuint currentStride = 0;
    glGenVertexArrays(1, &vaoHandle);
    glGenBuffers(1, &vboHandle);
    glBindVertexArray(vaoHandle);
    glBindBuffer(GL_ARRAY_BUFFER, vboHandle);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexArray), &vertexArray, GL_STATIC_DRAW);

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
        currentStride +=2;
    }

    if(vertexIndex != NULL) {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboElementHandle);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, 36 * sizeof(GLuint), vertexIndex, GL_STATIC_DRAW);
    }

    glBindVertexArray(0);
}

VBOShape::~VBOShape() {
    glDeleteVertexArrays(1, &vaoHandle);
    glDeleteBuffers(1, &vboHandle);
    glDeleteBuffers(1, &vboElementHandle);
}

void VBOShape::render() {
    glBindVertexArray(vaoHandle);

    if (vboElementHandle != NULL) {
        glDrawElements(GL_TRIANGLES, vertexIndexLength, GL_UNSIGNED_INT, ((GLubyte *)NULL + (0)));
    } else {
        glDrawArrays(GL_TRIANGLES, 0, vertexLength);
    }

}
