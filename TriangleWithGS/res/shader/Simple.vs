attribute vec4 pos;

uniform mat4 M;
uniform mat4 V;
uniform mat4 P;

void main()
{

    gl_Position = P * V * M * pos;
}