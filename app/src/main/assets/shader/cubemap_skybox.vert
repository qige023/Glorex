#version 300 es

layout (location = 0) in vec3 position;
out vec3 TexCoords;

uniform mat4 MVP;

void main()
{
    gl_Position =   MVP * vec4(position, 1.0);  
    TexCoords = position;
}  
