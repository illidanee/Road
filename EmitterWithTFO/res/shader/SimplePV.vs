attribute vec4 pos;

uniform mat4 P;
uniform mat4 V;

void main()
{
	gl_PointSize = 100;
    gl_Position = P * V * pos;
}