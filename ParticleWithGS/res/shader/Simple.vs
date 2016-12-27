attribute vec4 pos;

uniform mat4 M;
uniform mat4 V;

void main()
{

    gl_Position = V * M * pos;
}