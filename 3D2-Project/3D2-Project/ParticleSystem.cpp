#include "ParticleSystem.h"


ParticleSystem::ParticleSystem()
{
	m_texture = 0;
	m_particleList = 0;
	m_vertices = 0;
	m_vertexBuffer = 0;
	m_indexBuffer = 0;
}


ParticleSystem::~ParticleSystem()
{
}

bool ParticleSystem::Initialize(ID3D11Device* device, LPCSTR textureFilename)
{
	bool result;

	result = LoadTexture(device, textureFilename);
	if (!result)
	{
		return false;
	}

	result = InitializeParticleSystem();
	if (!result)
	{
		return false;
	}

	result = InitializeBuffers(device);
	if (!result)
	{
		return false;
	}

	return true;
}

void ParticleSystem::Shutdown()
{
	ShutdownBuffers();
	ShutdownParticleSystem();
	ReleaseTexture();
}

bool ParticleSystem::Render(ID3D11DeviceContext* deviceContext)
{
	bool result;

	result = UpdateBuffers(deviceContext);
	if (!result)
	{
		return false;
	}

	RenderBuffers(deviceContext);

	return true;
}

void ParticleSystem::Update(float deltaTime)
{
	KillParticles();

	EmitParticles(deltaTime);

	UpdateParticles(deltaTime);
}

ID3D11ShaderResourceView* ParticleSystem::GetTexture()
{
	return m_texture->GetTexture();
}

int ParticleSystem::GetIndexCount()
{
	return m_indexCount;
}

bool ParticleSystem::LoadTexture(ID3D11Device* device, LPCSTR filename)
{
	bool result;

	m_texture = new Texture();
	result = m_texture->Initialize(device, filename);
	if (!result)
	{
		return false;
	}

	return true;
}

void ParticleSystem::ReleaseTexture()
{
	m_texture->Shutdown();
	delete m_texture;
	m_texture = 0;
}

bool ParticleSystem::InitializeParticleSystem()
{
	m_particleDeviationX = 0.5f;
	m_particleDeviationY = 0.1f;
	m_particleDeviationZ = 2.0f;

	m_particleVelocity = 1.0f;
	m_particleVelocotyVariation = 2.0f;

	m_particleSize = 0.2f;

	m_particlesPerSecond = 250.0f;

	m_maxParticles = 5000;

	m_particleList = new ParticleType[m_maxParticles];

	for (int i = 0; i < m_maxParticles; i++)
	{
		m_particleList[i].active = false;
	}

	m_currentParticleCount = 0;
	m_currentTime = 0;

	return true;
}

void ParticleSystem::ShutdownParticleSystem()
{
	delete[] m_particleList;
	m_particleList = 0;
}

bool ParticleSystem::InitializeBuffers(ID3D11Device* device)
{
	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;

	m_vertexCount = m_maxParticles * 6;

	m_indexCount = m_vertexCount;

	m_vertices = new VertexType[m_vertexCount];

	indices = new unsigned long[m_indexCount];

	memset(m_vertices, 0, (sizeof(VertexType)*m_vertexCount));

	for (int i = 0; i < m_indexCount; i++)
	{
		indices[i] = i;
	}

	// vertex buffer desc
	vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	vertexBufferDesc.ByteWidth = sizeof(VertexType)* m_vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	vertexData.pSysMem = m_vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	// index buffer desc
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long)* m_indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	result = device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	// realese pointer
	delete[] indices;
	indices = 0;

	return true;
}

void ParticleSystem::ShutdownBuffers()
{
	m_indexBuffer->Release();
	m_indexBuffer = 0;

	m_vertexBuffer->Release();
	m_vertexBuffer = 0;
}

