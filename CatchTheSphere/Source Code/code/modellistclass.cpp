// Based on tutorials from http://www.rastertek.com
// For core module AG1102A - Game Programming for PC and Xbox, DirectX Game, University of Abertay, Dundee.
// By Richa Sachdeva, 1304869, M.Sc. Computer Games Technology
///////////////////////////////////////////////////////////////////////////////
// Filename: modellistclass.cpp
///////////////////////////////////////////////////////////////////////////////
#include "modellistclass.h"


ModelListClass::ModelListClass()
{
	m_ModelInfoList = 0;
}


ModelListClass::ModelListClass(const ModelListClass& other)
{
}


ModelListClass::~ModelListClass()
{
}


bool ModelListClass::Initialize(int numModels)
{
	int i;
	float red, green, blue;


	// Store the number of models.
	m_modelCount = numModels;

	// Create a list array of the model information.
	m_ModelInfoList = new ModelInfoType[m_modelCount];
	if(!m_ModelInfoList)
	{
		return false;
	}

	// Seed the random generator with the current time.
	srand((unsigned int)time(NULL));

	// Go through all the models and randomly generate the model color and position.
	float posX = 15, posZ = 22;
	float velX, velY;
	for(i=1; i<m_modelCount; i+=2)
	{
		// Generate a random color for the model.
		red = 1.0f; //(float)rand() / RAND_MAX;
		green = 0.0f;//(float)rand() / RAND_MAX;
		blue = 0.0f;//(float)rand() / RAND_MAX;

		m_ModelInfoList[i-1].color = D3DXVECTOR4(red, green, blue, 1.0f);

		// Generate a random position in front of the viewer for the mode.
		m_ModelInfoList[i-1].positionX = posX; //(((float)rand()-(float)rand())/RAND_MAX) * 10.0f;
		m_ModelInfoList[i-1].positionY = 1.5f;//(((float)rand()-(float)rand())/RAND_MAX) * 10.0f;
		m_ModelInfoList[i-1].positionZ = posZ;//((((float)rand()-(float)rand())/RAND_MAX) * 10.0f) + 5.0f;

		velX = (((int)rand()-(int)rand())/RAND_MAX) * 1;
		velY = (((int)rand()-(int)rand())/RAND_MAX) * 1;

	//	if( velX == velY){
			// Both are same
			velX = 0;
			velY = 0.5f;
	//	}
		m_ModelInfoList[i-1].dx = velX;
		m_ModelInfoList[i-1].dz = velY;
		
		posX += 10;

		red = 0.0f;
		green = 0.0f;
		blue = 1.0f;

		m_ModelInfoList[i].color = D3DXVECTOR4(red, green, blue, 1.0f);
		m_ModelInfoList[i].positionX = posX; 
		m_ModelInfoList[i].positionY = 1.5f;
		m_ModelInfoList[i].positionZ = posZ;

		velX = (((int)rand()-(int)rand())/RAND_MAX) * 1;
		velY = (((int)rand()-(int)rand())/RAND_MAX) * 1;

	//	if( velX == velY){
			// Both are same
			velX = -0.5;
			velY = 0;
	//	}

		m_ModelInfoList[i].dx = velX;
		m_ModelInfoList[i].dz = velY;

		if((i+1) > 19){
			posZ += 31;
			posX += 20;
			continue;
		}
		else if((i+1) > 15){
			posZ += 33;
			posX += 20;
			continue;
		}
		else if((i+1) > 11){
			posZ += 31;
			posX += 20;
			continue;
		}
		else if((i+1) > 9){
			posZ += 31;
			posX += 20;
			continue;
		}
		else if((i+1) > 7){
			posZ += 33;
			posX += 20;
			continue;
		}
		else if ((i+1) > 5){
			posZ += 34;
			posX += 20;
			continue;
		}
		else if((i+1) > 3){
			posZ += 31;
			posX += 15;
			continue;
		}
		else{
			posZ += 33;
			posX += 15;
		}
	}
	return true;
}


void ModelListClass::Shutdown()
{
	// Release the model information list.
	if(m_ModelInfoList)
	{
		delete [] m_ModelInfoList;
		m_ModelInfoList = 0;
	}

	return;
}


int ModelListClass::GetModelCount()
{
	return m_modelCount;
}


void ModelListClass::GetData(int index, float& positionX, float& positionY, float& positionZ, D3DXVECTOR4& color)
{
	positionX = m_ModelInfoList[index].positionX;
	positionY = m_ModelInfoList[index].positionY;
	positionZ = m_ModelInfoList[index].positionZ;

	color = m_ModelInfoList[index].color;

	return;
}

void ModelListClass::GetVel(int index, float& velX, float& velZ){
	velX = m_ModelInfoList[index].dx;
	velZ = m_ModelInfoList[index].dz;
}

void ModelListClass::MoveModelX(int index, float start, float end){
	Constrain(index, start, end);
	m_ModelInfoList[index].positionX += m_ModelInfoList[index].dx;
	m_ModelInfoList[index].positionZ += m_ModelInfoList[index].dz;
}

void ModelListClass::MoveModelZ(int index, float start, float end){
	Constrain(index, start, end);
	m_ModelInfoList[index].positionZ += m_ModelInfoList[index].dz;
	m_ModelInfoList[index].positionX += m_ModelInfoList[index].dx;
}

void ModelListClass::Constrain(int index,  float start, float end){

	if(m_ModelInfoList[index].positionZ < start)
    {
		/*if(m_ModelInfoList[index].dz < MAX_SPEED)
			m_ModelInfoList[index].dz += STEP;*/
        m_ModelInfoList[index].dz = abs(m_ModelInfoList[index].dz);
    }
	else if (m_ModelInfoList[index].positionZ > end)
    {
		if(m_ModelInfoList[index].dz > -MAX_SPEED)
			m_ModelInfoList[index].dz -= STEP;
        m_ModelInfoList[index].dz = -abs(m_ModelInfoList[index].dz);
    }
	else{
		if(m_ModelInfoList[index].dz < MAX_SPEED && m_ModelInfoList[index].dz > 0)
			m_ModelInfoList[index].dz += STEP;
		else if(abs(m_ModelInfoList[index].dz) < MAX_SPEED && m_ModelInfoList[index].dz < 0)
			m_ModelInfoList[index].dz -= STEP;
	}

	if(m_ModelInfoList[index].positionX < start)
	{
		m_ModelInfoList[index].dx = abs(m_ModelInfoList[index].dx);
	}
	else if (m_ModelInfoList[index].positionX > end)
	{
		m_ModelInfoList[index].dx = -abs(m_ModelInfoList[index].dx);
	}
	else{
		if(m_ModelInfoList[index].dx < MAX_SPEED && m_ModelInfoList[index].dx > 0)
			m_ModelInfoList[index].dx += STEP;
		else if(abs(m_ModelInfoList[index].dx) < MAX_SPEED && m_ModelInfoList[index].dx < 0)
			m_ModelInfoList[index].dx -= STEP;
	}

}

D3DXVECTOR3 ModelListClass::GetModelPosition(int index){

	return D3DXVECTOR3(m_ModelInfoList[index].positionX, m_ModelInfoList[index].positionY, m_ModelInfoList[index].positionZ);
}