// Based on tutorials from http://www.rastertek.com
// For core module AG1102A - Game Programming for PC and Xbox, DirectX Game, University of Abertay, Dundee.
// By Richa Sachdeva, 1304869, M.Sc. Computer Games Technology
////////////////////////////////////////////////////////////////////////////////
// Filename: positionclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "positionclass.h"


PositionClass::PositionClass()
{
	m_positionX = 0.0f;
	m_positionY = 0.0f;
	m_positionZ = 0.0f;
	
	m_rotationX = 0.0f;
	m_rotationY = 0.0f;
	m_rotationZ = 0.0f;

	m_frameTime = 0.0f;

	m_forwardSpeed   = 0.0f;
	m_backwardSpeed  = 0.0f;
	m_upwardSpeed    = 0.0f;
	m_downwardSpeed  = 0.0f;
	m_leftTurnSpeed  = 0.0f;
	m_rightTurnSpeed = 0.0f;
	m_lookUpSpeed    = 0.0f;
	m_lookDownSpeed  = 0.0f;

	// for sphere
	m_sphereX = 0;
	m_sphereY = 0;
	m_sphereZ = 0;
	m_fwd = 0;
	m_bkwd = 0;
	m_left = 0;
	m_right = 0;
}


PositionClass::PositionClass(const PositionClass& other)
{
}


PositionClass::~PositionClass()
{
}


void PositionClass::SetPosition(float x, float y, float z)
{
	m_positionX = x;
	m_positionY = y;
	m_positionZ = z;
	return;
}


void PositionClass::SetRotation(float x, float y, float z)
{
	m_rotationX = x;
	m_rotationY = y;
	m_rotationZ = z;
	return;
}


void PositionClass::GetPosition(float& x, float& y, float& z)
{
	x = m_positionX;
	y = m_positionY;
	z = m_positionZ;
	return;
}


void PositionClass::GetRotation(float& x, float& y, float& z)
{
	x = m_rotationX;
	y = m_rotationY;
	z = m_rotationZ;
	return;
}


void PositionClass::SetFrameTime(float time)
{
	m_frameTime = time;
	return;
}


void PositionClass::MoveForward(bool keydown)
{
	float radians;


	// Update the forward speed movement based on the frame time and whether the user is holding the key down or not.
	if(keydown)
	{
		m_forwardSpeed += m_frameTime * 0.001f;

		if(m_forwardSpeed > (m_frameTime * 0.03f))
		{
			m_forwardSpeed = m_frameTime * 0.03f;
		}
	}
	else
	{
		m_forwardSpeed -= m_frameTime * 0.0007f;

		if(m_forwardSpeed < 0.0f)
		{
			m_forwardSpeed = 0.0f;
		}
	}

	// Convert degrees to radians.
	radians = m_rotationY * 0.0174532925f;

	// Update the position.
	m_positionX += sinf(radians) * m_forwardSpeed;
	m_positionZ += cosf(radians) * m_forwardSpeed;

	return;
}


void PositionClass::MoveBackward(bool keydown)
{
	float radians;


	// Update the backward speed movement based on the frame time and whether the user is holding the key down or not.
	if(keydown)
	{
		m_backwardSpeed += m_frameTime * 0.001f;

		if(m_backwardSpeed > (m_frameTime * 0.03f))
		{
			m_backwardSpeed = m_frameTime * 0.03f;
		}
	}
	else
	{
		m_backwardSpeed -= m_frameTime * 0.0007f;
		
		if(m_backwardSpeed < 0.0f)
		{
			m_backwardSpeed = 0.0f;
		}
	}

	// Convert degrees to radians.
	radians = m_rotationY * 0.0174532925f;

	// Update the position.
	m_positionX -= sinf(radians) * m_backwardSpeed;
	m_positionZ -= cosf(radians) * m_backwardSpeed;

	return;
}


void PositionClass::MoveUpward(bool keydown)
{
	// Update the upward speed movement based on the frame time and whether the user is holding the key down or not.
	if(keydown)
	{
		m_upwardSpeed += m_frameTime * 0.003f;

		if(m_upwardSpeed > (m_frameTime * 0.03f))
		{
			m_upwardSpeed = m_frameTime * 0.03f;
		}
	}
	else
	{
		m_upwardSpeed -= m_frameTime * 0.002f;

		if(m_upwardSpeed < 0.0f)
		{
			m_upwardSpeed = 0.0f;
		}
	}

	// Update the height position.
	m_positionY += m_upwardSpeed;

	return;
}


