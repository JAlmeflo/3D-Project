#include "Camera.h"


Camera::Camera()
{
	m_positionX = 0.0f;
	m_positionY = 0.0f;
	m_positionZ = 0.0f;

	m_rotationX = 0.0f;
	m_rotationY = 0.0f;
	m_rotationZ = 0.0f;

	Publisher::AddSubscriber(this);
}


Camera::~Camera()
{
	Publisher::Unsubscribe(this);
}

void Camera::SetPosition(float x, float y, float z)
{
	m_positionX = x;
	m_positionY = y;
	m_positionZ = z;
}

void Camera::SetRotation(float x, float y, float z)
{
	m_rotationX = x;
	m_rotationY = y;
	m_rotationZ = z;
}

D3DXVECTOR3 Camera::GetPosition()
{
	return D3DXVECTOR3(m_positionX, m_positionY, m_positionZ);
}

D3DXVECTOR3 Camera::GetRotation()
{
	return D3DXVECTOR3(m_rotationX, m_rotationY, m_rotationZ);
}

void Camera::Render()
{
	D3DXVECTOR3 up, position, lookAt;
	float yaw, pitch, roll;
	D3DXMATRIX rotationMatrix;

	//m_rotationZ += 1.0f;

	up.x = 0.0f;
	up.y = 1.0f;
	up.z = 0.0f;

	position.x = m_positionX;
	position.y = m_positionY;
	position.z = m_positionZ;

	lookAt.x = 0.0f;
	lookAt.y = 0.0f;
	lookAt.z = 1.0f;

	// Set the yaw (Y axis), pitch (X axis), and roll (Z axis) rotations in radians.
	pitch = m_rotationX * 0.0174532925f;
	yaw = m_rotationY * 0.0174532925f;
	roll = m_rotationZ * 0.0174532925f;

	D3DXMatrixRotationYawPitchRoll(&rotationMatrix, yaw, pitch, roll);

	D3DXVec3TransformCoord(&lookAt, &lookAt, &rotationMatrix);
	D3DXVec3TransformCoord(&up, &up, &rotationMatrix);

	lookAt = position + lookAt;

	D3DXMatrixLookAtLH(&m_viewMatrix, &position, &lookAt, &up);
}

void Camera::GetViewMatrix(D3DXMATRIX& viewMatrix)
{
	viewMatrix = m_viewMatrix;
}

void Camera::Update(bool p_keys[256])
{
	// w = 0x57
	// a = 0x41
	// s = 0x53
	// d = 0x44

	// w
	if (p_keys[0x57])
	{
		D3DXVECTOR3 pos = GetPosition();
		SetPosition(pos.x, pos.y, pos.z + 1);
	}
	//a
	if (p_keys[0x41])
	{
		D3DXVECTOR3 pos = GetPosition();
		SetPosition(pos.x - 1, pos.y, pos.z);
	}
	// s
	if (p_keys[0x53])
	{
		D3DXVECTOR3 pos = GetPosition();
		SetPosition(pos.x, pos.y, pos.z - 1);
	}
	// d
	if (p_keys[0x44])
	{
		D3DXVECTOR3 pos = GetPosition();
		SetPosition(pos.x + 1, pos.y, pos.z);
	}
}