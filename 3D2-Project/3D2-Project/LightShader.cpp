#include "LightShader.h"


LightShader::LightShader()
{
	m_vertexShader = 0;
	m_pixelShader = 0;
	m_layout = 0;
	m_sampleState = 0;
	m_matrixBuffer = 0;
	m_lightBuffer = 0;
}


LightShader::~LightShader()
{
}

bool LightShader::Initialize(ID3D11Device* device, HWND hwnd)
{
	return true;
}

void LightShader::Shutdown()
{
}

bool LightShader::Render(ID3D11DeviceContext* deviceContext, int indexCount, D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix,
	ID3D11ShaderResourceView* texture, D3DXVECTOR3 lightDirection, D3DXVECTOR4 diffuseColor)
{
	return true;
}

bool LightShader::InitializeShader(ID3D11Device* device, HWND hwnd, LPCSTR vsFilename, LPCSTR psFilename)
{
	return true;
}

void LightShader::ShutdownShader()
{
}

void LightShader::OutputShaderErrorMessage(ID3D10Blob* errorMessage, HWND hwnd, LPCSTR shaderFilename)
{
}

bool LightShader::SetShaderParameters(ID3D11DeviceContext* deviceContext, D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix,
	ID3D11ShaderResourceView* texture, D3DXVECTOR3 lightDirection, D3DXVECTOR4 diffuseColor)
{
	return true;
}