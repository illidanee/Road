#pragma once

struct Vertex
{
	float pos[4];
	float texcoord[4];
	float normal[4];
};

int LoadObjModel(const char* filePath, Vertex** vertexes, unsigned int& vertexCount, unsigned int** indexes, unsigned int& indexCount);