#version 330 core

layout(points)in;
layout(triangle_strip, max_vertices = 4)out;

uniform mat4 P;

in vec4 VS_Color[];

out vec2 GS_Texcoord;
out vec4 GS_Color;

void main()
{
	vec4 pos=gl_in[0].gl_Position;
	float r = 0.5;

	gl_Position = P * (pos + vec4(-r, -r, 0.0, 1.0));
	GS_Texcoord = vec2(0.0, 0.0);
	GS_Color = VS_Color[0];
	EmitVertex();

	gl_Position = P * (pos + vec4(r, -r, 0.0, 1.0));
	GS_Texcoord = vec2(1.0, 0.0);
	GS_Color = VS_Color[0];
	EmitVertex();
	
	gl_Position = P * (pos + vec4(-r, r, 0.0, 1.0));
	GS_Texcoord = vec2(0.0, 1.0);
	GS_Color = VS_Color[0];
	EmitVertex();
	
	gl_Position = P * (pos + vec4(r, r, 0.0, 1.0));
	GS_Texcoord = vec2(1.0, 1.0);
	GS_Color = VS_Color[0];
	EmitVertex();
	
	EndPrimitive();
}