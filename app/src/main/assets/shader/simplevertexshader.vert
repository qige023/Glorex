#version 300 es  

layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec3 VertexNormal;
layout (location = 2) in vec2 VertexTexCoord;

out vec4 VertexColor;

const vec4 multiple = vec4( 2 );

uniform mat4 MVP;

void main()
{
    VertexColor = vec4(VertexPosition,1.0) * multiple;
    gl_Position = MVP * vec4(VertexPosition,1.0);
}
