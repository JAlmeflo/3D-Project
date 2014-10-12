#pragma once

#include <Windows.h>
#include <fstream>
#include <vector>
#include <D3DX10math.h>

struct Face
{
	Face()
	{
	}

	Face(D3DXVECTOR3 p_v1, D3DXVECTOR3 p_v2, D3DXVECTOR3 p_v3, D3DXVECTOR2 p_vt1, D3DXVECTOR2 p_vt2, D3DXVECTOR2 p_vt3, D3DXVECTOR3 p_n1, D3DXVECTOR3 p_n2, D3DXVECTOR3 p_n3)
	{
		vertex1 = p_v1;
		vertex2 = p_v2;
		vertex3 = p_v3;

		texture1 = p_vt1;
		texture2 = p_vt2;
		texture3 = p_vt3;

		normal1 = p_n1;
		normal2 = p_n2;
		normal3 = p_n3;
	}
	D3DXVECTOR3 vertex1, vertex2, vertex3;

	D3DXVECTOR2 texture1, texture2, texture3;

	D3DXVECTOR3 normal1, normal2, normal3;
};

struct Vertex
{
	D3DXVECTOR3 vertex;
	D3DXVECTOR2 texture;
	D3DXVECTOR3 normal;
};

struct int3
{
	int x, y, z;

	int3()
	{
		x = 0;
		y = 0;
		z = 0;
	}

	int3(int p_x, int p_y, int p_z)
	{
		x = p_x;
		y = p_y;
		z = p_z;
	}
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
	std::vector<Face> GetFaces();
private:
	void ResetVectors();
	void ReadVertexPos(std::ifstream& file);
	void ReadTexCoords(std::ifstream& file);
	void ReadNormals(std::ifstream& file);
	void ReadFaces(std::ifstream& file);
	int3 ConvertSubFace(std::string);

	std::vector<D3DXVECTOR3> m_vertices;
	std::vector<D3DXVECTOR2> m_texCoords;
	std::vector<D3DXVECTOR3> m_normals;
	std::vector<Face> m_faces;
};

