#pragma once

// Globals
const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;

#include <Windows.h>
#include "D3DClass.h"

class Graphicsclass
{
public:
	Graphicsclass();
	~Graphicsclass();

	bool Initialize(int, int, HWND);
	void Shutdown();
	bool Frame();
private :
	bool Render();
};

