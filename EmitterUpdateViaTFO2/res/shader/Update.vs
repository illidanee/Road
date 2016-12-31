attribute vec4 pos;
attribute vec4 mess;

out vec4 VS_Mess;

void main()
{
	gl_Position = pos + vec4(0.0, 0.01, 0.0, 0.0);
	VS_Mess = mess + vec4(0.016, 0.0, 0.0, 0.0);
}