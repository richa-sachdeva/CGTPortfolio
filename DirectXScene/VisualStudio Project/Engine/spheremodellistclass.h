// Based on tutorials from http://www.rastertek.com
// For core module AG1101A - Programming for Games, DirectX Coursework, University of Abertay, Dundee.
// By Richa Sachdeva, 1304869, M.Sc. Computer Games Technology
///////////////////////////////////////////////////////////////////////////////
// Filename: spheremodellist.h
///////////////////////////////////////////////////////////////////////////////
#ifndef _SPHEREMODELLISTCLASS_H_
#define _SPHEREMODELLISTCLASS_H_


//////////////
// INCLUDES //
//////////////
#include <d3dx10math.h>
#include <stdlib.h>
#include <time.h>


///////////////////////////////////////////////////////////////////////////////
// Class name: SphereModelListClass
///////////////////////////////////////////////////////////////////////////////
class SphereModelListClass
{
private:
	struct ModelInfoType
	{
		D3DXVECTOR4 color;
		float positionX, positionY, positionZ;
	};

	struct ModelPosition
	{
		float posX, posY, posZ;
	};

public:
	SphereModelListClass();
	SphereModelListClass(const SphereModelListClass&);
	~SphereModelListClass();

	bool Initialize(int);
	void Shutdown();

	int GetModelCount();
	void GetData(int, float&, float&, float&, D3DXVECTOR4&);

private:
	int m_modelCount;
	ModelInfoType* m_ModelInfoList;
	ModelPosition* m_ModelPosition;
};

#endif