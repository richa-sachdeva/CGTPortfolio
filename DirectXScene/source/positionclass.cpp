// Based on tutorials from http://www.rastertek.com
// For core module AG1101A - Programming for Games, DirectX Coursework, University of Abertay, Dundee.
// By Richa Sachdeva, 1304869, M.Sc. Computer Games Technology

#include "positionclass.h"
#include <d3dx10math.h>


PositionClass::PositionClass(){
	m_frameTime = 0.0f;
	m_rotationX = 0.0f;
	m_rotationY = 0.0f;
	m_rotationZ = 0.0f;
	m_positionX = 0.0f;
	m_positionY = 0.0f;
	m_positionZ = 0.0f;
	m_leftTurnSpeed = 0.0f;
	m_rightTurnSpeed = 0.0f;


	m_leftRollSpeed = 0.0f;
	m_rightRollSpeed = 0.0f;
	m_upTurnSpeed = 0.0f;
	m_downTurnSpeed = 0.0f;
	m_forwardMoveSpeed = 0.0f;
	m_backwardMoveSpeed = 0.0f;
	m_leftMoveSpeed = 0.0f;
	m_rightMoveSpeed = 0.0f;
	m_upMoveSpeed = 0.0f;
	m_downMoveSpeed = 0.0f;

}


PositionClass::PositionClass(const PositionClass& other)
{
}

PositionClass::~PositionClass()
{
}

void PositionClass::SetFrameTime(float time)
{
	m_frameTime = time;
	return;
}

void PositionClass::GetRotation(float& x, float& y, float& z)
{
	x = m_rotationX;
	y = m_rotationY;
	z = m_rotationZ;
	return;
}

void PositionClass::GetPosition(float& x, float& y, float& z)
{
	x = m_positionX;
	y = m_positionY;
	z = m_positionZ;
	return;
}

void PositionClass::TurnLeft(bool keydown)
{
	// If the key is pressed increase the speed at which the camera turns left.  
	// If not slow down the turn speed.
	if(keydown)
	{
		m_leftTurnSpeed += 5.0f / m_frameTime;

		if(m_leftTurnSpeed > (15.0f / m_frameTime))
		{
			m_leftTurnSpeed = 15.0f / m_frameTime;
		}
	}
	else
	{
		m_leftTurnSpeed -= 5.0f / m_frameTime;


		if(m_leftTurnSpeed < 0.0f)
		{

			m_leftTurnSpeed = 0.0f;
		}
	}

	// Update the rotation using the turning speed.
	m_rotationY -= m_leftTurnSpeed;
	if(m_rotationY < 0.0f)
	{
		m_rotationY += 360.0f;
	}

	return;
}

void PositionClass::TurnRight(bool keydown)
{
	// If the key is pressed increase the speed at which the camera turns right.  
	//If not slow down the turn speed.
	if(keydown)
	{
		m_rightTurnSpeed += 5.0f / m_frameTime;

		if(m_rightTurnSpeed > (15.0f / m_frameTime))
		{
			m_rightTurnSpeed = 15.0f / m_frameTime;
		}
	}
	else
	{
		m_rightTurnSpeed -= 5.0f / m_frameTime;


		if(m_rightTurnSpeed < 0.0f)
		{

			m_rightTurnSpeed = 0.0f;
		}
	}

	// Update the rotation using the turning speed.
	m_rotationY += m_rightTurnSpeed;
	if(m_rotationY > 360.0f)
	{
		m_rotationY -= 360.0f;
	}

	return;
}

void PositionClass::TurnUp(bool keydown)
{
	// If the key is pressed increase the speed at which the camera turns.  
	// If not slow down the turn speed.
	if(keydown)
	{
		m_upTurnSpeed += 5.0f / m_frameTime;	//m_frameTime* 0.010f;

		if(m_upTurnSpeed > (15.0f / m_frameTime))
		{
			m_upTurnSpeed = 15.0f / m_frameTime;//m_frameTime* 0.050f;;
		}
	}
	else
	{
		m_upTurnSpeed -= 5.0f / m_frameTime;


		if(m_upTurnSpeed < 0.0f)
		{

			m_upTurnSpeed = 0.0f;
		}
	}
	// Update the rotation using the turning speed.

	m_rotationX -= m_upTurnSpeed;
	if(m_rotationX < 0.0f)
	{
		m_rotationX += 360.0f;
	}

	return;
}

void PositionClass::TurnDown(bool keydown)
{
	// If the key is pressed increase the speed at which the camera turns.  
	// If not slow down the turn speed.
	if(keydown)
	{
		m_downTurnSpeed += 5.0f / m_frameTime;

		if(m_downTurnSpeed > (15.0f / m_frameTime))
		{
			m_downTurnSpeed = 15.0f / m_frameTime;
		}
	}
	else
	{
		m_downTurnSpeed -= 5.0f / m_frameTime;

		if(m_downTurnSpeed < 0.0f)
		{

			m_downTurnSpeed = 0.0f;
		}
	}
	// Update the rotation using the turning speed.
	m_rotationX += m_downTurnSpeed;
	if(m_rotationX > 360.0f)
	{
		m_rotationX -= 360.0f;
	}

	return;
}

