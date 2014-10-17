#pragma once

#include "Publisher.h"

class Input
{
public:
	Input();
	~Input();

	bool Initialize();

	void KeyDown(unsigned int);
	void KeyUp(unsigned int);

	bool IsKeyDown(unsigned int);

	void Update();
private:
	bool m_keys[256];
	Publisher m_publisher;
};

