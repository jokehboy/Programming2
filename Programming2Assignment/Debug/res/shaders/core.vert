#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;
layout (location = 2) in vec2 textureCoord;


out vec3 ourColor;
out vec2 TexCoord;

uniform mat4 transform;

void main(void)
{
    gl_Position = transform * vec4(position, 1.0);

    ourColor = color;

	TexCoord = vec2(textureCoord.x,1.0-textureCoord.y);
}