void ParticleSystem::EmitParticles(float deltaTime)
{
	bool  emitParticle, found;
	float positionX, positionY, positionZ, velocity, red, green, blue;

	m_currentTime += deltaTime;

	emitParticle = false;

	if (m_currentTime > (1000.0f / m_particlesPerSecond))
	{
		m_currentTime = 0.0f;
		emitParticle = true;
	}

	if (emitParticle && m_currentParticleCount < m_maxParticles - 1)
	{
		m_currentParticleCount++;

		positionX = ((float)rand() / RAND_MAX) * m_particleDeviationX;
		positionY = ((float)rand() / RAND_MAX) * m_particleDeviationY;
		positionZ = ((float)rand() / RAND_MAX) * m_particleDeviationZ;

		velocity = m_particleVelocity + ((float)rand() / RAND_MAX) * m_particleVelocotyVariation;

		red = ((float)rand() / RAND_MAX) * 0.5f;
		green = ((float)rand() / RAND_MAX) * 0.5f;
		blue = ((float)rand() / RAND_MAX) * 0.5f;

		int index = 0;
		found = false;
		while (!found)
		{
			if (!m_particleList[index].active || m_particleList[index].positionZ < positionZ)
			{
				found = true;
			}
			else
			{
				index++;
				if (index >= m_maxParticles)
				{
					return;
				}
			}
		}

		int i = m_currentParticleCount;
		int j = i - 1;
		while (i != index)
		{
			m_particleList[i].positionX = m_particleList[j].positionX;
			m_particleList[i].positionY = m_particleList[j].positionY;
			m_particleList[i].positionZ = m_particleList[j].positionZ;
			m_particleList[i].red = m_particleList[j].red;
			m_particleList[i].green = m_particleList[j].green;
			m_particleList[i].blue = m_particleList[j].blue;
			m_particleList[i].velocity = m_particleList[j].velocity;
			m_particleList[i].active = m_particleList[j].active;
			i--;
			j--;
		}

		m_particleList[index].positionX = positionX;
		m_particleList[index].positionY = positionY;
		m_particleList[index].positionZ = positionZ;
		m_particleList[index].red = red;
		m_particleList[index].green = green;
		m_particleList[index].blue = blue;
		m_particleList[index].velocity = velocity;
		m_particleList[index].active = true;
	}
}

void ParticleSystem::UpdateParticles(float deltaTime)
{
	if (m_currentParticleCount >= 1)
	{
		int a = 0;
	}
	for (int i = 0; i < m_currentParticleCount; i++)
	{
		m_particleList[i].positionY = m_particleList[i].positionY - m_particleList[i].velocity * deltaTime * 0.0001f;
	}
}

void ParticleSystem::KillParticles()
{
	for (int i = 0; i < m_maxParticles; i++)
	{
		if (m_particleList[i].active && m_particleList[i].positionY < -3.0f)
		{
			m_particleList[i].active = false;
			m_currentParticleCount--;

			for (int j = i; j < m_maxParticles - 1; j++)
			{
				m_particleList[j].positionX = m_particleList[j + 1].positionX;
				m_particleList[j].positionY = m_particleList[j + 1].positionY;
				m_particleList[j].positionZ = m_particleList[j + 1].positionZ;
				m_particleList[j].red = m_particleList[j + 1].red;
				m_particleList[j].green = m_particleList[j + 1].green;
				m_particleList[j].blue = m_particleList[j + 1].blue;
				m_particleList[j].velocity = m_particleList[j + 1].velocity;
				m_particleList[j].active = m_particleList[j + 1].active;
			}
			i--;
		}
	}
}

