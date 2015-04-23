#version 300 es

precision mediump float;

in vec4 VertexColor;
layout (location=0) out vec4 FragColor;

uniform sampler2D SampleTexture;

void main() {
    FragColor = VertexColor; 
}
