attribute vec4 pos;
attribute vec4 mess;

uniform mat4 M;

out vec4 O_Mess;

void main()
{
	gl_Position = M * pos;
	O_Mess = mess;
}