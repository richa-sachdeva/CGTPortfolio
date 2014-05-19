// Based on tutorials from http://www.rastertek.com
// For core module AG1102A - Game Programming for PC and Xbox, DirectX Game, University of Abertay, Dundee.
// By Richa Sachdeva, 1304869, M.Sc. Computer Games Technology
////////////////////////////////////////////////////////////////////////////////
// Filename: terrainclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "terrainclass.h"
#include <cmath>


TerrainClass::TerrainClass()
{
	m_vertexBuffer = 0;
	m_indexBuffer = 0;
	m_heightMap = 0;
	m_vertices = 0;
}


TerrainClass::TerrainClass(const TerrainClass& other)
{
}


TerrainClass::~TerrainClass()
{
}

bool TerrainClass::InitializeTerrain(ID3D11Device* device, int terrainWidth, int terrainHeight)
{
	int index;
	float height = 0.0;
	bool result;

	// Save the dimensions of the terrain.
	m_terrainWidth = terrainWidth;
	m_terrainHeight = terrainHeight;

	// Create the structure to hold the terrain data.
	m_heightMap = new HeightMapType[m_terrainWidth * m_terrainHeight];
	if(!m_heightMap)
	{
		return false;
	}

	// Initialise the data in the height map (flat).
	for(int j=0; j<m_terrainHeight; j++)
	{
		for(int i=0; i<m_terrainWidth; i++)
		{			
			index = (m_terrainHeight * j) + i;

			m_heightMap[index].x = (float)i;
			m_heightMap[index].y = GenerateCorrectHeight(index, j, i);//(float)height;
			m_heightMap[index].z = (float)j;

		}
	}

	// Smoothen the generated terrain -> to make it more even
	result = SmoothHeightMap();
		if(!result)
			return false;

	//even though we are generating a flat terrain, we still need to normalise it. 
	// Calculate the normals for the terrain data.
	result = CalculateNormals();
	if(!result)
	{
		return false;
	}

	// Initialize the vertex and index buffer that hold the geometry for the terrain.
	result = InitializeBuffers(device);
	if(!result)
	{
		return false;
	}

	return true;
}

bool TerrainClass::DoubleTerrain(ID3D11Device* device, int count){

	int index;
	float height = 0.0;
	bool result;

	int originalHeight = 512;
	int originalWidth = 512;

	if(count < 0 )
		return false;

	m_terrainWidth = originalWidth/count;
	m_terrainHeight = originalHeight/count;

	m_heightMap = 0;
	// Create the structure to hold the terrain data.
	m_heightMap = new HeightMapType[m_terrainWidth * m_terrainHeight];
	if(!m_heightMap)
	{
		return false;
	}

	// Initialise the data in the height map (flat).
	for(int j=0; j<m_terrainHeight; j++)
	{
		for(int i=0; i<m_terrainWidth; i++)
		{			
			index = (m_terrainHeight * j) + i;

			m_heightMap[index].x = (float)i;
			m_heightMap[index].y = GenerateCorrectHeight(index, j, i);//(float)height;
			m_heightMap[index].z = (float)j;

		}
	}

	// Smoothen the generated terrain -> to make it more even
	result = SmoothHeightMap();
		if(!result)
			return false;

	//even though we are generating a flat terrain, we still need to normalise it. 
	// Calculate the normals for the terrain data.
	result = CalculateNormals();
	if(!result)
	{
		return false;
	}

	// Initialize the vertex and index buffer that hold the geometry for the terrain.
	result = InitializeBuffers(device);
	if(!result)
	{
		return false;
	}

	return true;
}

float TerrainClass::GenerateCorrectHeight(int index, int i, int j){

	float height = 0;
	int size = 32;
	int half = size/2;
	// 0, 1, 2 -> 0; 3, 4, 5 -> 1; 6, 7, 8, 9 -> 2; 10, 11, 12 -> 1; 13, 14 15 -> 0

	int grid, index2;

	int xcomp = i%size;
	int zcomp = 0;
	int ycomp = j%size;

	int k = ceil((float)(i*j)/(float)(size*size));

	height = SetHeight(xcomp, ycomp);

	return height;
}

