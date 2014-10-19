#include "Graphics.h"


Graphics::Graphics()
{
	m_D3D = 0;
	m_camera = 0;
	m_models = std::vector<Model*>();
	m_renderTexture = 0;
	m_depthShader = 0;
	m_shadowShader = 0;
	m_light = 0;
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
	m_camera->SetPosition(0.0f, 10.0f, -50.0f);
	//m_camera->SetRotation(0.0f, 20.0f, 0.0f);

	// Create the model
	m_models = std::vector<Model*>();

	// Ground
	Model* ground = new Model();
	result = ground->Initialize(m_D3D->GetDevice(), "../3D2-Project/Obj/Ground.obj", "../3D2-Project/Textures/Ground.jpg");
	if (!result)
	{
		MessageBox(hwnd, "Could not initialize the model object.", "Error", MB_OK);
		return false;
	}
	//Van
	Model* van = new Model();
	result = van->Initialize(m_D3D->GetDevice(), "../3D2-Project/Obj/Van.obj", "../3D2-Project/Textures/Van.jpg");
	if (!result)
	{
		MessageBox(hwnd, "Could not initialize the model object.", "Error", MB_OK);
		return false;
	}
	
	m_models.push_back(ground);
	m_models.push_back(van);

	// Create the light object
	m_light = new Light();
	m_light->SetAmbientColor(0.15f, 0.15f, 0.15f, 1.0f);
	m_light->SetDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_light->SetLookAt(0.0f, 0.0, 0.0f);
	m_light->GenerateProjectionMatrix(SCREEN_DEPTH, SCREEN_NEAR);

	// Create the shaders
	m_renderTexture = new RenderTexture();
	result = m_renderTexture->Initialize(m_D3D->GetDevice(), SHADOWMAP_WIDTH, SHADOWMAP_HEIGHT, SCREEN_DEPTH, SCREEN_NEAR);

	m_shader = new Shader();
	result = m_shader->Initialize(m_D3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, "Could not initialize the texture shader object.", "Error", MB_OK);
		return false;
	}



	return true;
}

void Graphics::Shutdown()
{
	// Shutdown light
	delete m_light;
	m_light = 0;

	// Shutdown lightShader
	m_shader->Shutdown();
	delete m_shader;
	m_shader = 0;

	// Shutdown model
	for (int i = 0; i < m_models.size(); i++)
	{
		m_models[i]->Shutdown();
		delete m_models[i];
	}
	m_models.clear();

	// Shutdown camera
	delete m_camera;
	m_camera = 0;

	// Shutdown Direct3D
	m_D3D->Shutdown();
	delete m_D3D;
	m_D3D = 0;
}

bool Graphics::Frame(float posX, float posY, float posZ, float rotX, float rotY, float rotZ)
{
	bool result;
	static float rotation = 0.0f;

	rotation += (float)D3DX_PI * 0.01f;
	if (rotation > 2 * D3DX_PI)
	{
		rotation -= 2 * D3DX_PI;
	}

	//m_light->Rotate(rotation);
	result = Render(rotation);
	if (!result)
	{
		return false;
	}

	return true;
}

bool Graphics::RenderSceneToTexture()
{
	return true;
}

bool Graphics::Render(float rotation)
{
	D3DXMATRIX viewMatrix, worldMatrix, projectionMatrix;
	D3DXMATRIX lightViewMatrix, lightProjectionMatrix;
	bool result;

	result = RenderSceneToTexture();
	if (!result)
	{
		return false;
	}

	m_D3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	m_camera->Render();

	m_light->GenerateViewMatrix();

	m_camera->GetViewMatrix(viewMatrix);
	m_D3D->GetWorldMatrix(worldMatrix);
	m_D3D->GetProjectionMatrix(projectionMatrix);

	m_light->GetViewMatrix(lightViewMatrix);
	m_light->GetProjectionMatrix(lightProjectionMatrix);

	for (int i = 0; i < m_models.size(); i++)
	{
		m_models[i]->Render(m_D3D->GetDeviceContext());
		

		result = m_shader->Render(m_D3D->GetDeviceContext(), m_models[i]->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix,
			lightViewMatrix, lightProjectionMatrix, m_models[i]->GetTexture(), "ERROR FIX", m_light->GetPosition(),
			m_light->GetAmbientColor(), m_light->GetDiffuseColor());
		if (!result)
		{
			return false;
		}
	}
	// Present the rendered scene to the screen
	m_D3D->EndScene();

	return true;
}