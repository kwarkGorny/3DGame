#version 330 core

in vec2 v_TexCoord;

layout(location = 0) out vec4 color;

uniform sampler2D u_Texture;
uniform vec4 u_Color;

void main()
{	
	color = u_Color * texture(u_Texture, v_TexCoord);
}
 