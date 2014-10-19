#pragma once

// Globals
const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 100.0f;
const float SCREEN_NEAR = 1.0f;

const int SHADOWMAP_WIDTH = 1024;
const int SHADOWMAP_HEIGHT = 1024;

#include <Windows.h>
#include "D3DClass.h"
#include "Camera.h"
#include "Model.h"
#include "Shader.h"
#include "Light.h"
#include "RenderTexture.h"
#include "DepthShader.h"

class Graphics
{
public:
	Graphics();
	~Graphics();

	bool Initialize(int, int, HWND);
	void Shutdown();
	bool Frame(float, float, float, float, float, float);
private :
	bool RenderSceneToTexture();
	bool Render(float);

	D3DClass* m_D3D;
	Camera* m_camera;
	std::vector<Model*> m_models;
	RenderTexture* m_renderTexture;
	DepthShader* m_depthShader;
	Shader* m_shadowShader;
	Light* m_light;
};

