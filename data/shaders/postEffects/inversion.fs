#version 330 core
out vec4 color;
  
in vec2 v_TexCoords;

uniform sampler2D u_Texture;

void main()
{ 
    color = vec4(vec3(1.0 - texture(u_Texture, v_TexCoords)), 1.0);
}