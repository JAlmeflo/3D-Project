#pragma once

#include <D3D11.h>
#include <D3DX10math.h>
#include "Texture.h"
#include "OBJReader.h"

class ParticleModel
{
private:
	struct VertexType
	{
		D3DXVECTOR3 position;
		D3DXVECTOR2 texture;
		D3DXVECTOR3 color;
	};

	struct InstanceType
	{
	};

public:
	ParticleModel();
	~ParticleModel();

	bool Initialize(ID3D11Device*, char*, LPCSTR, int);
	void Shutdown();
	void Render(ID3D11DeviceContext*);

	int GetVertexCount();
	int GetInstanceCount();
	ID3D11ShaderResourceView* GetTexture();
	
	void SetPosition(float, float, float);
	D3DXVECTOR3 GetPosition();

private:
	bool InitializeBuffers(ID3D11Device*, int);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext*);

	bool LoadTexture(ID3D11Device*, LPCSTR);
	void ReleaseTexture();

	bool LoadModel(char*);

	ID3D11Buffer *m_vertexBuffer, *m_instanceBuffer;
	int m_vertexCount, m_instanceCount;
	Texture* m_texture;
	OBJReader reader;
	D3DXVECTOR3 m_position;
};

