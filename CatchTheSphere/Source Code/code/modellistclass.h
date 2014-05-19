// Based on tutorials from http://www.rastertek.com
// For core module AG1102A - Game Programming for PC and Xbox, DirectX Game, University of Abertay, Dundee.
// By Richa Sachdeva, 1304869, M.Sc. Computer Games Technology
///////////////////////////////////////////////////////////////////////////////
// Filename: modellistclass.h
///////////////////////////////////////////////////////////////////////////////
#ifndef _MODELLISTCLASS_H_
#define _MODELLISTCLASS_H_


//////////////
// INCLUDES //
//////////////
#include <d3dx10math.h>
#include <stdlib.h>
#include <time.h>
#include <d3dx10math.h>

///////////////////////////////////////////////////////////////////////////////
// Class name: ModelListClass
///////////////////////////////////////////////////////////////////////////////

const float MAX_SPEED = 2.0f;
const float STEP = 0.25f;

class ModelListClass
{
private:
	struct ModelInfoType
	{
		D3DXVECTOR4 color;
		float positionX, positionY, positionZ;
		float dx, dz;
	};

public:
	ModelListClass();
	ModelListClass(const ModelListClass&);
	~ModelListClass();

	bool Initialize(int);
	void Shutdown();

	int GetModelCount();
	void GetData(int, float&, float&, float&, D3DXVECTOR4&);

	D3DXVECTOR3 GetModelPosition(int index);

	void MoveModelX(int, float, float);
	void MoveModelZ(int, float, float);

	void GetVel(int, float&, float&);

private:

	// To move the Model
	void Constrain(int, float, float);


	int m_modelCount;
	ModelInfoType* m_ModelInfoList;
};

#endif