float TerrainClass::SetHeight(int i, int j){
	if(i==0 || i==1 || i==2 || i==13 || i==14 || i==15){
		if(j==0 || j==1 || j==2 || j==13 || j==14 || j==15)
			return 0;
		else if(j==3 || j==4 || j==5 || j==10 || j==11 || j==12)
			return 2;
		else if(j==6 || j==7 || j==8 || j==9)
			return 4;
	}
	else if(i==3 || i==4 || i==5 || i==10 || i==11 || i==12){
		if(j==0 || j==1 || j==2 || j==13 || j==14 || j==15)
			return 0;
		else if(j==3 || j==4 || j==5 || j==10 || j==11 || j==12)
			return 2;
		else if(j==6 || j==7 || j==8 || j==9)
			return 4;
	}
	else if(i==6 || i==7 || i==8 || i==9){
		if(j==0 || j==1 || j==2 || j==13 || j==14 || j==15)
			return 0;
		else if(j==3 || j==4 || j==5 || j==10 || j==11 || j==12)
			return 2;
		else if(j==6 || j==7 || j==8 || j==9)
			return 4;
	}
	return 0;
}

bool TerrainClass::SmoothHeightMap(){
	
	float smoothSum = 0.0f;

	for(int j=0; j<m_terrainHeight-1; j++)
	{
		smoothSum = 0.0f;
		for(int i=0; i<m_terrainWidth-1; i++)
		{
			smoothSum = CalculateSmoothSum(i, j);
			int index = (m_terrainHeight * j) + i;
			m_heightMap[index].y = smoothSum;
		}
	}
	return true;
}

float TerrainClass::CalculateSmoothSum(int i, int j){
	float sum = 0.0f;
	int k = i-1; if(k<0)k=0;
	int p = i+1; if(p>m_terrainWidth)p=m_terrainWidth;
	int m = j-1; if(m<0)m=0;
	int n = j+1; if(n>m_terrainHeight)n=m_terrainHeight;
	int index = 0;
	float count = 0.0f;

	for(k; k<p+1; k+=1){
		/*if(k<0 || k>m_terrainHeight)
			continue;*/
		for(m; m<n+1; m+=1){
			/*if(m < 0 || m>m_terrainWidth)
				continue;*/
			index = (m_terrainHeight * m) + k;
			sum += m_heightMap[index].y;
			count++;
		}
	}
	return sum/count;
}


void TerrainClass::Shutdown()
{
	// Release the vertex and index buffer.
	ShutdownBuffers();

	// Release the height map data.
	ShutdownHeightMap();

	return;
}


void TerrainClass::Render(ID3D11DeviceContext* deviceContext)
{
	// Put the vertex and index buffers on the graphics pipeline to prepare them for drawing.
	RenderBuffers(deviceContext);

	return;
}


int TerrainClass::GetIndexCount()
{
	return m_indexCount;
}


