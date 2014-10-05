#pragma once
#define WIN32_LEAN_AND_MEAN

#include <Windows.h>

class SystemClass
{
public:
	SystemClass();
	~SystemClass();

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


	static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
	static SystemClass* ApplicationHandle;
};

