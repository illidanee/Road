#include "Model.h"
#include "Misc.h"

#include <sstream>
#include <vector>

#define LINE_SIZE 1024

struct VertexInfo
{
	float v[3];
};
struct VertexDefine
{
	long vIndex;
	long tIndex;
	long nIndex;
};

int LoadObjModel(const char* filePath, Vertex** vertexes, unsigned int& vertexCount, unsigned int** indexes, unsigned int& indexCount)
{
	char* fileContext = LoadFileContent(filePath);
	if (fileContext == 0)
		return -1;

	std::vector<VertexInfo> vList;
	std::vector<VertexInfo> tList;
	std::vector<VertexInfo> nList;

	std::vector<unsigned int> indexList;
	std::vector<VertexDefine> vertexList;

	std::stringstream sFile(fileContext);
	char oneLine[LINE_SIZE];
	while (!sFile.eof())
	{
		memset(oneLine, 0, LINE_SIZE);
		sFile.getline(oneLine, LINE_SIZE);
		if (strlen(oneLine) > 0)
		{
			std::stringstream sLine(oneLine);
			std::string tmpString;
			if (oneLine[0] == 'v')
			{
				if (oneLine[1] == 't')
				{
					
					sLine >> tmpString;
					VertexInfo tInfo;
					sLine >> tInfo.v[0];
					sLine >> tInfo.v[1];
					tList.push_back(tInfo);

					//printf("vt : %f %f \n", tInfo.v[0], tInfo.v[1]);
				}
				else if (oneLine[1] == 'n')
				{
					sLine >> tmpString;
					VertexInfo nInfo;
					sLine >> nInfo.v[0];
					sLine >> nInfo.v[1];
					sLine >> nInfo.v[2];
					nList.push_back(nInfo);

					//printf("vn : %f %f %f \n", nInfo.v[0], nInfo.v[1], nInfo.v[2]);
				}
				else
				{
					sLine >> tmpString;
					VertexInfo vInfo;
					sLine >> vInfo.v[0];
					sLine >> vInfo.v[1];
					sLine >> vInfo.v[2];
					vList.push_back(vInfo);

					//printf("v : %f %f %f \n", vInfo.v[0], vInfo.v[1], vInfo.v[2]);
				}
			}
			else if (oneLine[0] == 'f')
			{
				//std::cout << oneLine << std::endl;
				sLine >> tmpString;
				std::string subString;
				for (int i = 0; i < 3; ++i)
				{
					sLine >> subString;
					int pos1 = (int)subString.find_first_of('/');
					std::string vIndexString = subString.substr(0, pos1);
					int pos2 = (int)subString.find_first_of('/', pos1 + 1);
					std::string tIndexString = subString.substr(pos1 + 1, pos2 - pos1 - 1);
					std::string nIndexString = subString.substr(pos2 + 1, subString.length() - pos2 - 1);

					VertexDefine vd;
					vd.vIndex = atoi(vIndexString.c_str()) - 1;
					vd.tIndex = atoi(tIndexString.c_str()) - 1;
					vd.nIndex = atoi(nIndexString.c_str()) - 1;

					int index = -1;
					for (int j = 0; j < vertexList.size(); ++j)
					{
						if (vertexList[j].vIndex == vd.vIndex &&
							vertexList[j].tIndex == vd.tIndex &&
							vertexList[j].nIndex == vd.nIndex)
						{
							index = j;
							break;
						}
					}

					if (index == -1)
					{
						index = (int)vertexList.size();
						vertexList.push_back(vd);
					}

					indexList.push_back(index);
				}
			}
		}
	}

	indexCount = (unsigned int)indexList.size();
	*indexes = new unsigned int[indexCount];
	for (unsigned int i = 0; i < indexCount; ++i)
	{
		(*indexes)[i] = indexList[i];
	}

	vertexCount = (unsigned int)vertexList.size();
	*vertexes = new Vertex[vertexCount];
	for (unsigned int i = 0; i < vertexCount; ++i)
	{
		memcpy((*vertexes)[i].pos, vList[vertexList[i].vIndex].v, sizeof(float) * 3);
		memcpy((*vertexes)[i].texcoord, tList[vertexList[i].tIndex].v, sizeof(float) * 2);
		memcpy((*vertexes)[i].normal, nList[vertexList[i].nIndex].v, sizeof(float) * 3);
	}

	delete[] fileContext;
	return 0;
}