void PositionClass::MoveDownward(bool keydown)
{
	// Update the downward speed movement based on the frame time and whether the user is holding the key down or not.
	if(keydown)
	{
		m_downwardSpeed += m_frameTime * 0.003f;

		if(m_downwardSpeed > (m_frameTime * 0.03f))
		{
			m_downwardSpeed = m_frameTime * 0.03f;
		}
	}
	else
	{
		m_downwardSpeed -= m_frameTime * 0.002f;

		if(m_downwardSpeed < 0.0f)
		{
			m_downwardSpeed = 0.0f;
		}
	}

	// Update the height position.
	m_positionY -= m_downwardSpeed;

	return;
}


void PositionClass::TurnLeft(bool keydown)
{
	// Update the left turn speed movement based on the frame time and whether the user is holding the key down or not.
	if(keydown)
	{
		m_leftTurnSpeed += m_frameTime * 0.01f;

		if(m_leftTurnSpeed > (m_frameTime * 0.15f))
		{
			m_leftTurnSpeed = m_frameTime * 0.15f;
		}
	}
	else
	{
		m_leftTurnSpeed -= m_frameTime* 0.005f;

		if(m_leftTurnSpeed < 0.0f)
		{
			m_leftTurnSpeed = 0.0f;
		}
	}

	// Update the rotation.
	m_rotationY -= m_leftTurnSpeed;
	m_sphRotY -= m_leftTurnSpeed;

	// Keep the rotation in the 0 to 360 range.
	if(m_rotationY < 0.0f)
	{
		m_rotationY += 360.0f;
		m_sphRotY += 360.0f;
	}

	return;
}


void PositionClass::TurnRight(bool keydown)
{
	// Update the right turn speed movement based on the frame time and whether the user is holding the key down or not.
	if(keydown)
	{
		m_rightTurnSpeed += m_frameTime * 0.01f;

		if(m_rightTurnSpeed > (m_frameTime * 0.15f))
		{
			m_rightTurnSpeed = m_frameTime * 0.15f;
		}
	}
	else
	{
		m_rightTurnSpeed -= m_frameTime* 0.005f;

		if(m_rightTurnSpeed < 0.0f)
		{
			m_rightTurnSpeed = 0.0f;
		}
	}

	// Update the rotation.
	m_rotationY += m_rightTurnSpeed;

	// Keep the rotation in the 0 to 360 range.
	if(m_rotationY > 360.0f)
	{
		m_rotationY -= 360.0f;
	}

	return;
}


void PositionClass::LookUpward(bool keydown)
{
	// Update the upward rotation speed movement based on the frame time and whether the user is holding the key down or not.
	if(keydown)
	{
		m_lookUpSpeed += m_frameTime * 0.01f;

		if(m_lookUpSpeed > (m_frameTime * 0.15f))
		{
			m_lookUpSpeed = m_frameTime * 0.15f;
		}
	}
	else
	{
		m_lookUpSpeed -= m_frameTime* 0.005f;

		if(m_lookUpSpeed < 0.0f)
		{
			m_lookUpSpeed = 0.0f;
		}
	}

	// Update the rotation.
	m_rotationX -= m_lookUpSpeed;

	// Keep the rotation maximum 90 degrees.
	if(m_rotationX > 90.0f)
	{
		m_rotationX = 90.0f;
	}

	return;
}


void PositionClass::LookDownward(bool keydown)
{
	// Update the downward rotation speed movement based on the frame time and whether the user is holding the key down or not.
	if(keydown)
	{
		m_lookDownSpeed += m_frameTime * 0.01f;

		if(m_lookDownSpeed > (m_frameTime * 0.15f))
		{
			m_lookDownSpeed = m_frameTime * 0.15f;
		}
	}
	else
	{
		m_lookDownSpeed -= m_frameTime* 0.005f;

		if(m_lookDownSpeed < 0.0f)
		{
			m_lookDownSpeed = 0.0f;
		}
	}

	// Update the rotation.
	m_rotationX += m_lookDownSpeed;

	// Keep the rotation maximum 90 degrees.
	if(m_rotationX < -90.0f)
	{
		m_rotationX = -90.0f;
	}

	return;
}

void PositionClass::MoveSphereForward(bool keydown){
	float radians;

	// Update the forward speed movement based on the frame time and whether the user is holding the key down or not.
	if(keydown)
	{
		m_fwd += m_frameTime * 0.001f;

		if(m_fwd > (m_frameTime * 0.03f))
		{
			m_fwd = m_frameTime * 0.03f;
		}
	}
	else
	{
		m_fwd -= m_frameTime * 0.0007f;

		if(m_fwd < 0.0f)
		{
			m_fwd = 0.0f;
		}
	}

	m_sphereX += m_fwd;

	return;
}

