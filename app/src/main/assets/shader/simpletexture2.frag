#version 300 es

precision mediump float;

in vec2 v_texCoord;
layout (location=0) out vec4 FragColor;

uniform sampler2D SampleTexture1;
uniform sampler2D SampleTexture2;

void main() {
    // Linearly interpolate between both textures (second texture is only slightly combined)
    FragColor = mix(texture(SampleTexture1, v_texCoord), texture(SampleTexture2, v_texCoord), 0.2);
}
