#version 330 core
layout(points)in;
layout(points, max_vertices = 1)out;

in vec4 VS_Mess[];

out vec4 O_Mess;

void main()
{
	float currentAge = VS_Mess[0].x;
	if (currentAge < 20.0)
	{
		O_Mess = VS_Mess[0];
		gl_Position = gl_in[0].gl_Position;
		EmitVertex();
	}
}