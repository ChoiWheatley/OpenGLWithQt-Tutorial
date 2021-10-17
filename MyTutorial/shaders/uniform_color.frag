#version 330 core

// fragment shader

in vec2 texCoord;       // input from vert shader, texture coordinate
out vec4 finalColor;     // output: final color value as rgba-value

uniform sampler2D ourTexture;

void main() {
    finalColor = texture(ourTexture,     // texture sampler
                         texCoord);     // texture coordinates
}
