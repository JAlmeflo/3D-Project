#include "Input.h"


Input::Input()
{
}


Input::~Input()
{
}

bool Input::Initialize()
{
	for (int i = 0; i < 256; i++)
	{
		m_keys[i] = false;
	}

	m_publisher = Publisher();
	return m_publisher.Initialize();
}

void Input::KeyDown(unsigned int input)
{
	m_keys[input] = true;
}

void Input::KeyUp(unsigned int input)
{
	m_keys[input] = false;
}

bool Input::IsKeyDown(unsigned int key)
{
	return m_keys[key];
}

void Input::Update()
{
	m_publisher.Update(m_keys);
}