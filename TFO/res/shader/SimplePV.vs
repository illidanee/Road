attribute vec4 pos;

uniform mat4 P;
uniform mat4 V;

void main()
{
    gl_Position = P * V * pos;
}