#pragma once
#define WIN32_LEAN_AND_MEAN

#include <Windows.h>
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
	void InitializeWindows(int&, int&);
	void ShutdownWindows();

	LPCSTR m_applicationName;
	HINSTANCE m_hinstance;
	HWND m_hwnd;

	Input* m_input;
	Graphics* m_graphics;

	static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
	static System* ApplicationHandle;
};

