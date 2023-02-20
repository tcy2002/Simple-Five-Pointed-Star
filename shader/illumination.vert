#version 330

layout (location = 1) in vec3 aPosition;
layout (location = 2) in vec3 aNormal;
layout (location = 3) in vec2 aTexCoord;

out vec3 Position;
out vec3 Normal;
out vec2 TexCoord;

void main()
{
    gl_Position = vec4(aPosition, 1.0);
    Position = aPosition;
    Normal = aNormal;
    TexCoord = aTexCoord;
}