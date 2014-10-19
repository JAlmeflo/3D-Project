#pragma once

// Globals
const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;

#include <Windows.h>
#include "D3DClass.h"
#include "Camera.h"
#include "Model.h"
#include "LightShader.h"
#include "Light.h"

class Graphics
{
public:
	Graphics();
	~Graphics();

	bool Initialize(int, int, HWND);
	void Shutdown();
	bool Frame();
private :
	bool Render(float);

	D3DClass* m_D3D;
	Camera* m_camera;
	std::vector<Model*> m_models;
	LightShader* m_lightShader;
	Light* m_light;
};

