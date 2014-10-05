#pragma once
class Inputclass
{
public:
	Inputclass();
	~Inputclass();

	bool Initialize();

	void KeyDown(unsigned int);
	void KeyUp(unsigned int);

	bool IsKeyDown(unsigned int);
private:
	bool m_keys[256];
};

