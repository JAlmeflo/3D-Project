#pragma once

#include <Windows.h>

class Subscriber
{
public:
	Subscriber();
	~Subscriber();

	virtual void Update(bool p_keys[256]) = 0;
};

