#version 330 core

layout(points)in;
layout(triangle_strip, max_vertices = 4)out;

uniform mat4 P;

out vec2 V_Texcoord;

void main()
{
	vec4 pos=gl_in[0].gl_Position;
	float r = 0.5;

	gl_Position = P * (pos + vec4(-r, -r, 0.0, 1.0));
	V_Texcoord = vec2(0.0, 0.0);
	EmitVertex();

	gl_Position = P * (pos + vec4(r, -r, 0.0, 1.0));
	V_Texcoord = vec2(1.0, 0.0);
	EmitVertex();
	
	gl_Position = P * (pos + vec4(-r, r, 0.0, 1.0));
	V_Texcoord = vec2(0.0, 1.0);
	EmitVertex();
	
	gl_Position = P * (pos + vec4(r, r, 0.0, 1.0));
	V_Texcoord = vec2(1.0, 1.0);
	EmitVertex();
	
	EndPrimitive();
}