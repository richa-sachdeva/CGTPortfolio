// Based on tutorials from http://www.rastertek.com
// For core module AG1101A - Programming for Games, DirectX Coursework, University of Abertay, Dundee.
// By Richa Sachdeva, 1304869, M.Sc. Computer Games Technology
////////////////////////////////////////////////////////////////////////////////
// Filename: positionclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _POSITIONCLASS_H_
#define _POSITIONCLASS_H_
//////////////
// INCLUDES //
//////////////
#include <math.h>
////////////////////////////////////////////////////////////////////////////////
// Class name: PositionClass

////////////////////////////////////////////////////////////////////////////////
class PositionClass
{
public:
	PositionClass();
	PositionClass(const PositionClass&);
	~PositionClass();

	void SetPosition(float, float, float);
	void SetRotation(float, float, float);

	void GetPosition(float&, float&, float&);
	void GetRotation(float&, float&, float&);

	void SetFrameTime(float);

	void MoveForward(bool);
	void MoveBackward(bool);
	void MoveUp(bool);
	void MoveDown(bool);
	void MoveLeft(bool);
	void MoveRight(bool);

	void TurnLeft(bool);
	void TurnRight(bool);
	void TurnUp(bool);
	void TurnDown(bool);
	void RollLeft(bool);
	void RollRight(bool);

private:
	float m_positionX, m_positionY, m_positionZ;
	float m_rotationX, m_rotationY, m_rotationZ;

	float m_frameTime;

	float m_leftTurnSpeed, m_rightTurnSpeed;
	float m_leftRollSpeed, m_rightRollSpeed;
	float m_upTurnSpeed, m_downTurnSpeed;

	float m_forwardMoveSpeed, m_backwardMoveSpeed;
	float m_leftMoveSpeed, m_rightMoveSpeed;
	float m_upMoveSpeed, m_downMoveSpeed;
};

#endif