#pragma once

#include <D3D11.h>
#include <D3DX10math.h>
#include "Texture.h"

class ParticleSystem
{
private:
	struct ParticleType
	{
		float positionX, positionY, positionZ;
		float red, green, blue;
		float velocity;
		bool active;
	};

	struct VertexType
	{
		D3DXVECTOR3 position;
		D3DXVECTOR2 texture;
		D3DXVECTOR4 color;
	};
public:
	ParticleSystem();
	~ParticleSystem();

	bool Initialize(ID3D11Device*, LPCSTR);
	void Shutdown();
	bool Render(ID3D11DeviceContext*);
	void Update(float);

	ID3D11ShaderResourceView* GetTexture();
	int GetIndexCount();

private:
	bool LoadTexture(ID3D11Device*, LPCSTR);
	void ReleaseTexture();

	bool InitializeParticleSystem();
	void ShutdownParticleSystem();

	bool InitializeBuffers(ID3D11Device*);
	void ShutdownBuffers();

	void EmitParticles(float);
	void UpdateParticles(float);
	void KillParticles();

	bool UpdateBuffers(ID3D11DeviceContext*);

	void RenderBuffers(ID3D11DeviceContext*);
	

	float m_particleDeviationX, m_particleDeviationY, m_particleDeviationZ;
	float m_particleVelocity, m_particleVelocotyVariation;
	float m_particleSize, m_particlesPerSecond;
	int m_maxParticles;

	int m_currentParticleCount;
	float m_currentTime;

	Texture* m_texture;
	ParticleType* m_particleList;

	int m_vertexCount, m_indexCount;
	VertexType* m_vertices;
	ID3D11Buffer *m_vertexBuffer, *m_indexBuffer;

	//bool Initialize(ID3D11Device*);
	//void Shutdown();

	//void Update(float, ID3D11Device*);
	//bool Render(ID3D11DeviceContext*, Shader*, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX, 
	//	ID3D11ShaderResourceView*, D3DXVECTOR3, D3DXVECTOR4, D3DXVECTOR4);
private:
	//Model* m_particles;
	//float spawnRate, currentTime;

};