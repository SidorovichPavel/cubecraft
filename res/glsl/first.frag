#version 330

in vec2 TexCoord;

out vec4 Result;

uniform sampler2D ourTexture;

void main()
{
    Result = texture(ourTexture, TexCoord);
}