bool ParticleSystem::UpdateBuffers(ID3D11DeviceContext* deviceContext)
{
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	VertexType* verticesPtr;
	int index;

	memset(m_vertices, 0, (sizeof(VertexType)* m_vertexCount));
	index = 0;

	for (int i = 0; i < m_currentParticleCount; i++)
	{
		// Bottom left.
		m_vertices[index].position = D3DXVECTOR3(m_particleList[i].positionX - m_particleSize, m_particleList[i].positionY - m_particleSize, m_particleList[i].positionZ);
		m_vertices[index].texture = D3DXVECTOR2(0.0f, 1.0f);
		m_vertices[index].color = D3DXVECTOR4(m_particleList[i].red, m_particleList[i].green, m_particleList[i].blue, 1.0f);
		index++;

		// Top left.
		m_vertices[index].position = D3DXVECTOR3(m_particleList[i].positionX - m_particleSize, m_particleList[i].positionY + m_particleSize, m_particleList[i].positionZ);
		m_vertices[index].texture = D3DXVECTOR2(0.0f, 0.0f);
		m_vertices[index].color = D3DXVECTOR4(m_particleList[i].red, m_particleList[i].green, m_particleList[i].blue, 1.0f);
		index++;

		// Bottom right.
		m_vertices[index].position = D3DXVECTOR3(m_particleList[i].positionX + m_particleSize, m_particleList[i].positionY - m_particleSize, m_particleList[i].positionZ);
		m_vertices[index].texture = D3DXVECTOR2(1.0f, 1.0f);
		m_vertices[index].color = D3DXVECTOR4(m_particleList[i].red, m_particleList[i].green, m_particleList[i].blue, 1.0f);
		index++;

		// Bottom right.
		m_vertices[index].position = D3DXVECTOR3(m_particleList[i].positionX + m_particleSize, m_particleList[i].positionY - m_particleSize, m_particleList[i].positionZ);
		m_vertices[index].texture = D3DXVECTOR2(1.0f, 1.0f);
		m_vertices[index].color = D3DXVECTOR4(m_particleList[i].red, m_particleList[i].green, m_particleList[i].blue, 1.0f);
		index++;

		// Top left.
		m_vertices[index].position = D3DXVECTOR3(m_particleList[i].positionX - m_particleSize, m_particleList[i].positionY + m_particleSize, m_particleList[i].positionZ);
		m_vertices[index].texture = D3DXVECTOR2(0.0f, 0.0f);
		m_vertices[index].color = D3DXVECTOR4(m_particleList[i].red, m_particleList[i].green, m_particleList[i].blue, 1.0f);
		index++;

		// Top right.
		m_vertices[index].position = D3DXVECTOR3(m_particleList[i].positionX + m_particleSize, m_particleList[i].positionY + m_particleSize, m_particleList[i].positionZ);
		m_vertices[index].texture = D3DXVECTOR2(1.0f, 0.0f);
		m_vertices[index].color = D3DXVECTOR4(m_particleList[i].red, m_particleList[i].green, m_particleList[i].blue, 1.0f);
		index++;
	}

	result = deviceContext->Map(m_vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		return false;
	}

	verticesPtr = (VertexType*)mappedResource.pData;

	memcpy(verticesPtr, (void*)m_vertices, (sizeof(VertexType)* m_vertexCount));

	deviceContext->Unmap(m_vertexBuffer, 0);

	return true;
}

void ParticleSystem::RenderBuffers(ID3D11DeviceContext* deviceContext)
{
	unsigned int stride;
	unsigned int offset;

	stride = sizeof(VertexType);
	offset = 0;

	deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

//bool ParticleSystem::Initialize(ID3D11Device* device)
//{
//	bool result;
//	spawnRate = 0.5f;
//	currentTime = 0;
//
//	m_particles = new Model();
//	result = m_particles->Initialize(device, "../3D2-Project/Obj/Cube.obj", "../3D2-Project/Textures/dirt.jpg", 1);
//	if (!result)
//	{
//		return false;
//	}
//	m_particles->SetPosition(-50, 5, -30);
//
//	return true;
//}
//
//void ParticleSystem::Shutdown()
//{
//	m_particles->Shutdown();
//	delete m_particles;
//	m_particles = 0;
//}
//
//void ParticleSystem::Update(float dt, ID3D11Device* device)
//{
//	// move
//	D3DXVECTOR3 currPos = m_particles->GetPosition();
//	m_particles->SetPosition(currPos.x + 2 * dt, currPos.y, currPos.z);
//
//	// spawn
//	currentTime += dt;
//	if (currentTime >= spawnRate)
//	{
//		//m_particles->SetNewInstanceObject(device);
//		currentTime -= spawnRate;
//	}
//}
//
//bool ParticleSystem::Render(ID3D11DeviceContext* deviceContext, Shader* shadowShader, D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix,
//	D3DXMATRIX lightViewMatrix, D3DXMATRIX lightProjectionMatrix, ID3D11ShaderResourceView* renderTextureShaderResourceView, 
//	D3DXVECTOR3 lightPosition, D3DXVECTOR4 lightAmbientColor, D3DXVECTOR4 lightDiffusetColor)
//{
//	bool result;
//	D3DXVECTOR3 pos = m_particles->GetPosition();
//	D3DXMatrixTranslation(&worldMatrix, pos.x, pos.y, pos.z);
//
//	m_particles->Render(deviceContext);
//
//	result = shadowShader->Render(deviceContext, m_particles->GetVertexCount(), m_particles->GetInstanceCount(), worldMatrix, viewMatrix, projectionMatrix,
//		lightViewMatrix, lightProjectionMatrix, m_particles->GetTexture(), renderTextureShaderResourceView, lightPosition,
//		lightAmbientColor, lightDiffusetColor);
//	if (!result)
//	{
//		return false;
//	}
//
//	return true;
//}