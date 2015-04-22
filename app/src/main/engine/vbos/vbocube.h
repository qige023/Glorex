#ifndef VBOCUBE_H
#define VBOCUBE_H

class VBOCube
{

private:
    unsigned int vaoHandle;
    float size;

public:
    VBOCube(float nsize = 1.0f);

    void render();
};

#endif // VBOCUBE_H
