// Based on tutorials from http://www.rastertek.com
// For core module AG1101A - Programming for Games, DirectX Coursework, University of Abertay, Dundee.
// By Richa Sachdeva, 1304869, M.Sc. Computer Games Technology
////////////////////////////////////////////////////////////////////////////////
// Filename: graphicsclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _GRAPHICSCLASS_H_
#define _GRAPHICSCLASS_H_

/////////////
// GLOBALS //
/////////////
const bool FULL_SCREEN = true;
const bool VSYNC_ENABLED = false;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;

///////////////////////
// MY CLASS INCLUDES //
///////////////////////

#include "d3dclass.h"
#include "cameraclass.h"
#include "modelclass.h"
#include "lightclass.h"
#include "rendertextureclass.h"
#include "lightshaderclass.h"
#include "textclass.h"
#include "translateshaderclass.h"
#include "spheremodellistclass.h"
#include "skydomeclass.h"
#include "reflectionshaderclass.h"
#include "particleshaderclass.h"
#include "particlesystemclass.h"
#include "bitmapclass.h"
#include "textureshaderclass.h"
///////////////////////////////////////////////////////////////////////////////
// Class name: GraphicsClass
////////////////////////////////////////////////////////////////////////////////

class GraphicsClass
{
public:
	GraphicsClass();
	GraphicsClass(const GraphicsClass&);
	~GraphicsClass();

	bool Initialize(int, int, HWND);

	void Shutdown();
	// Frame-by-frame graphics processing
	bool Frame(int mouseX, int mouseY, int fps, int cpu, float frameTime, 
		D3DXVECTOR3 camera_rotation, D3DXVECTOR3 camera_position, bool transfer);

private:
	// Calls render functions in order
	bool Render(int mouseX, int mouseY, D3DXVECTOR3 camera_position);

	bool RenderScene();

	bool RenderToTexture();

private:
	D3DClass* m_D3D;
	CameraClass* m_Camera;
	ModelClass *m_GroundModel, *m_WallModel[3], *m_SphereModel;
	LightClass* m_Light;
	LightShaderClass* m_LightShader;

	TextClass* m_Text;
	TranslateShaderClass* m_TranslateShader;
	
	SphereModelListClass* m_SphereModelList;
	
	SkyDomeClass* m_SkyDome;

	RenderTextureClass* m_RenderTexture;
	ReflectionShaderClass* m_ReflectionShader;

	ParticleShaderClass* m_ParticleShader;
	ParticleSystemClass* m_ParticleSystem;

	BitmapClass* m_Bitmap;
	TextureShaderClass* m_TextureShader;
	// to keep count of screens
	int m_Screen;
};

#endif