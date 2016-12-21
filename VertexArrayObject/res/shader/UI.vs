attribute vec3 pos;
attribute vec2 texcoord;
attribute vec3 normal;

uniform mat4 M;
uniform mat4 V;
uniform mat4 P;

varying vec2 V_Texture;

void main()
{
	V_Texture = texcoord;

    gl_Position= vec4(pos.x * 2, pos.y * 2, pos.z ,1.0);
}