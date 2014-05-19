// Based on tutorials from http://www.rastertek.com
// For core module AG1102A - Game Programming for PC and Xbox, DirectX Game, University of Abertay, Dundee.
// By Richa Sachdeva, 1304869, M.Sc. Computer Games Technology
////////////////////////////////////////////////////////////////////////////////
// Filename: terrainclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _TERRAINCLASS_H_
#define _TERRAINCLASS_H_


//////////////
// INCLUDES //
//////////////
#include <d3d11.h>
#include <d3dx10math.h>
#include <stdio.h>


////////////////////////////////////////////////////////////////////////////////
// Class name: TerrainClass
////////////////////////////////////////////////////////////////////////////////
class TerrainClass
{
private:
	struct VertexType
	{
		D3DXVECTOR3 position;
	//	D3DXVECTOR2 texture;
	    D3DXVECTOR3 normal;
		D3DXVECTOR4 color;
	};

	struct HeightMapType 
	{ 
		float x, y, z;
		float nx, ny, nz;
		float r, g, b;
	};

	struct VectorType 
	{ 
		float x, y, z;
	};

public:
	TerrainClass();
	TerrainClass(const TerrainClass&);
	~TerrainClass();

	// algorithmic terrain
	bool InitializeTerrain(ID3D11Device*, int terrainWidth, int terrainHeight);
	bool DoubleTerrain(ID3D11Device*, int);
	
	void Shutdown();
	void Render(ID3D11DeviceContext*);
	
	int  GetIndexCount();

	int GetVertexCount();
	void CopyVertexArray(void*);

	int GetTerrainHeight(){ return m_terrainWidth;}
	int GetTerrainWidth(){ return m_terrainHeight;}
	// Boundary of the terrain
	float getStartX();
	float getStartZ();

	float getEndX();
	float getEndZ();

	float getBaseY();

private:
	bool LoadHeightMap(char*);
	void NormalizeHeightMap();
	bool CalculateNormals();
	void ShutdownHeightMap();

	bool InitializeBuffers(ID3D11Device*);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext*);

	// for generating the terrain
	float GenerateCorrectHeight(int, int, int);
	float SetHeight(int i, int j);

	// for smoothing out the terrain -> generated height
	bool SmoothHeightMap();
	float CalculateSmoothSum(int, int);
	
private:

	int m_terrainWidth, m_terrainHeight;
	int m_vertexCount, m_indexCount;
	ID3D11Buffer *m_vertexBuffer, *m_indexBuffer;
	HeightMapType* m_heightMap;

	VertexType* m_vertices;
};

#endif