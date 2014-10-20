#include "Graphics.h"


Graphics::Graphics()
{
	m_D3D = 0;
	m_camera = 0;
	m_models = std::vector<Model*>();
	m_particleSystem = 0;
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
	rotation = 0.0f;

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
	result = ground->Initialize(m_D3D->GetDevice(), "../3D2-Project/Obj/Ground.obj", "../3D2-Project/Textures/Ground.jpg", 1);
	if (!result)
	{
		MessageBox(hwnd, "Could not initialize the model object.", "Error", MB_OK);
		return false;
	}
	ground->SetPosition(0, 0, 0);

	//Van
	Model* van = new Model();
	result = van->Initialize(m_D3D->GetDevice(), "../3D2-Project/Obj/Van.obj", "../3D2-Project/Textures/Van.jpg", 3);
	if (!result)
	{
		MessageBox(hwnd, "Could not initialize the model object.", "Error", MB_OK);
		return false;
	}
	van->SetPosition(-20, 0, 0);

	//Van
	Model* van2 = new Model();
	result = van2->Initialize(m_D3D->GetDevice(), "../3D2-Project/Obj/Van.obj", "../3D2-Project/Textures/Van.jpg", 5);
	if (!result)
	{
		MessageBox(hwnd, "Could not initialize the model object.", "Error", MB_OK);
		return false;
	}
	van2->SetPosition(-10, 0, 10);
	
	m_models.push_back(ground);
	m_models.push_back(van);
	//m_models.push_back(van2);

	// Create the particle system
	m_particleSystem = new ParticleSystem();
	result = m_particleSystem->Initialize(m_D3D->GetDevice());
	if (!result)
	{
		MessageBox(hwnd, "Could not initialize Particle system", "Error", MB_OK);
		return false;
	}

	// Create the light object
	m_light = new Light();
	m_light->SetAmbientColor(0.15f, 0.15f, 0.15f, 1.0f);
	m_light->SetDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_light->SetLookAt(0.0f, 0.0, 0.0f);
	m_light->SetPosition(0.0f, 30.0f, -10.0f);
	m_light->GenerateProjectionMatrix(SCREEN_DEPTH, SCREEN_NEAR);

	// Create the shaders
	m_renderTexture = new RenderTexture();
	result = m_renderTexture->Initialize(m_D3D->GetDevice(), SHADOWMAP_WIDTH, SHADOWMAP_HEIGHT, SCREEN_DEPTH, SCREEN_NEAR);
	if (!result)
	{
		MessageBox(hwnd, "Could not initialize the render texture object.", "Error", MB_OK);
		return false;
	}

	m_depthShader = new DepthShader();
	result = m_depthShader->Initialize(m_D3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, "Could not initialize the depth shader object.", "Error", MB_OK);
		return false;
	}

	m_shadowShader = new Shader();
	result = m_shadowShader->Initialize(m_D3D->GetDevice(), hwnd);
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
	m_light->~Light();
	delete m_light;
	m_light = 0;

	// Shutdown lightShader
	m_shadowShader->Shutdown();
	delete m_shadowShader;
	m_shadowShader = 0;

	m_depthShader->Shutdown();
	delete m_depthShader;
	m_depthShader = 0;

	m_renderTexture->Shutdown();
	delete m_renderTexture;
	m_renderTexture = 0;

	// Shutdown model
	for (int i = 0; i < m_models.size(); i++)
	{
		m_models[i]->Shutdown();
		delete m_models[i];
	}
	m_models.clear();

	//Shutdown particle system
	m_particleSystem->Shutdown();
	delete m_particleSystem;
	m_particleSystem = 0;

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
	result = Render(rotation);
	if (!result)
	{
		return false;
	}

	return true;
}

void Graphics::Update(float dt)
{
	rotation += (float)D3DX_PI * 0.8f * dt;
	if (rotation > 2 * D3DX_PI)
	{
		rotation -= 2 * D3DX_PI;
	}

	m_light->Rotate(rotation);

	m_particleSystem->Update(dt, m_D3D->GetDevice());
}

bool Graphics::RenderSceneToTexture()
{
	D3DXMATRIX worldMatrix, lightViewMatrix, lightProjectionMatrix, translateMatrix;
	D3DXVECTOR3 pos;
	bool result;

	// render texture
	m_renderTexture->SetRenderTarget(m_D3D->GetDeviceContext());
	m_renderTexture->ClearRenderTarget(m_D3D->GetDeviceContext(), 0.0f, 0.0f, 0.0f, 1.0f);

	// get matrices
	m_light->GenerateViewMatrix();

	m_light->GetViewMatrix(lightViewMatrix);
	m_light->GetProjectionMatrix(lightProjectionMatrix);

	// for loop here
	for (int i = 0; i < m_models.size(); i++)
	{
		m_D3D->GetWorldMatrix(worldMatrix);

		pos = m_models[i]->GetPosition();
		D3DXMatrixTranslation(&worldMatrix, pos.x, pos.y, pos.z);
		m_models[i]->Render(m_D3D->GetDeviceContext());
		
		result = m_depthShader->Render(m_D3D->GetDeviceContext(), m_models[i]->GetVertexCount(), m_models[i]->GetInstanceCount(), worldMatrix, lightViewMatrix, lightProjectionMatrix);
		if (!result)
		{
			return false;
		}
	}

	m_D3D->SetBackBufferRenderTarget();
	m_D3D->ResetViewport();

	return true;
}

bool Graphics::Render(float rotation)
{
	D3DXMATRIX viewMatrix, worldMatrix, projectionMatrix;
	D3DXMATRIX lightViewMatrix, lightProjectionMatrix;
	D3DXVECTOR3 pos;
	bool result;

	// rendedr scene to texture
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

	// render models
	for (int i = 0; i < m_models.size(); i++)
	{
		pos = m_models[i]->GetPosition();
		D3DXMatrixTranslation(&worldMatrix, pos.x, pos.y, pos.z);

		m_models[i]->Render(m_D3D->GetDeviceContext());
		
		result = m_shadowShader->Render(m_D3D->GetDeviceContext(), m_models[i]->GetVertexCount(), m_models[i]->GetInstanceCount(), worldMatrix, viewMatrix, projectionMatrix,
			lightViewMatrix, lightProjectionMatrix, m_models[i]->GetTexture(), m_renderTexture->GetShaderResourceView(), m_light->GetPosition(),
			m_light->GetAmbientColor(), m_light->GetDiffuseColor());
		if (!result)
		{
			return false;
		}
		m_D3D->GetWorldMatrix(worldMatrix);
	}

	// render particle system 
	result = m_particleSystem->Render(m_D3D->GetDeviceContext(), m_shadowShader, worldMatrix, viewMatrix, projectionMatrix, lightViewMatrix, lightProjectionMatrix,
		m_renderTexture->GetShaderResourceView(), m_light->GetPosition(), m_light->GetAmbientColor(), m_light->GetDiffuseColor());
	if (!result)
	{
		return false;
	}
	m_D3D->GetWorldMatrix(worldMatrix);

	// Present the rendered scene to the screen
	m_D3D->EndScene();

	return true;
}