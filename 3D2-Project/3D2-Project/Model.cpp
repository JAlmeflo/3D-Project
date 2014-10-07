#include "Model.h"


Model::Model()
{
	m_vertexBuffer = 0;
	m_indexBuffer = 0;
}


Model::~Model()
{
}

bool Model::Initialize(ID3D11Device* device)
{
	bool result;

	result = InitializeBuffers(device);
	if (!result)
	{
		return false;
	}

	return true;
}

void Model::Shutdown()
{
	ShutdownBuffers();
}

void Model::Render(ID3D11DeviceContext* deviceContext)
{
	RenderBuffers(deviceContext);
}

int Model::GetIndexCount()
{
	return m_indexCount;
}

bool Model::InitializeBuffers(ID3D11Device* device)
{
	VertexType* vertices;
	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;

	m_vertexCount = 3;
	m_indexCount = 3;

	vertices = new VertexType[m_vertexCount];
	return true;
}

void Model::ShutdownBuffers()
{
}

void Model::RenderBuffers(ID3D11DeviceContext* deviceContext)
{
}