#version 330 core
out vec4 color;
  
in vec2 v_TexCoords;

uniform sampler2D u_Texture;

void main()
{ 
    vec4 textColor = texture(u_Texture, v_TexCoords);
    float average = 0.2126 * textColor.r + 0.7152 * textColor.g + 0.0722 * textColor.b;
    color = vec4(average, average, average, textColor.a);
}