bool TerrainClass::LoadHeightMap(char* filename)
{
	FILE* filePtr;
	int error;
	unsigned int count;
	BITMAPFILEHEADER bitmapFileHeader;
	BITMAPINFOHEADER bitmapInfoHeader;
	int imageSize, i, j, k, index;
	unsigned char* bitmapImage;
	unsigned char height;


	// Open the height map file in binary.
	error = fopen_s(&filePtr, filename, "rb");
	if(error != 0)
	{
		return false;
	}

	// Read in the file header.
	count = fread(&bitmapFileHeader, sizeof(BITMAPFILEHEADER), 1, filePtr);
	if(count != 1)
	{
		return false;
	}

	// Read in the bitmap info header.
	count = fread(&bitmapInfoHeader, sizeof(BITMAPINFOHEADER), 1, filePtr);
	if(count != 1)
	{
		return false;
	}

	// Save the dimensions of the terrain.
	m_terrainWidth = bitmapInfoHeader.biWidth;
	m_terrainHeight = bitmapInfoHeader.biHeight;

	// Calculate the size of the bitmap image data.
	imageSize = m_terrainWidth * m_terrainHeight * 3;

	// Allocate memory for the bitmap image data.
	bitmapImage = new unsigned char[imageSize];
	if(!bitmapImage)
	{
		return false;
	}

	// Move to the beginning of the bitmap data.
	fseek(filePtr, bitmapFileHeader.bfOffBits, SEEK_SET);

	// Read in the bitmap image data.
	count = fread(bitmapImage, 1, imageSize, filePtr);
	if(count != imageSize)
	{
		return false;
	}

	// Close the file.
	error = fclose(filePtr);
	if(error != 0)
	{
		return false;
	}

	// Create the structure to hold the height map data.
	m_heightMap = new HeightMapType[m_terrainWidth * m_terrainHeight];
	if(!m_heightMap)
	{
		return false;
	}

	// Initialize the position in the image data buffer.
	k=0;

	// Read the image data into the height map.
	for(j=0; j<m_terrainHeight; j++)
	{
		for(i=0; i<m_terrainWidth; i++)
		{
			height = bitmapImage[k];
			
			index = (m_terrainHeight * j) + i;

			m_heightMap[index].x = (float)i;
			m_heightMap[index].y = (float)height;
			m_heightMap[index].z = (float)j;

			k+=3;
		}
	}

	// Release the bitmap image data.
	delete [] bitmapImage;
	bitmapImage = 0;

	return true;
}


void TerrainClass::NormalizeHeightMap()
{
	int i, j;


	for(j=0; j<m_terrainHeight; j++)
	{
		for(i=0; i<m_terrainWidth; i++)
		{
			m_heightMap[(m_terrainHeight * j) + i].y /= 15.0f;
		}
	}

	return;
}


