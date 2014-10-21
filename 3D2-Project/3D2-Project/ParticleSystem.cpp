#include "ParticleSystem.h"


ParticleSystem::ParticleSystem()
{
	m_particles = 0;
}


ParticleSystem::~ParticleSystem()
{
}

bool ParticleSystem::Initialize(ID3D11Device* device)
{
	bool result;
	spawnRate = 0.5f;
	currentTime = 0;

	m_particles = new Model();
	result = m_particles->Initialize(device, "../3D2-Project/Obj/Cube.obj", "../3D2-Project/Textures/dirt.jpg", 1);
	if (!result)
	{
		return false;
	}
	m_particles->SetPosition(-50, 5, -30);

	return true;
}

void ParticleSystem::Shutdown()
{
	m_particles->Shutdown();
	delete m_particles;
	m_particles = 0;
}

void ParticleSystem::Update(float dt, ID3D11Device* device)
{
	// move
	D3DXVECTOR3 currPos = m_particles->GetPosition();
	m_particles->SetPosition(currPos.x + 2 * dt, currPos.y, currPos.z);

	// spawn
	currentTime += dt;
	if (currentTime >= spawnRate)
	{
		//m_particles->SetNewInstanceObject(device);
		currentTime -= spawnRate;
	}
}

bool ParticleSystem::Render(ID3D11DeviceContext* deviceContext, Shader* shadowShader, D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix,
	D3DXMATRIX lightViewMatrix, D3DXMATRIX lightProjectionMatrix, ID3D11ShaderResourceView* renderTextureShaderResourceView, 
	D3DXVECTOR3 lightPosition, D3DXVECTOR4 lightAmbientColor, D3DXVECTOR4 lightDiffusetColor)
{
	bool result;
	D3DXVECTOR3 pos = m_particles->GetPosition();
	D3DXMatrixTranslation(&worldMatrix, pos.x, pos.y, pos.z);

	m_particles->Render(deviceContext);

	result = shadowShader->Render(deviceContext, m_particles->GetVertexCount(), m_particles->GetInstanceCount(), worldMatrix, viewMatrix, projectionMatrix,
		lightViewMatrix, lightProjectionMatrix, m_particles->GetTexture(), renderTextureShaderResourceView, lightPosition,
		lightAmbientColor, lightDiffusetColor);
	if (!result)
	{
		return false;
	}

	return true;
}