void PositionClass::RollLeft(bool keydown)
{
	// If the key is pressed increase the speed at which the camera turns left.  
	// If not slow down the turn speed.
	if(keydown)
	{
		m_leftRollSpeed += 5.0f / m_frameTime;

		if(m_leftRollSpeed > (15.0f / m_frameTime))
		{
			m_leftRollSpeed = 15.0f / m_frameTime;
		}
	}
	else
	{
		m_leftRollSpeed -= 5.0f / m_frameTime;

		if(m_leftRollSpeed < 0.0f)
		{

			m_leftRollSpeed = 0.0f;
		}
	}

	// Update the rotation using the turning speed.
	m_rotationZ += m_leftRollSpeed;
	if(m_rotationZ > 360.0f)
	{
		m_rotationZ -= 360.0f;
	}
	return;
}

void PositionClass::RollRight(bool keydown)
{
	// If the key is pressed increase the speed at which the camera turns right.  
	//If not slow down the turn speed.
	if(keydown)
	{
		m_rightRollSpeed += 5.0f / m_frameTime;

		if(m_rightRollSpeed > (15.0f / m_frameTime))
		{
			m_rightRollSpeed = 15.0f / m_frameTime;
		}
	}
	else
	{
		m_rightRollSpeed -= 5.0f / m_frameTime;


		if(m_rightRollSpeed < 0.0f)
		{

			m_rightRollSpeed = 0.0f;
		}
	}

	// Update the rotation using the turning speed.
	m_rotationZ -= m_rightRollSpeed;
	if(m_rotationZ < 0.0f)
	{

		m_rotationZ += 360.0f;
	}

	return;
}

void PositionClass::MoveForward(bool keydown)
{
	// If the key is pressed increase the speed at which the camera moves.  
	// If not slow down the move speed.
	if(keydown)
	{
		m_forwardMoveSpeed += 5.0f / m_frameTime;

		if(m_forwardMoveSpeed > (15.0f / m_frameTime))
		{
			m_forwardMoveSpeed = 15.0f / m_frameTime;
		}
	}
	else
	{
		m_forwardMoveSpeed -= 5.0f / m_frameTime;


		if(m_forwardMoveSpeed < 0.0f)
		{

			m_forwardMoveSpeed = 0.0f;
		}
	}

	// Modify speed based on rotation.
	D3DXMATRIX worldMatrix, moveMatrix, rotationMatrix;
	D3DXMatrixTranslation(&moveMatrix, 0, 0, m_forwardMoveSpeed);
	D3DXMatrixRotationYawPitchRoll(&rotationMatrix, m_rotationX * -0.0174532925f, 
		m_rotationY * -0.0174532925f, 
		m_rotationZ * 0.0174532925f);
	worldMatrix = moveMatrix * rotationMatrix;

	// Update the position.
	m_positionY += worldMatrix._41;
	m_positionX += worldMatrix._42;
	m_positionZ += worldMatrix._43;

	return;
}

void PositionClass::MoveBackward(bool keydown)
{
	// If the key is pressed increase the speed at which the camera moves.  
	// If not slow down the move speed.
	if(keydown)
	{
		m_backwardMoveSpeed += 5.0f / m_frameTime;

		if(m_backwardMoveSpeed > (15.0f / m_frameTime))
		{
			m_backwardMoveSpeed = 15.0f / m_frameTime;
		}
	}
	else
	{
		m_backwardMoveSpeed -= 5.0f / m_frameTime;

		if(m_backwardMoveSpeed < 0.0f)
		{
			m_backwardMoveSpeed = 0.0f;
		}
	}

	// Modify speed based on rotation.
	D3DXMATRIX worldMatrix, moveMatrix, rotationMatrix;
	D3DXMatrixTranslation(&moveMatrix, 0, 0, -1*m_backwardMoveSpeed);
	D3DXMatrixRotationYawPitchRoll(&rotationMatrix, m_rotationX * -0.0174532925f, 
		m_rotationY * -0.0174532925f, 
		m_rotationZ * 0.0174532925f);
	worldMatrix = moveMatrix * rotationMatrix;

	// Update the position.
	m_positionY += worldMatrix._41;
	m_positionX += worldMatrix._42;
	m_positionZ += worldMatrix._43;

	return;
}

