// Based on tutorials from http://www.rastertek.com
// For core module AG1101A - Programming for Games, DirectX Coursework, University of Abertay, Dundee.
// By Richa Sachdeva, 1304869, M.Sc. Computer Games Technology
////////////////////////////////////////////////////////////////////////////////
// Filename: cameraclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _CAMERACLASS_H_
#define _CAMERACLASS_H_
//////////////
// INCLUDES //
//////////////
#include <d3dx10math.h>
////////////////////////////////////////////////////////////////////////////////

// Class name: CameraClass
////////////////////////////////////////////////////////////////////////////////
class CameraClass
{
public:
	CameraClass();
	CameraClass(const CameraClass&);
	~CameraClass();

	void SetPosition(float, float, float);
	void SetRotation(float, float, float);
	D3DXVECTOR3 GetPosition();
	D3DXVECTOR3 GetRotation();
	void Render();
	void GetViewMatrix(D3DXMATRIX&);

	// for reflection
	void RenderReflection(float);
	D3DXMATRIX GetReflectionViewMatrix();

private:
	D3DXMATRIX m_viewMatrix;
	float m_positionX, m_positionY, m_positionZ;
	float m_rotationX, m_rotationY, m_rotationZ;

	D3DXMATRIX m_reflectionViewMatrix;
};

#endif


