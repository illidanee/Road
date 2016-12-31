attribute vec4 pos;
attribute vec4 mess;

uniform mat4 V;

out vec4 VS_Color;

void main()
{
	gl_Position = V * pos;
	VS_Color.z = pow(sin(mess.x * 3.14 / 20.0), 32.0);
}