bool TerrainClass::CalculateNormals()
{
	int i, j, index1, index2, index3, index, count;
	float vertex1[3], vertex2[3], vertex3[3], vector1[3], vector2[3], sum[3], length;
	VectorType* normals;


	// Create a temporary array to hold the un-normalized normal vectors.
	normals = new VectorType[(m_terrainHeight-1) * (m_terrainWidth-1)];
	if(!normals)
	{
		return false;
	}

	// Go through all the faces in the mesh and calculate their normals.
	for(j=0; j<(m_terrainHeight-1); j++)
	{
		for(i=0; i<(m_terrainWidth-1); i++)
		{
			index1 = (j * m_terrainHeight) + i;
			index2 = (j * m_terrainHeight) + (i+1);
			index3 = ((j+1) * m_terrainHeight) + i;

			// Get three vertices from the face.
			vertex1[0] = m_heightMap[index1].x;
			vertex1[1] = m_heightMap[index1].y;
			vertex1[2] = m_heightMap[index1].z;
		
			vertex2[0] = m_heightMap[index2].x;
			vertex2[1] = m_heightMap[index2].y;
			vertex2[2] = m_heightMap[index2].z;
		
			vertex3[0] = m_heightMap[index3].x;
			vertex3[1] = m_heightMap[index3].y;
			vertex3[2] = m_heightMap[index3].z;

			// Calculate the two vectors for this face.
			vector1[0] = vertex1[0] - vertex3[0];
			vector1[1] = vertex1[1] - vertex3[1];
			vector1[2] = vertex1[2] - vertex3[2];
			vector2[0] = vertex3[0] - vertex2[0];
			vector2[1] = vertex3[1] - vertex2[1];
			vector2[2] = vertex3[2] - vertex2[2];

			index = (j * (m_terrainHeight-1)) + i;

			// Calculate the cross product of those two vectors to get the un-normalized value for this face normal.
			normals[index].x = (vector1[1] * vector2[2]) - (vector1[2] * vector2[1]);
			normals[index].y = (vector1[2] * vector2[0]) - (vector1[0] * vector2[2]);
			normals[index].z = (vector1[0] * vector2[1]) - (vector1[1] * vector2[0]);
		}
	}

	// Now go through all the vertices and take an average of each face normal 	
	// that the vertex touches to get the averaged normal for that vertex.
	for(j=0; j<m_terrainHeight; j++)
	{
		for(i=0; i<m_terrainWidth; i++)
		{
			// Initialize the sum.
			sum[0] = 0.0f;
			sum[1] = 0.0f;
			sum[2] = 0.0f;

			// Initialize the count.
			count = 0;

			// Bottom left face.
			if(((i-1) >= 0) && ((j-1) >= 0))
			{
				index = ((j-1) * (m_terrainHeight-1)) + (i-1);

				sum[0] += normals[index].x;
				sum[1] += normals[index].y;
				sum[2] += normals[index].z;
				count++;
			}

			// Bottom right face.
			if((i < (m_terrainWidth-1)) && ((j-1) >= 0))
			{
				index = ((j-1) * (m_terrainHeight-1)) + i;

				sum[0] += normals[index].x;
				sum[1] += normals[index].y;
				sum[2] += normals[index].z;
				count++;
			}

			// Upper left face.
			if(((i-1) >= 0) && (j < (m_terrainHeight-1)))
			{
				index = (j * (m_terrainHeight-1)) + (i-1);

				sum[0] += normals[index].x;
				sum[1] += normals[index].y;
				sum[2] += normals[index].z;
				count++;
			}

			// Upper right face.
			if((i < (m_terrainWidth-1)) && (j < (m_terrainHeight-1)))
			{
				index = (j * (m_terrainHeight-1)) + i;

				sum[0] += normals[index].x;
				sum[1] += normals[index].y;
				sum[2] += normals[index].z;
				count++;
			}
			
			// Take the average of the faces touching this vertex.
			sum[0] = (sum[0] / (float)count);
			sum[1] = (sum[1] / (float)count);
			sum[2] = (sum[2] / (float)count);

			// Calculate the length of this normal.
			length = sqrt((sum[0] * sum[0]) + (sum[1] * sum[1]) + (sum[2] * sum[2]));
			
			// Get an index to the vertex location in the height map array.
			index = (j * m_terrainHeight) + i;

			// Normalize the final shared normal for this vertex and store it in the height map array.
			m_heightMap[index].nx = (sum[0] / length);
			m_heightMap[index].ny = (sum[1] / length);
			m_heightMap[index].nz = (sum[2] / length);
		}
	}

	// Release the temporary normals.
	delete [] normals;
	normals = 0;

	return true;
}


void TerrainClass::ShutdownHeightMap()
{
	if(m_heightMap)
	{
		delete [] m_heightMap;
		m_heightMap = 0;
	}

	return;
}


