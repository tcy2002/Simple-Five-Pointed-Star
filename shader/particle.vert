#version 330

layout (location = 1) in vec3 aPosition;
layout (location = 2) in vec4 aColor;
layout (location = 3) in vec2 aTexCoord;

out vec4 Color;
out vec2 TexCoord;

void main() {
    gl_Position = vec4(aPosition, 1.0);
    TexCoord = aTexCoord;
    Color = aColor;
}
