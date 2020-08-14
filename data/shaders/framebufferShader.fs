#version 330 core
out vec4 color;
  
in vec2 v_TexCoords;

uniform sampler2D u_Texture;

void main()
{ 
    color = texture(u_Texture, v_TexCoords);
}