#include "Graphicsclass.h"


Graphicsclass::Graphicsclass()
{
	m_D3D = 0;
}


Graphicsclass::~Graphicsclass()
{
}

bool Graphicsclass::Initialize(int screenWidth, int screenHeight, HWND hwnd)
{
	bool result;

	m_D3D = new D3DClass();
	result = m_D3D->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	if (!result)
	{
		MessageBox(hwnd, "Could not initialize Direct3D", "Error", MB_OK);
		return false;
	}


	return true;
}

void Graphicsclass::Shutdown()
{
	m_D3D->Shutdown();
	delete m_D3D;
	m_D3D = 0;
}

bool Graphicsclass::Frame()
{
	bool result;

	result = Render();
	if (!result)
	{
		return false;
	}

	return true;
}

bool Graphicsclass::Render()
{
	m_D3D->BeginScene(0.5f, 0.5f, 0.5f, 0.5f);

	m_D3D->EndScene();

	return true;
}