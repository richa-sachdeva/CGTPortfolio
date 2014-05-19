// Based on tutorials from http://www.rastertek.com
// For core module AG1102A - Game Programming for PC and Xbox, DirectX Game, University of Abertay, Dundee.
// By Richa Sachdeva, 1304869, M.Sc. Computer Games Technology
////////////////////////////////////////////////////////////////////////////////
// Filename: shadermanagerclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _SHADERMANAGERCLASS_H_
#define _SHADERMANAGERCLASS_H_


///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "d3dclass.h"

#include "fontshaderclass.h"
#include "terrainshaderclass.h"

#include "textureshaderclass.h"
#include "lightshaderclass.h"

#include "horizontalblurshaderclass.h"
#include "verticalblurshaderclass.h"

#include "colorshaderclass.h"


////////////////////////////////////////////////////////////////////////////////
// Class name: ShaderManagerClass
////////////////////////////////////////////////////////////////////////////////
class ShaderManagerClass
{
public:
	ShaderManagerClass();
	ShaderManagerClass(const ShaderManagerClass&);
	~ShaderManagerClass();

	bool Initialize(ID3D11Device*, HWND);
	void Shutdown();

	bool RenderFontShader(ID3D11DeviceContext*, int, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX, ID3D11ShaderResourceView*, D3DXVECTOR4);

	bool RenderTerrainShader(ID3D11DeviceContext*, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX, D3DXVECTOR4, D3DXVECTOR4, D3DXVECTOR3);

	bool RenderTextureShader(ID3D11DeviceContext*, int, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX, ID3D11ShaderResourceView*);

	bool RenderLightShader(ID3D11DeviceContext*, int, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX, ID3D11ShaderResourceView*, D3DXVECTOR3, D3DXVECTOR4);

	bool RenderVerticalShader(ID3D11DeviceContext*, int, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX, ID3D11ShaderResourceView*, float, float);

	bool RenderColorShader(ID3D11DeviceContext*, int, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX);

	TerrainShaderClass* GetTerrainShaderObject();

	FontShaderClass* GetFontShaderObject();

private:
	FontShaderClass* m_FontShader;
	TerrainShaderClass* m_TerrainShader;
	TextureShaderClass* m_TextureShader;
	LightShaderClass* m_LightShader;
//	HorizontalBlurShaderClass* m_HorizontalBlurShader;
	VerticalBlurShaderClass* m_VerticalBlurShader;
	ColorShaderClass* m_ColorShader;
};

#endif