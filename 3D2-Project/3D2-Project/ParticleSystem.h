#pragma once

#include "Model.h"
#include "Shader.h"
#include <D3D11.h>

class ParticleSystem
{
public:
	ParticleSystem();
	~ParticleSystem();

	bool Initialize(ID3D11Device*);
	void Shutdown();

	void Update(float, ID3D11Device*);
	bool Render(ID3D11DeviceContext*, Shader*, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX, 
		ID3D11ShaderResourceView*, D3DXVECTOR3, D3DXVECTOR4, D3DXVECTOR4);
private:
	Model* m_particles;
	float spawnRate, currentTime;

};

