#pragma once

#include <Windows.h>
#include <fstream>
#include <vector>
#include <D3DX10math.h>

struct VertexPoint
{
	D3DXVECTOR3 vertex;
	D3DXVECTOR2 texture;
	D3DXVECTOR3 normal;
};

struct int3
{
	int x, y, z;
};

class OBJReader
{
public:
	OBJReader();
	~OBJReader();

	bool Load(char* filename);
	std::vector<D3DXVECTOR3> GetVertices();
	std::vector<D3DXVECTOR2> GetTexCoords();
	std::vector<D3DXVECTOR3> GetNormals();
    std::vector<VertexPoint> GetVertexPoints();
private:
	void ResetVectors();
	void ReadVertexPos(std::ifstream& file);
	void ReadTexCoord(std::ifstream& file);
	void ReadNormal(std::ifstream& file);
	void ReadFace(std::ifstream& file);
	int3 ConvertFaceValues(std::string);

	std::vector<D3DXVECTOR3> m_vertices;
	std::vector<D3DXVECTOR2> m_texCoords;
	std::vector<D3DXVECTOR3> m_normals;
    std::vector<VertexPoint> m_vertexPoints;
};