bool TerrainClass::InitializeBuffers(ID3D11Device* device)
{
	VertexType* vertices;
	unsigned long* indices;
	int index, i, j;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
    D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;
	int index1, index2, index3, index4;


	// Calculate the number of vertices in the terrain mesh.
	m_vertexCount = (m_terrainWidth - 1) * (m_terrainHeight - 1) * 6;

	// Set the index count to the same as the vertex count.
	m_indexCount = m_vertexCount;

	// Create the vertex array.
	m_vertices = new VertexType[m_vertexCount];
	if(!m_vertices)
	{
		return false;
	}

	// Create the index array.
	/*indices = new unsigned long[m_indexCount];
	if(!indices)
	{
		return false;
	}*/

	// Initialize the index to the vertex buffer.
	index = 0;

	// Load the vertex and index array with the terrain data.
	for(j=0; j<(m_terrainHeight-1); j++)
	{
		for(i=0; i<(m_terrainWidth-1); i++)
		{
			index1 = (m_terrainHeight * j) + i;          // Bottom left.
			index2 = (m_terrainHeight * j) + (i+1);      // Bottom right.
			index3 = (m_terrainHeight * (j+1)) + i;      // Upper left.
			index4 = (m_terrainHeight * (j+1)) + (i+1);  // Upper right.

			if(j%2 == 0){

				if(i%2 == 0){
					// / this kind of geometry
					// Upper left.
					m_vertices[index].position = D3DXVECTOR3(m_heightMap[index3].x, m_heightMap[index3].y, m_heightMap[index3].z);
					m_vertices[index].normal = D3DXVECTOR3(m_heightMap[index3].nx, m_heightMap[index3].ny, m_heightMap[index3].nz);
					if(m_heightMap[index3].y > 0.3)
						m_vertices[index].color = D3DXVECTOR4(0.5f, 0.0f, 0.0f, 1.0f);
					else
						m_vertices[index].color = D3DXVECTOR4(0.0f, 0.0f, 0.0f, 1.0f);	
					//indices[index] = index;
					index++;

					// Upper right.
					m_vertices[index].position = D3DXVECTOR3(m_heightMap[index4].x, m_heightMap[index4].y, m_heightMap[index4].z);
					m_vertices[index].normal = D3DXVECTOR3(m_heightMap[index4].nx, m_heightMap[index4].ny, m_heightMap[index4].nz);
					if(m_heightMap[index4].y > 0.3)
						m_vertices[index].color = D3DXVECTOR4(0.5f, 0.0f, 0.0f, 1.0f);
					else
						m_vertices[index].color = D3DXVECTOR4(0.0f, 0.0f, 0.0f, 1.0f);	
					//indices[index] = index;
					index++;

					// Bottom left.
					m_vertices[index].position = D3DXVECTOR3(m_heightMap[index1].x, m_heightMap[index1].y, m_heightMap[index1].z);
					m_vertices[index].normal = D3DXVECTOR3(m_heightMap[index1].nx, m_heightMap[index1].ny, m_heightMap[index1].nz);
					if(m_heightMap[index1].y > 0.3)
						m_vertices[index].color = D3DXVECTOR4(0.5f, 0.0f, 0.0f, 1.0f);
					else
						m_vertices[index].color = D3DXVECTOR4(0.0f, 0.0f, 0.0f, 1.0f);		
					//indices[index] = index;
					index++;

					// Bottom left.
					m_vertices[index].position = D3DXVECTOR3(m_heightMap[index1].x, m_heightMap[index1].y, m_heightMap[index1].z);
					m_vertices[index].normal = D3DXVECTOR3(m_heightMap[index1].nx, m_heightMap[index1].ny, m_heightMap[index1].nz);
					if(m_heightMap[index1].y > 0.3)
						m_vertices[index].color = D3DXVECTOR4(0.5f, 0.0f, 0.0f, 1.0f);
					else
						m_vertices[index].color = D3DXVECTOR4(0.0f, 0.0f, 0.0f, 1.0f);	
					//indices[index] = index;
					index++;

					// Upper right.
					m_vertices[index].position = D3DXVECTOR3(m_heightMap[index4].x, m_heightMap[index4].y, m_heightMap[index4].z);
					m_vertices[index].normal = D3DXVECTOR3(m_heightMap[index4].nx, m_heightMap[index4].ny, m_heightMap[index4].nz);
					if(m_heightMap[index4].y > 0.3)
						m_vertices[index].color = D3DXVECTOR4(0.5f, 0.0f, 0.0f, 1.0f);
					else
						m_vertices[index].color = D3DXVECTOR4(0.0f, 0.0f, 0.0f, 1.0f);		
					//indices[index] = index;
					index++;

					// Bottom right.
					m_vertices[index].position = D3DXVECTOR3(m_heightMap[index2].x, m_heightMap[index2].y, m_heightMap[index2].z);
					m_vertices[index].normal = D3DXVECTOR3(m_heightMap[index2].nx, m_heightMap[index2].ny, m_heightMap[index2].nz);
					if(m_heightMap[index2].y > 0.3)
						m_vertices[index].color = D3DXVECTOR4(0.5f, 0.0f, 0.0f, 1.0f);
					else
						m_vertices[index].color = D3DXVECTOR4(0.0f, 0.0f, 0.0f, 1.0f);		
					//indices[index] = index;
					index++;
				}
				else{
					// \ this kind
					// Bottom left.
				m_vertices[index].position = D3DXVECTOR3(m_heightMap[index1].x, m_heightMap[index1].y, m_heightMap[index1].z);
				m_vertices[index].normal = D3DXVECTOR3(m_heightMap[index1].nx, m_heightMap[index1].ny, m_heightMap[index1].nz);
				if(m_heightMap[index1].y > 0.3)
					m_vertices[index].color = D3DXVECTOR4(0.0f, 0.0f, 0.5f, 1.0f);
				else
					m_vertices[index].color = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);	
				//indices[index] = index;
				index++;

				// Upper left.
				m_vertices[index].position = D3DXVECTOR3(m_heightMap[index3].x, m_heightMap[index3].y, m_heightMap[index3].z);
				m_vertices[index].normal = D3DXVECTOR3(m_heightMap[index3].nx, m_heightMap[index3].ny, m_heightMap[index3].nz);
				if(m_heightMap[index3].y > 0.3)
					m_vertices[index].color = D3DXVECTOR4(0.0f, 0.0f, 0.5f, 1.0f);
				else
					m_vertices[index].color = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);	
				index++;

				// Bottom right.
				m_vertices[index].position = D3DXVECTOR3(m_heightMap[index2].x, m_heightMap[index2].y, m_heightMap[index2].z);
				m_vertices[index].normal = D3DXVECTOR3(m_heightMap[index2].nx, m_heightMap[index2].ny, m_heightMap[index2].nz);
				if(m_heightMap[index2].y > 0.3)
					m_vertices[index].color = D3DXVECTOR4(0.0f, 0.0f, 0.5f, 1.0f);
				else
					m_vertices[index].color = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);	
				index++;

				// Bottom right.
				m_vertices[index].position = D3DXVECTOR3(m_heightMap[index2].x, m_heightMap[index2].y, m_heightMap[index2].z);
				m_vertices[index].normal = D3DXVECTOR3(m_heightMap[index2].nx, m_heightMap[index2].ny, m_heightMap[index2].nz);
				if(m_heightMap[index2].y > 0.3)
					m_vertices[index].color = D3DXVECTOR4(0.0f, 0.0f, 0.5f, 1.0f);
				else
					m_vertices[index].color = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);	
				index++;

				// Upper left.
				m_vertices[index].position = D3DXVECTOR3(m_heightMap[index3].x, m_heightMap[index3].y, m_heightMap[index3].z);
				m_vertices[index].normal = D3DXVECTOR3(m_heightMap[index3].nx, m_heightMap[index3].ny, m_heightMap[index3].nz);
				if(m_heightMap[index3].y > 0.3)
					m_vertices[index].color = D3DXVECTOR4(0.0f, 0.0f, 0.5f, 1.0f);
				else
					m_vertices[index].color = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);	
				index++;

				// Upper right.
				m_vertices[index].position = D3DXVECTOR3(m_heightMap[index4].x, m_heightMap[index4].y, m_heightMap[index4].z);
				m_vertices[index].normal = D3DXVECTOR3(m_heightMap[index4].nx, m_heightMap[index4].ny, m_heightMap[index4].nz);
				if(m_heightMap[index4].y > 0.3)
					m_vertices[index].color = D3DXVECTOR4(0.0f, 0.0f, 0.5f, 1.0f);
				else
					m_vertices[index].color = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);	
				index++;
				
				}	
			}
			else{
				if(i%2 == 0){
					// Bottom left.
					m_vertices[index].position = D3DXVECTOR3(m_heightMap[index1].x, m_heightMap[index1].y, m_heightMap[index1].z);
					m_vertices[index].normal = D3DXVECTOR3(m_heightMap[index1].nx, m_heightMap[index1].ny, m_heightMap[index1].nz);
					if(m_heightMap[index1].y > 0.3)
						m_vertices[index].color = D3DXVECTOR4(0.0f, 0.0f, 0.5f, 1.0f);
					else
						m_vertices[index].color = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);	
					index++;

					// Upper left.
					m_vertices[index].position = D3DXVECTOR3(m_heightMap[index3].x, m_heightMap[index3].y, m_heightMap[index3].z);
					m_vertices[index].normal = D3DXVECTOR3(m_heightMap[index3].nx, m_heightMap[index3].ny, m_heightMap[index3].nz);
					if(m_heightMap[index3].y > 0.3)
						m_vertices[index].color = D3DXVECTOR4(0.0f, 0.0f, 0.5f, 1.0f);
					else
						m_vertices[index].color = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);	
					//indices[index] = index;
					index++;

					// Bottom right.
					m_vertices[index].position = D3DXVECTOR3(m_heightMap[index2].x, m_heightMap[index2].y, m_heightMap[index2].z);
					m_vertices[index].normal = D3DXVECTOR3(m_heightMap[index2].nx, m_heightMap[index2].ny, m_heightMap[index2].nz);
					if(m_heightMap[index2].y > 0.3)
						m_vertices[index].color = D3DXVECTOR4(0.0f, 0.0f, 0.5f, 1.0f);
					else
						m_vertices[index].color = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);	
					//indices[index] = index;
					index++;

					// Bottom right.
					m_vertices[index].position = D3DXVECTOR3(m_heightMap[index2].x, m_heightMap[index2].y, m_heightMap[index2].z);
					m_vertices[index].normal = D3DXVECTOR3(m_heightMap[index2].nx, m_heightMap[index2].ny, m_heightMap[index2].nz);
					if(m_heightMap[index2].y > 0.3)
						m_vertices[index].color = D3DXVECTOR4(0.0f, 0.0f, 0.5f, 1.0f);
					else
						m_vertices[index].color = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);	
					//indices[index] = index;
					index++;

					// Upper left.
					m_vertices[index].position = D3DXVECTOR3(m_heightMap[index3].x, m_heightMap[index3].y, m_heightMap[index3].z);
					m_vertices[index].normal = D3DXVECTOR3(m_heightMap[index3].nx, m_heightMap[index3].ny, m_heightMap[index3].nz);
					if(m_heightMap[index3].y > 0.3)
						m_vertices[index].color = D3DXVECTOR4(0.0f, 0.0f, 0.5f, 1.0f);
					else
						m_vertices[index].color = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);	
					//indices[index] = index;
					index++;

					// Upper right.
					m_vertices[index].position = D3DXVECTOR3(m_heightMap[index4].x, m_heightMap[index4].y, m_heightMap[index4].z);
					m_vertices[index].normal = D3DXVECTOR3(m_heightMap[index4].nx, m_heightMap[index4].ny, m_heightMap[index4].nz);
					if(m_heightMap[index4].y > 0.3)
						m_vertices[index].color = D3DXVECTOR4(0.0f, 0.0f, 0.5f, 1.0f);
					else
						m_vertices[index].color = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
					//indices[index] = index;
					index++;
				}
				else{
					// / this kind of geometry
					// Upper left.
					m_vertices[index].position = D3DXVECTOR3(m_heightMap[index3].x, m_heightMap[index3].y, m_heightMap[index3].z);
					m_vertices[index].normal = D3DXVECTOR3(m_heightMap[index3].nx, m_heightMap[index3].ny, m_heightMap[index3].nz);
					if(m_heightMap[index3].y > 0.3)
						m_vertices[index].color = D3DXVECTOR4(0.5f, 0.0f, 0.0f, 1.0f);
					else
						m_vertices[index].color = D3DXVECTOR4(0.0f, 0.0f, 0.0f, 1.0f);	
					//indices[index] = index;
					index++;

					// Upper right.
					m_vertices[index].position = D3DXVECTOR3(m_heightMap[index4].x, m_heightMap[index4].y, m_heightMap[index4].z);
					m_vertices[index].normal = D3DXVECTOR3(m_heightMap[index4].nx, m_heightMap[index4].ny, m_heightMap[index4].nz);
					if(m_heightMap[index4].y > 0.3)
						m_vertices[index].color = D3DXVECTOR4(0.5f, 0.0f, 0.0f, 1.0f);
					else
						m_vertices[index].color = D3DXVECTOR4(0.0f, 0.0f, 0.0f, 1.0f);	
					index++;

					// Bottom left.
					m_vertices[index].position = D3DXVECTOR3(m_heightMap[index1].x, m_heightMap[index1].y, m_heightMap[index1].z);
					m_vertices[index].normal = D3DXVECTOR3(m_heightMap[index1].nx, m_heightMap[index1].ny, m_heightMap[index1].nz);
					if(m_heightMap[index1].y > 0.3)
						m_vertices[index].color = D3DXVECTOR4(0.5f, 0.0f, 0.0f, 1.0f);
					else
						m_vertices[index].color = D3DXVECTOR4(0.0f, 0.0f, 0.0f, 1.0f);	
					index++;

					// Bottom left.
					m_vertices[index].position = D3DXVECTOR3(m_heightMap[index1].x, m_heightMap[index1].y, m_heightMap[index1].z);
					m_vertices[index].normal = D3DXVECTOR3(m_heightMap[index1].nx, m_heightMap[index1].ny, m_heightMap[index1].nz);
					if(m_heightMap[index1].y > 0.3)
						m_vertices[index].color = D3DXVECTOR4(0.5f, 0.0f, 0.0f, 1.0f);
					else
						m_vertices[index].color = D3DXVECTOR4(0.0f, 0.0f, 0.0f, 1.0f);	
					index++;

					// Upper right.
					m_vertices[index].position = D3DXVECTOR3(m_heightMap[index4].x, m_heightMap[index4].y, m_heightMap[index4].z);
					m_vertices[index].normal = D3DXVECTOR3(m_heightMap[index4].nx, m_heightMap[index4].ny, m_heightMap[index4].nz);
					if(m_heightMap[index4].y > 0.3)
						m_vertices[index].color = D3DXVECTOR4(0.5f, 0.0f, 0.0f, 1.0f);
					else
						m_vertices[index].color = D3DXVECTOR4(0.0f, 0.0f, 0.0f, 1.0f);	
					index++;

					// Bottom right.
					m_vertices[index].position = D3DXVECTOR3(m_heightMap[index2].x, m_heightMap[index2].y, m_heightMap[index2].z);
					m_vertices[index].normal = D3DXVECTOR3(m_heightMap[index2].nx, m_heightMap[index2].ny, m_heightMap[index2].nz);
					if(m_heightMap[index2].y > 0.3)
						m_vertices[index].color = D3DXVECTOR4(0.5f, 0.0f, 0.0f, 1.0f);
					else
						m_vertices[index].color = D3DXVECTOR4(0.0f, 0.0f, 0.0f, 1.0f);	
					index++;
				}
			}	
		}
	}

	return true;
}


void TerrainClass::ShutdownBuffers()
{
	// Release the vertex array.
	if(m_vertices)
	{
		delete [] m_vertices;
		m_vertices = 0;
	}

	return;
}

int TerrainClass::GetVertexCount()
{
	return m_vertexCount;
}

void TerrainClass::CopyVertexArray(void* vertexList)
{
	memcpy(vertexList, m_vertices, sizeof(VertexType) * m_vertexCount);
	return;
}


void TerrainClass::RenderBuffers(ID3D11DeviceContext* deviceContext)
{
	unsigned int stride;
	unsigned int offset;


	// Set vertex buffer stride and offset.
	stride = sizeof(VertexType); 
	offset = 0;
    
	// Set the vertex buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

    // Set the index buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

    // Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	return;
}

// Terrain Boundaries
float TerrainClass::getStartX(){
	return 0;
}

float TerrainClass::getEndX(){
	return m_terrainHeight;		// should be a variable, instead of passing it like this
}

float TerrainClass::getStartZ(){
	return 0;
}

float TerrainClass::getEndZ(){
	return m_terrainWidth;
}

float TerrainClass::getBaseY(){
	return 0.0;		// Terrain is at 0, but walking is at 1, so anything above 1, should be blocked I think !! @TODO
}