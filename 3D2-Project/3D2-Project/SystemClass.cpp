#include "SystemClass.h"
static SystemClass* ApplicationHandle;


SystemClass::SystemClass()
{
}


SystemClass::~SystemClass()
{
}

bool SystemClass::Initialize()
{
	int screenWidth, screenHeight;
	bool result;

	screenWidth = 0;
	screenHeight = 0;
	InitializeWindows(screenWidth, screenHeight);

	//Input

	//Graphics

	return true;
}

void SystemClass::Shutdown()
{
	//Graphics

	//Input

	ShutdownWindows();
}

void SystemClass::Run()
{
	MSG msg;
	bool done, result;
	
	ZeroMemory(&msg, sizeof(MSG));

	done = false;
	while (!done)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		if (msg.message == WM_QUIT)
		{
			done = true;
		}
		else
		{
			result = Frame();
			if (!result)
			{
				done = true;
			}
		}
	}
}

LRESULT CALLBACK SystemClass::MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
{
	switch (umsg)
	{
		default:
		{
		   return DefWindowProc(hwnd, umsg, wparam, lparam);
		}
	}
}

bool SystemClass::Frame()
{
	bool result;

	//Input

	// Graphics

	return true;
}

void SystemClass::InitializeWindows(int& screenWidth, int& screenHeight)
{
}

void SystemClass::ShutdownWindows()
{
}

LRESULT CALLBACK SystemClass::WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
{
	return NULL;
}