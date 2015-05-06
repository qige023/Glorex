#version 300 es  

layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec3 VertexNormal;
layout (location = 2) in vec2 VertexTexCoord;

out vec2 v_texCoord;

uniform mat4 MVP;

void main()
{
    v_texCoord = VertexTexCoord;
    gl_Position = MVP * vec4(VertexPosition,1.0);
}
