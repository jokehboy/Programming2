#version 330 
in vec2 TexCoord;
out vec4 color;

// Texture samplers
uniform sampler2D ourTexture1;

void main()
{
    color = texture(ourTexture1, vec2(TexCoord.x, 1.0-TexCoord.y));	
}