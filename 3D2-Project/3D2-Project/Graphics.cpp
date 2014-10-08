#include "Graphics.h"


Graphics::Graphics()
{
	m_D3D = 0;
	m_camera = 0;
	m_model = 0;
	m_textureShader = 0;
}


Graphics::~Graphics()
{
}

bool Graphics::Initialize(int screenWidth, int screenHeight, HWND hwnd)
{
	bool result;

	m_D3D = new D3DClass();
	result = m_D3D->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	if (!result)
	{
		MessageBox(hwnd, "Could not initialize Direct3D", "Error", MB_OK);
		return false;
	}

	// Create the camera
	m_camera = new Camera();
	m_camera->SetPosition(0.0f, 0.0f, -4.0f);
	//m_camera->SetRotation(0.0f, 20.0f, 0.0f);

	// Create the model
	m_model = new Model();
	result = m_model->Initialize(m_D3D->GetDevice(), "../3D2-Project/Textures/dirt.jpg");
	if (!result)
	{
		MessageBox(hwnd, "Could not initialize the model object.", "Error", MB_OK);
		return false;
	}

	// Create the colorShader
	m_textureShader = new TextureShader();
	result = m_textureShader->Initialize(m_D3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, "Could not initialize the texture shader object.", "Error", MB_OK);
		return false;
	}


	return true;
}

void Graphics::Shutdown()
{
	// Shutdown colorShader
	m_textureShader->Shutdown();
	delete m_textureShader;
	m_textureShader = 0;

	// Shutdown model
	m_model->Shutdown();
	delete m_model;
	m_model = 0;

	// Shutdown camera
	delete m_camera;
	m_camera = 0;

	// Shutdown Direct3D
	m_D3D->Shutdown();
	delete m_D3D;
	m_D3D = 0;
}

bool Graphics::Frame()
{
	bool result;

	result = Render();
	if (!result)
	{
		return false;
	}

	return true;
}

bool Graphics::Render()
{
	D3DXMATRIX viewMatrix, worldMatrix, projectionMatrix;
	bool result;

	m_D3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	m_camera->Render();

	m_camera->GetViewMatrix(viewMatrix);
	m_D3D->GetWorldMatrix(worldMatrix);
	m_D3D->GetProjectionMatrix(projectionMatrix);

	m_model->Render(m_D3D->GetDeviceContext());

	result = m_textureShader->Render(m_D3D->GetDeviceContext(), m_model->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix, m_model->GetTexture());
	if (!result)
	{
		return false;
	}

	// Present the rendered scene to the screen
	m_D3D->EndScene();

	return true;
}