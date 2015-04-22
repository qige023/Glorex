#version 300 es

precision mediump float;

in vec2 v_texCoord;
layout (location=0) out vec4 FragColor;

uniform sampler2D SampleTexture;

void main() {
    FragColor = texture( SampleTexture, v_texCoord ); 
}
