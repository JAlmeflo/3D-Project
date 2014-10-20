#pragma once

#include <D3D11.h>
#include <D3DX10math.h>
#include <fstream>
#include <iostream>
#include "Texture.h"
#include "OBJReader.h"

using namespace std;

class Model
{
private:
	struct VertexType
	{
		D3DXVECTOR3 position;
		D3DXVECTOR2 texture;
		D3DXVECTOR3 normal;
	};

	struct InstanceType
	{
		D3DXVECTOR3 position;
	};

	struct ModelType
	{
		float x, y, z;
		float tu, tv;
		float nx, ny, nz;
	};
public:
	Model();
	~Model();

	bool Initialize(ID3D11Device*, char*, LPCSTR, int);
	void Shutdown();
	void Render(ID3D11DeviceContext*);

	int GetVertexCount();
	int GetInstanceCount();
	ID3D11ShaderResourceView* GetTexture();

	void SetPosition(float, float, float);
	D3DXVECTOR3 GetPosition();

	void SetNewInstanceObject(ID3D11Device*);
private:
	bool InitializeBuffers(ID3D11Device*, int);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext*);

	bool LoadTexture(ID3D11Device*, LPCSTR);
	void ReleaseTexture();

	bool LoadModel(char*);
	void ReleaseModel();

	ID3D11Buffer *m_vertexBuffer, *m_instanceBuffer;
	int m_vertexCount, m_instanceCount;
	Texture* m_texture;
	ModelType* m_model;
	OBJReader reader;
	D3DXVECTOR3 m_position;
	InstanceType* m_instances;
};

