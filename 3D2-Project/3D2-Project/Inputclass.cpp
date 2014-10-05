#include "Inputclass.h"


Inputclass::Inputclass()
{
}


Inputclass::~Inputclass()
{
}

bool Inputclass::Initialize()
{
	for (int i = 0; i < 256; i++)
	{
		m_keys[i] = false;
	}

	return true;
}

void Inputclass::KeyDown(unsigned int input)
{
	m_keys[input] = true;
}

void Inputclass::KeyUp(unsigned int input)
{
	m_keys[input] = false;
}

bool Inputclass::IsKeyDown(unsigned int key)
{
	return m_keys[key];
}