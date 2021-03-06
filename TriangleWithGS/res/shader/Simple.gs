#version 330 core
layout(points)in;
layout(triangle_strip, max_vertices = 3)out;

void main()
{
	vec4 pos=gl_in[0].gl_Position;

	gl_Position = pos;
	EmitVertex();

	gl_Position = pos+vec4(0.5,0.0,0.0,1.0);
	EmitVertex();

	gl_Position = pos + vec4( 0.0, 0.5, 0.0, 1.0);
	EmitVertex();

	EndPrimitive();
}