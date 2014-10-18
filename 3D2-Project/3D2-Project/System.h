#pragma once
#define WIN32_LEAN_AND_MEAN

#include <Windows.h>
#include <windowsx.h>
#include "Input.h"
#include "Graphics.h"

class System
{
public:
	System();
	~System();

	bool Initialize();
	void Run();
	void Shutdown();

	LRESULT CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM);
private:
	bool Frame();
	bool Render(float deltaTime);
	bool Update(float deltaTime);
	void InitializeWindows(int&, int&);
	void ShutdownWindows();

	LPCSTR m_applicationName;
	HINSTANCE m_hinstance;
	HWND m_hwnd;
	int screenWidth, screenHeight;
	bool firstUpdate;

	Input* m_input;
	Graphics* m_graphics;

	static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
	static System* ApplicationHandle;
};

