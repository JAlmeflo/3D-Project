#pragma once

#include <D3DX10math.h>

class Light
{
public:
	Light();
	~Light();

	void SetDiffuseColor(float, float, float, float);
	void SetDirection(float, float, float);
	void Rotate(float);

	D3DXVECTOR4 GetDiffuseColor();
	D3DXVECTOR3 GetDirection();
private:
	D3DXVECTOR4 m_diffuseColor;
	D3DXVECTOR3 m_direction;
};