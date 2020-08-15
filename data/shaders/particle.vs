#version 330

uniform mat4 u_View;
uniform mat4 u_Projection;

layout(location = 0) in vec4 v_Vertex;
layout(location = 1) in vec4 v_Color;

out vec4 fragColor;

void main() 
{
	fragColor = v_Color;

    vec4 eyePos = u_View * v_Vertex;
    gl_Position = u_Projection * eyePos;

	float dist = length(eyePos.xyz);
	float att = inversesqrt(0.1f * dist);
	gl_PointSize = 10.f;
}