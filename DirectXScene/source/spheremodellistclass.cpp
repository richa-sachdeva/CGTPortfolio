// Based on tutorials from http://www.rastertek.com
// For core module AG1101A - Programming for Games, DirectX Coursework, University of Abertay, Dundee.
// By Richa Sachdeva, 1304869, M.Sc. Computer Games Technology
///////////////////////////////////////////////////////////////////////////////
// Filename: spheremodellistclass.cpp
///////////////////////////////////////////////////////////////////////////////
#include "spheremodellistclass.h"


SphereModelListClass::SphereModelListClass()
{
	m_ModelInfoList = 0;
	m_ModelPosition = 0;
}


SphereModelListClass::SphereModelListClass(const SphereModelListClass& other)
{
}


SphereModelListClass::~SphereModelListClass()
{
}


bool SphereModelListClass::Initialize(int numModels)
{
	int i;
	float red, green, blue;
	float addToY = 12.0f, addToZ = -6.0f, addToX = 7.0f; //3.125f, -2.625f
	float posX = -15.0f, posY = 12.0f, posZ = 6.0f;

	// Store the number of models.
	m_modelCount = numModels;

	// Create a list array of the model information.
	m_ModelInfoList = new ModelInfoType[m_modelCount];
	if(!m_ModelInfoList)
	{
		return false;
	}

	// Create a list array of the model information.
	m_ModelPosition = new ModelPosition[m_modelCount];
	if(!m_ModelPosition)
	{
		return false;
	}

	// add positions
	int width = 5, index;
	for(int i=0; i<width; i++)
	{
		for(int j=0; j<width; j++)
		{
			index = (width*i) +j;
			m_ModelPosition[index].posX = posX + (addToX * j);
			m_ModelPosition[index].posY = posY;
			m_ModelPosition[index].posZ = posZ + (addToZ * i);
		}
	}

	// Seed the random generator with the current time.
	srand((unsigned int)time(NULL));

	// Go through all the models and randomly generate the model color and position.
	for(i=0; i<m_modelCount; i++)
	{
		// Generate a random color for the model.
		red = (float)rand() / RAND_MAX;
		green = (float)rand() / RAND_MAX;
		blue = (float)rand() / RAND_MAX;

		m_ModelInfoList[i].color = D3DXVECTOR4(red, green, blue, 1.0f);

		// Generate a random position in front of the viewer for the mode.
		//m_ModelInfoList[i].positionX = (((float)rand()-(float)rand())/RAND_MAX) * 20.0f;														//
		//m_ModelInfoList[i].positionY = addToY + ((float)rand()/(((float)RAND_MAX + 1) * 5.0));	// Y position can be between 8 - 12, so 10 +- 2, so add random fucntion which takes -2 to 2.
		//m_ModelInfoList[i].positionZ = ((((float)rand()-(float)rand())/RAND_MAX) * 15.0f);// + 15.0f;

		m_ModelInfoList[i].positionX = m_ModelPosition[i].posX;
		m_ModelInfoList[i].positionY = m_ModelPosition[i].posY;
		m_ModelInfoList[i].positionZ = m_ModelPosition[i].posZ;
	}

	return true;
}


void SphereModelListClass::Shutdown()
{
	// Release the model information list.
	if(m_ModelInfoList)
	{
		delete [] m_ModelInfoList;
		m_ModelInfoList = 0;
	}

	return;
}


int SphereModelListClass::GetModelCount()
{
	return m_modelCount;
}


void SphereModelListClass::GetData(int index, float& positionX, float& positionY, float& positionZ, D3DXVECTOR4& color)
{
	positionX = m_ModelInfoList[index].positionX;
	positionY = m_ModelInfoList[index].positionY;
	positionZ = m_ModelInfoList[index].positionZ;

	color = m_ModelInfoList[index].color;

	return;
}