void PositionClass::MoveLeft(bool keydown)
{
	// If the key is pressed increase the speed at which the camera moves.  
	// If not slow down the move speed.
	if(keydown)
	{
		m_leftMoveSpeed += 5.0f / m_frameTime;

		if(m_leftMoveSpeed > (15.0f / m_frameTime))
		{
			m_leftMoveSpeed = 15.0f / m_frameTime;
		}
	}
	else
	{
		m_leftMoveSpeed -= 5.0f / m_frameTime;

		if(m_leftMoveSpeed < 0.0f)
		{

			m_leftMoveSpeed = 0.0f;
		}
	}

	// Modify speed based on rotation.
	D3DXMATRIX worldMatrix, moveMatrix, rotationMatrix;
	D3DXMatrixTranslation(&moveMatrix, 0, -1*m_leftMoveSpeed, 0);
	D3DXMatrixRotationYawPitchRoll(&rotationMatrix, m_rotationX * -0.0174532925f, 
		m_rotationY * -0.0174532925f, 
		m_rotationZ * -0.0174532925f);
	worldMatrix = moveMatrix * rotationMatrix;

	// Update the position.
	m_positionY += worldMatrix._41;
	m_positionX += worldMatrix._42;
	m_positionZ += worldMatrix._43;

	return;
}

void PositionClass::MoveRight(bool keydown)
{
	// If the key is pressed increase the speed at which the camera moves.  
	// If not slow down the move speed.
	if(keydown)
	{

		m_rightMoveSpeed += 5.0f / m_frameTime;

		if(m_rightMoveSpeed > (15.0f / m_frameTime))
		{
			m_rightMoveSpeed = 15.0f / m_frameTime;
		}
	}
	else
	{
		m_rightMoveSpeed -= 5.0f / m_frameTime;

		if(m_rightMoveSpeed < 0.0f)
		{
			m_rightMoveSpeed = 0.0f;
		}
	}

	// Modify speed based on rotation.
	D3DXMATRIX worldMatrix, moveMatrix, rotationMatrix;
	D3DXMatrixTranslation(&moveMatrix, 0, 1*m_rightMoveSpeed, 0);
	D3DXMatrixRotationYawPitchRoll(&rotationMatrix, m_rotationX * -0.0174532925f, 
		m_rotationY * -0.0174532925f, 
		m_rotationZ * -0.0174532925f);
	worldMatrix = moveMatrix * rotationMatrix;

	// Update the position.
	m_positionY += worldMatrix._41;
	m_positionX += worldMatrix._42;
	m_positionZ += worldMatrix._43;

	return;
}

void PositionClass::MoveUp(bool keydown)
{
	// If the key is pressed increase the speed at which the camera moves.  
	// If not slow down the move speed.
	if(keydown)
	{
		m_upMoveSpeed += 5.0f / m_frameTime;

		if(m_upMoveSpeed > (15.0f / m_frameTime))
		{
			m_upMoveSpeed = 15.0f / m_frameTime;
		}
	}
	else
	{
		m_upMoveSpeed -= 5.0f / m_frameTime;

		if(m_upMoveSpeed < 0.0f)
		{

			m_upMoveSpeed = 0.0f;
		}
	}

	// Modify speed based on rotation.
	D3DXMATRIX worldMatrix, moveMatrix, rotationMatrix;
	D3DXMatrixTranslation(&moveMatrix, m_upMoveSpeed, 0, 0);
	D3DXMatrixRotationYawPitchRoll(&rotationMatrix, m_rotationX * -0.0174532925f, 
		m_rotationY * -0.0174532925f, 
		m_rotationZ * -0.0174532925f);
	worldMatrix = moveMatrix * rotationMatrix;

	// Update the position.
	m_positionY += worldMatrix._41;
	m_positionX += worldMatrix._42;
	m_positionZ += worldMatrix._43;

	return;
}

void PositionClass::MoveDown(bool keydown)
{
	// If the key is pressed increase the speed at which the camera moves.  
	// If not slow down the move speed.
	if(keydown)
	{

		m_downMoveSpeed += 5.0f / m_frameTime;

		if(m_downMoveSpeed > (15.0f / m_frameTime))
		{
			m_downMoveSpeed = 15.0f / m_frameTime;
		}
	}
	else
	{
		m_downMoveSpeed -= 5.0f / m_frameTime;

		if(m_downMoveSpeed < 0.0f)
		{
			m_downMoveSpeed = 0.0f;
		}
	}

	// Modify speed based on rotation.
	D3DXMATRIX worldMatrix, moveMatrix, rotationMatrix;
	D3DXMatrixTranslation(&moveMatrix, -1*m_downMoveSpeed, 0, 0);
	D3DXMatrixRotationYawPitchRoll(&rotationMatrix, m_rotationX * -0.0174532925f, 
		m_rotationY * -0.0174532925f, 
		m_rotationZ * -0.0174532925f);
	worldMatrix = moveMatrix * rotationMatrix;

	// Update the position.
	m_positionY += worldMatrix._41;
	m_positionX += worldMatrix._42;
	m_positionZ += worldMatrix._43;

	return;
}