void PositionClass::MoveSphereBackward(bool keydown){
	float radians;
	// Update the backward speed movement based on the frame time and whether the user is holding the key down or not.
	if(keydown)
	{
		m_bkwd += m_frameTime * 0.001f;

		if(m_bkwd > (m_frameTime * 0.03f))
		{
			m_bkwd = m_frameTime * 0.03f;
		}
	}
	else
	{
		m_bkwd -= m_frameTime * 0.0007f;
		
		if(m_bkwd < 0.0f)
		{
			m_bkwd = 0.0f;
		}
	}
	m_sphereX -= m_bkwd;

	return;
}

void PositionClass::TurnSphereLeft(bool keydown){
	float radians;
	// Update the backward speed movement based on the frame time and whether the user is holding the key down or not.
	if(keydown)
	{
		m_left += m_frameTime * 0.001f;

		if(m_left > (m_frameTime * 0.03f))
		{
			m_left = m_frameTime * 0.03f;
		}
	}
	else
	{
		m_left -= m_frameTime * 0.0007f;
		
		if(m_left < 0.0f)
		{
			m_left = 0.0f;
		}
	}

	// Convert degrees to radians.
	//m_sphRotY -= m_left;
	//radians = m_sphRotY * 0.0174532925f;

	// Update the position.
	/*m_positionX += sinf(radians) * m_forwardSpeed;
	m_positionZ += cosf(radians) * m_forwardSpeed;*/

	// Update the position.
	m_sphereZ +=   m_left;
	

	// Keep the rotation in the 0 to 360 range.
	/*if(m_sphRotY < 0.0f)
	{
		m_sphRotY += 360.0f;
	}*/

	return;
}

void PositionClass::TurnSphereRight(bool keydown){
	float radians;

	if(keydown)
	{
		m_right += m_frameTime * 0.001f;

		if(m_right > (m_frameTime * 0.03f))
		{
			m_right = m_frameTime * 0.03f;
		}
	}
	else
	{
		m_right -= m_frameTime * 0.0007f;
		
		if(m_right < 0.0f)
		{
			m_right = 0.0f;
		}
	}

	//m_sphRotY += m_right;
	//radians = m_sphRotY * 0.0174532925f;

	// Update the position.
	m_sphereZ -= m_right;

	//// Keep the rotation in the 0 to 360 range.
	//if(m_sphRotY < 0.0f)
	//{
	//	m_sphRotY += 360.0f;
	//}

	return;
}

void PositionClass::SetSpherePosition(float x, float y, float z)
{
	m_sphereX = x;
	m_sphereY = y;
	m_sphereZ = z;
	return;
}

void PositionClass::GetSpherePosition(float& x, float& y, float& z)
{
	x = m_sphereX;
	y = m_sphereY;
	z = m_sphereZ;
	return;
}

void PositionClass::SetSphereRotation(float x, float y, float z)
{
	m_sphRotX = x;
	m_sphRotY = y;
	m_sphRotZ = x;
	return;
}

void PositionClass::GetSphereRotation(float& x, float& y, float& z)
{
	x = m_sphRotX;
	y = m_sphRotY;
	z = m_sphRotZ;
	return;
}

void PositionClass::RotateSphereLeft(bool keydown){
	// Update the left turn speed movement based on the frame time and whether the user is holding the key down or not.
	if(keydown)
	{
		m_left += m_frameTime * 0.01f;

		if(m_left > (m_frameTime * 0.15f))
		{
			m_left = m_frameTime * 0.15f;
		}
	}
	else
	{
		m_left -= m_frameTime* 0.005f;

		if(m_left < 0.0f)
		{
			m_left = 0.0f;
		}
	}

	// Update the position.
	//m_sphereZ +=  m_left;

	// Update the rotation.
	m_sphRotY -= m_left;

	// Keep the rotation in the 0 to 360 range.
	if(m_sphRotY < 0.0f)
	{
		m_sphRotY += 360.0f;
	}

	return;
}

void PositionClass::RotateSphereRight(bool keydown){
	// Update the right turn speed movement based on the frame time and whether the user is holding the key down or not.
	if(keydown)
	{
		m_right += m_frameTime * 0.01f;

		if(m_right > (m_frameTime * 0.15f))
		{
			m_right = m_frameTime * 0.15f;
		}
	}
	else
	{
		m_right -= m_frameTime* 0.005f;

		if(m_right < 0.0f)
		{
			m_right = 0.0f;
		}
	}

	// Update the rotation.
	m_sphRotY += m_right;

	// Keep the rotation in the 0 to 360 range.
	if(m_sphRotY > 360.0f)
	{
		m_sphRotY -= 360.0f;
	}

	return;
}