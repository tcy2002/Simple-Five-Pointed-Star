#version 330

layout (location = 1) in vec3 aPosition;

void main() {
    gl_Position = vec4(aPosition, 1.0);
}