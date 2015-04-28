#ifndef VBOSKYBOX_H
#define VBOSKYBOX_H

#include "vboshape.h"

class VBOSkybox : public VBOShape {
protected:
    static const GLsizei Skybox_Vertex_Length = 36;
    static GLfloat vertexArray[Skybox_Vertex_Length * 3];
public:
    VBOSkybox();
    ~VBOSkybox();

    void render() const;
};

#endif //VBOSKYBOX_H
