#version 330

uniform sampler2D u_Texture;

in vec4 fragColor;

layout(location = 0) out vec4 color;

void main() 
{
	color = fragColor;
}