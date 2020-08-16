#version 330

uniform mat4 u_View;
uniform mat4 u_Projection;
uniform mat4 u_Model;

layout(location = 0) in vec4 position;
layout(location = 1) in vec4 color;

out vec4 fragColor;

void main() 
{
	fragColor = color;

    vec4 eyePos = u_View * u_Model * position;
    gl_Position = u_Projection * eyePos;

	float dist = length(eyePos.xyz);
	float att = inversesqrt(0.1f * dist);
	gl_PointSize = 10.0f * att;
}