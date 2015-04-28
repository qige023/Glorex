#version 300 es

precision mediump float;

in vec3 TexCoords;
layout (location=0) out vec4 color;

uniform samplerCube skybox;

void main()
{    
    color = texture(skybox, TexCoords);
}
  