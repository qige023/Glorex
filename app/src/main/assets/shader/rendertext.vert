#version 300 es  

layout (location=0) in vec3 VertexPosition;

out vec3 Color;

uniform vec3 TextColor;
uniform mat4 MVP;

void main()
{
    Color = TextColor;
    gl_Position = MVP * vec4(VertexPosition,1.0);
}
