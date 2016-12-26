#version 430

struct Vertex
{
	vec4 pos;
	vec4 texcoord;
	vec4 normal;
};

layout(std140, binding = 0) buffer Mesh
{
	Vertex vertexes[];
} mesh;

uniform mat4 M;
uniform mat4 V;
uniform mat4 P;

varying vec2 V_Texcoord;

void main()
{
	int spriteID = gl_VertexID >> 2;
	vec4 spritePos = vec4(mesh.vertexes[spriteID].pos.xyz, 1.0);
	vec4 viewPos = V * M * spritePos;
	V_Texcoord = vec2( ((gl_VertexID - 1) & 2) >> 1, (gl_VertexID & 2) >> 1);
	float r = 0.1;
	vec4 fixViewPos = viewPos + vec4( vec2(V_Texcoord.x - 0.5, V_Texcoord.y - 0.5) * r , 0.0, 1.0);

    gl_Position = P * fixViewPos;
}