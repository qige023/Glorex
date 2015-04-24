#ifndef VBOCUBE_H
#define VBOCUBE_H

class VBOCube
{

private:
    unsigned int vaoHandle;
    unsigned int handle[4];

    float size;

public:
    VBOCube(float nsize = 1.0f);
    ~VBOCube();

    void render();
};

#endif // VBOCUBE_H
