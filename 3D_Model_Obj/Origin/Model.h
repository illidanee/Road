#pragma once

struct Vertex
{
	float pos[3];
	float texcoord[2];
	float normal[2];
};

int LoadObjModel(const char* filePath, Vertex** vertexes, unsigned int& vertexCount, unsigned int** indexes, unsigned int& indexCount);