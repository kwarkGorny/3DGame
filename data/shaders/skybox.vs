#version 330 core
layout (location = 0) in vec3 position;

out vec3 v_TexCoord;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;

out vec4 fragColor;

void main()
{
    v_TexCoord = position;
    vec4 pos = u_Projection * u_View * u_Model * vec4(position, 1.0);
    gl_Position = pos.xyww;
}