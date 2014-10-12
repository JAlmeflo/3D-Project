#include "OBJReader.h"


OBJReader::OBJReader()
{
}


OBJReader::~OBJReader()
{
}

bool OBJReader::Load(char* filename)
{
	ResetVectors();

	std::ifstream file;
	file.open(filename);

	if (!file)
	{
		MessageBox(0, "Could not find OBJ file", filename, MB_ICONERROR | MB_OK);
		return false;
	}


	char* charp = new char();
	std::string str;
	while (!file.eof())
	{
		file >> charp;
		str = charp;
		if (str == "v")
			ReadVertexPos(file);
		else if (str == "vt")
			ReadTexCoords(file);
		else if (str == "vn")
			ReadNormals(file);
		else if (str == "f")
			ReadFaces(file);
	}

	return true;
}

void OBJReader::ResetVectors()
{
	m_vertices = std::vector<D3DXVECTOR3>();
	m_texCoords = std::vector<D3DXVECTOR2>();
	m_normals = std::vector<D3DXVECTOR3>();
	m_faces = std::vector<Face>();
}

void OBJReader::ReadVertexPos(std::ifstream& file)
{
	D3DXVECTOR3 vertex;

	file >> vertex.x;
	file >> vertex.y;
	file >> vertex.z;
	// Go from right to left handed coordinates
	vertex.z *= -1;

	m_vertices.push_back(vertex);
}

void OBJReader::ReadTexCoords(std::ifstream& file)
{
	D3DXVECTOR2 texCoord;

	file >> texCoord.x;
	file >> texCoord.y;

	m_texCoords.push_back(texCoord);
}

void OBJReader::ReadNormals(std::ifstream& file)
{
	D3DXVECTOR3 normal;

	file >> normal.x;
	file >> normal.y;
	file >> normal.z;

	m_normals.push_back(normal);
}

void OBJReader::ReadFaces(std::ifstream& file)
{
	char* cp = new char();
	std::string str;
	int3 subFace;
	Face face;

	file >> cp;
	str = cp;	
	subFace = ConvertSubFace(str);
	face.vertex1 = m_vertices[subFace.x - 1];
	face.texture1 = m_texCoords[subFace.y - 1];
	face.normal1 = m_normals[subFace.z - 1];

	file >> cp;
	str = cp;
	subFace = ConvertSubFace(str);
	face.vertex2 = m_vertices[subFace.x - 1];
	face.texture2 = m_texCoords[subFace.y - 1];
	face.normal2 = m_normals[subFace.z - 1];

	file >> cp;
	str = cp;
	subFace = ConvertSubFace(str);
	face.vertex3 = m_vertices[subFace.x - 1];
	face.texture3 = m_texCoords[subFace.y - 1];
	face.normal3 = m_normals[subFace.z - 1];

	m_faces.push_back(face);
}

int3 OBJReader::ConvertSubFace(std::string str)
{
	int3 face;
	std::string string1 = "", string2 = "", string3 = "";
	int n = 0;
	for (unsigned int i = 0; i < str.size(); i++)
	{
		char token = str[i];
		if (token == '/')
		{
			n++;
		}
		else
		{
			switch (n)
			{
			case 0:
				string1 += token;
				break;
			case 1:
				string2 += token;
				break;
			case 2:
				string3 += token;
				break;
			}
		}
	}

	face.x = atoi(string1.c_str());
	face.y = atoi(string2.c_str());
	face.z = atoi(string3.c_str());

	return face;
}

std::vector<D3DXVECTOR3> OBJReader::GetVertices()
{
	return m_vertices;
}

std::vector<D3DXVECTOR2> OBJReader::GetTexCoords()
{
	return m_texCoords;
}

std::vector<D3DXVECTOR3> OBJReader::GetNormals()
{
	return m_normals;
}

std::vector<Face> OBJReader::GetFaces()
{
	return m_faces;
}