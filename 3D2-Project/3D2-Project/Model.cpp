#include "Model.h"


Model::Model()
{
	m_vertexBuffer = 0;
	m_instanceBuffer = 0;
	m_textures[0] = 0;
	m_textures[1] = 0;
	m_instances = 0;
}


Model::~Model()
{
}

bool Model::Initialize(ID3D11Device* device, char* modelFilename, LPCSTR textureFilename, LPCSTR textureFilename2, int nrOfInstances)
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

	result = LoadTexture(device, textureFilename, textureFilename2);
	if (!result)
	{
		return false;
	}

	return true;
}

void Model::Shutdown()
{
	ReleaseTexture();
	ShutdownBuffers();
}

void Model::Render(ID3D11DeviceContext* deviceContext)
{
	RenderBuffers(deviceContext);
}

int Model::GetVertexCount()
{
	return m_vertexCount;
}

int Model::GetInstanceCount()
{
	return m_instanceCount;
}

ID3D11ShaderResourceView** Model::GetTextureArray()
{
	//ID3D11ShaderResourceView* textures[2];
	//textures[0] = m_textures[0]->GetTexture();
	//textures[1] = m_textures[1]->GetTexture();

	return m_textures;
}

void Model::SetPosition(float x, float y, float z)
{
	m_position = D3DXVECTOR3(x, y, z);
}

D3DXVECTOR3 Model::GetPosition()
{
	return m_position;
}

void Model::SetNewInstanceObject(ID3D11Device* device)
{
	D3D11_BUFFER_DESC instanceBufferDesc;
	D3D11_SUBRESOURCE_DATA instanceData;

	// init instance desc
	m_instanceCount++;

	InstanceType* oldInstances = m_instances;
	m_instances = new InstanceType[m_instanceCount];
	
	for (int i = 0; i < m_instanceCount - 1; i++)
	{
		m_instances[i].position = oldInstances[i].position;
	}
	float r1 = ((double)rand() / (RAND_MAX));
	float r2 = ((double)rand() / (RAND_MAX));
	float r3 = ((double)rand() / (RAND_MAX));

	m_instances[m_instanceCount - 1].position = D3DXVECTOR3(r1 * 20, r2 * 3, r3 * 10);

	instanceBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	instanceBufferDesc.ByteWidth = sizeof(InstanceType)* m_instanceCount;
	instanceBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	instanceBufferDesc.CPUAccessFlags = 0;
	instanceBufferDesc.MiscFlags = 0;
	instanceBufferDesc.StructureByteStride = 0;

	instanceData.pSysMem = m_instances;
	instanceData.SysMemPitch = 0;
	instanceData.SysMemSlicePitch = 0;

	device->CreateBuffer(&instanceBufferDesc, &instanceData, &m_instanceBuffer);
}

bool Model::InitializeBuffers(ID3D11Device* device, int nrOfInstances)
{
	VertexType* vertices;
	D3D11_BUFFER_DESC vertexBufferDesc, instanceBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, instanceData;
	HRESULT result;

	//vertices = new VertexType[m_vertexCount];
	//indices = new unsigned long[m_indexCount];

	//// Load the vertex array and index array with data.
	//for (int i = 0; i<m_vertexCount; i++)
	//{
	//	vertices[i].position = D3DXVECTOR3(m_model[i].x, m_model[i].y, m_model[i].z);
	//	vertices[i].texture = D3DXVECTOR2(m_model[i].tu, m_model[i].tv);
	//	vertices[i].normal = D3DXVECTOR3(m_model[i].nx, m_model[i].ny, m_model[i].nz);

	//	indices[i] = i;
	//}
	m_vertexCount = reader.GetVertices().size();
	vertices = new VertexType[m_vertexCount];

	for (int i = 0; i < m_vertexCount; i++)
	{
		Vertex vertex = reader.GetVertices()[i];
		vertices[i].position = vertex.position;
		vertices[i].texture = vertex.texture;
		vertices[i].normal = vertex.normal;
	}


	// Set up the description of the static vertex buffer.
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexType)* m_vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the vertex data.
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	// Old index buffer
	/*
	// Set up the description of the static index buffer.
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long)* m_indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the index data.
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	result = device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);
	if (FAILED(result))
	{
		return false;
	}
	*/

	// init instance desc
	m_instanceCount = nrOfInstances;

	m_instances = new InstanceType[m_instanceCount];

	for (int i = 0; i < m_instanceCount; i++)
	{
		m_instances[i].position = D3DXVECTOR3(i * 20.0, 0, 0);
	}

	instanceBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	instanceBufferDesc.ByteWidth = sizeof(InstanceType)* m_instanceCount;
	instanceBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	instanceBufferDesc.CPUAccessFlags = 0;
	instanceBufferDesc.MiscFlags = 0;
	instanceBufferDesc.StructureByteStride = 0;

	instanceData.pSysMem = m_instances;
	instanceData.SysMemPitch = 0;
	instanceData.SysMemSlicePitch = 0;

	result = device->CreateBuffer(&instanceBufferDesc, &instanceData, &m_instanceBuffer);
	if (FAILED(result))
	{
		return false;
	}

	// Release the arrays now that the vertex and index buffers have been created and loaded.
	delete[] vertices;
	vertices = 0;

	return true;
}

void Model::ShutdownBuffers()
{
	// Release the instance buffer.
	if (m_instanceBuffer)
	{
		m_instanceBuffer->Release();
		m_instanceBuffer = 0;
	}

	// Release the vertex buffer.
	if (m_vertexBuffer)
	{
		m_vertexBuffer->Release();
		m_vertexBuffer = 0;
	}

	delete m_instances;
	m_instances = 0;
}

void Model::RenderBuffers(ID3D11DeviceContext* deviceContext)
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

	//deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

bool Model::LoadTexture(ID3D11Device* device, LPCSTR filename, LPCSTR filename2)
{
	HRESULT result;

	//m_textures[0] = new Texture();
	//result = m_textures[0]->Initialize(device, filename);
	//if (!result)
	//{
	//	return false;
	//}
	result = D3DX11CreateShaderResourceViewFromFile(device, filename, NULL, NULL, &m_textures[0], NULL);
	if (FAILED(result))
	{
		return false;
	}

	result = D3DX11CreateShaderResourceViewFromFile(device, filename2, NULL, NULL, &m_textures[1], NULL);
	if (FAILED(result))
	{
		return false;
	}

	//m_textures[1] = new Texture();
	//result = m_textures[1]->Initialize(device, filename2);
	//if (!result)
	//{
	//	return false;
	//}

	return true;
}

void Model::ReleaseTexture()
{
	m_textures[0]->Release();
	m_textures[0] = 0;

	m_textures[1]->Release();
	m_textures[1] = 0;
}

bool Model::LoadModel(char* filename)
{
	reader.Load(filename);

    return true;
}