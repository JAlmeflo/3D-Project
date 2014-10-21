#include "ParticleModel.h"


ParticleModel::ParticleModel()
{
	m_vertexBuffer = 0;
	m_instanceBuffer = 0;
	m_texture = 0;
}


ParticleModel::~ParticleModel()
{
}

bool ParticleModel::Initialize(ID3D11Device* device, char* modelFilename, LPCSTR textureFilename, int nrOfInstances)
{
	bool result;

	reader = OBJReader();

	result = LoadModel(modelFilename);
	if (!result)
	{
		return false;
	}

	result = InitializeBuffers(device, nrOfInstances);
	if (!result)
	{
		return false;
	}

	return true;
}

void ParticleModel::Shutdown()
{
	ReleaseTexture();
	ShutdownBuffers();
}

void ParticleModel::Render(ID3D11DeviceContext* deviceContext)
{
	RenderBuffers(deviceContext);
}

int ParticleModel::GetVertexCount()
{
	return m_vertexCount;
}

int ParticleModel::GetInstanceCount()
{
	return m_instanceCount;
}

ID3D11ShaderResourceView* ParticleModel::GetTexture()
{
	return m_texture->GetTexture();
}

void ParticleModel::SetPosition(float x, float y, float z)
{
	m_position = D3DXVECTOR3(x, y, z);
}

D3DXVECTOR3 ParticleModel::GetPosition()
{
	return m_position;
}

bool ParticleModel::InitializeBuffers(ID3D11Device* device, int nrOfInstances)
{
	VertexType* vertices;
	InstanceType* instances;
	D3D11_BUFFER_DESC vertexBufferDesc, instanceBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, instanceData;
	HRESULT result;

	m_vertexCount = reader.GetVertices().size();
	vertices = new VertexType[m_vertexCount];

	for (int i = 0; i < m_vertexCount; i++)
	{
		Vertex vertex = reader.GetVertices()[i];
		vertices[i].position = vertex.position;
		vertices[i].texture = vertex.texture;
		vertices[i].color = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}

	// vertex buffer desc
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexType)* m_vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	// instance buffer desc
	m_instanceCount = nrOfInstances;

	instances = new InstanceType[m_instanceCount];

	for (int i = 0; i < m_instanceCount; i++)
	{
		float r1 = ((double)rand() / (RAND_MAX));
		float r2 = ((double)rand() / (RAND_MAX));
		float r3 = ((double)rand() / (RAND_MAX));

		// normalzie ?
		//float l = sqrt(r1*r1 + r2*r2 + r3*r3);
		//r1 = r1 / l;
		//r2 = r2 / l;
		//r3 = r3 / l;

		//instances[i].direction = D3DXVECTOR3(r1, r2, r3);		
	}

	instanceBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	instanceBufferDesc.ByteWidth = sizeof(InstanceType)* m_instanceCount;
	instanceBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	instanceBufferDesc.CPUAccessFlags = 0;
	instanceBufferDesc.MiscFlags = 0;
	instanceBufferDesc.StructureByteStride = 0;

	instanceData.pSysMem = instances;
	instanceData.SysMemPitch = 0;
	instanceData.SysMemSlicePitch = 0;

	result = device->CreateBuffer(&instanceBufferDesc, &instanceData, &m_instanceBuffer);
	if (FAILED(result))
	{
		return false;
	}

	// release
	delete[] vertices;
	vertices = 0;

	delete[] instances;
	instances = 0;

	return true;
}

void ParticleModel::ShutdownBuffers()
{
	m_instanceBuffer->Release();
	m_instanceBuffer = 0;

	m_vertexBuffer->Release();
	m_vertexBuffer = 0;
}

void ParticleModel::RenderBuffers(ID3D11DeviceContext* deviceContext)
{
	unsigned int strides[2];
	unsigned int offsets[2];
	ID3D11Buffer* bufferPointers[2];

	strides[0] = sizeof(VertexType);
	strides[1] = sizeof(InstanceType);

	offsets[0] = 0;
	offsets[1] = 0;

	bufferPointers[0] = m_vertexBuffer;
	bufferPointers[1] = m_instanceBuffer;

	deviceContext->IASetVertexBuffers(0, 2, bufferPointers, strides, offsets);
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

bool ParticleModel::LoadTexture(ID3D11Device* device, LPCSTR filename)
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

void ParticleModel::ReleaseTexture()
{
	m_texture->Shutdown();
	delete m_texture;
	m_texture = 0;
}

bool ParticleModel::LoadModel(char* filename)
{
	reader.Load(filename);

	return true;
}