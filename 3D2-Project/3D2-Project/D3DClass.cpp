#include "D3DClass.h"


D3DClass::D3DClass()
{
}


D3DClass::~D3DClass()
{
}

bool D3DClass::Initialize(int screenWidth, int screenHeight, bool vsync, HWND hwnd, bool fullscreen, float screenDepth, float screenNear)
{
	return true;
}

void D3DClass::Shutdown()
{
}

void D3DClass::BeginScene(float red, float green, float blue, float alpha)
{
}

void D3DClass::EndScene()
{
}

ID3D11Device* D3DClass::GetDevice()
{
	return m_device;
}

ID3D11DeviceContext* D3DClass::GetDeviceContext()
{
	return m_deviceContext;
}

void D3DClass::GetProjectionMatrix(D3DXMATRIX&)
{
}

void D3DClass::GetWorldMatrix(D3DXMATRIX&)
{
}

void D3DClass::GetOrthoMatrix(D3DXMATRIX&)
{
}

void GetVideoCardInfo(char*, int&)
{
}