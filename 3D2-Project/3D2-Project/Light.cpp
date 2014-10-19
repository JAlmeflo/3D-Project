#include "Light.h"


Light::Light()
{
}


Light::~Light()
{
}

void Light::SetAmbientColor(float red, float green, float blue, float alpha)
{
	m_ambientColor = D3DXVECTOR4(red, green, blue, alpha);
}

void Light::SetDiffuseColor(float red, float green, float blue, float alpha)
{
	m_diffuseColor = D3DXVECTOR4(red, green, blue, alpha);
}

void Light::SetDirection(float x, float y, float z)
{
	m_direction = D3DXVECTOR3(x, y, z);
}

void Light::Rotate(float rotation)
{
	float x = cos(rotation);
	float y = sin(rotation);
	SetDirection(x, GetDirection().y, y);
}

D3DXVECTOR4 Light::GetAmbientColor()
{
	return m_ambientColor;
}

D3DXVECTOR4 Light::GetDiffuseColor()
{
	return m_diffuseColor;
}

D3DXVECTOR3 Light::GetDirection()
{
	return m_direction;
}