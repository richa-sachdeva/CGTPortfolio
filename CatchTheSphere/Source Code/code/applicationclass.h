// Based on tutorials from http://www.rastertek.com
// For core module AG1102A - Game Programming for PC and Xbox, DirectX Game, University of Abertay, Dundee.
// By Richa Sachdeva, 1304869, M.Sc. Computer Games Technology
////////////////////////////////////////////////////////////////////////////////
// Filename: applicationclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _APPLICATIONCLASS_H_
#define _APPLICATIONCLASS_H_


/////////////
// GLOBALS //
/////////////
const bool FULL_SCREEN = true;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;


///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "inputclass.h"
#include "d3dclass.h"
#include "cameraclass.h"
#include "terrainclass.h"
#include "timerclass.h"
#include "positionclass.h"
#include "fpsclass.h"
#include "cpuclass.h"

#include "textclass.h"

#include "lightclass.h"

#include "modelclass.h"

#include "frustumclass.h"
#include "quadtreeclass.h"

#include "modellistclass.h"

#include "bitmapclass.h"

#include "rendertextureclass.h"

#include "shadermanagerclass.h"


////////////////////////////////////////////////////////////////////////////////
// Class name: ApplicationClass
////////////////////////////////////////////////////////////////////////////////
class ApplicationClass
{
public:
	ApplicationClass();
	ApplicationClass(const ApplicationClass&);
	~ApplicationClass();

	bool Initialize(HINSTANCE, HWND, int, int);
	void Shutdown();
	bool Frame();

private:
	bool HandleInput(float);
	bool RenderGraphics();
	bool RenderToTexture();
	bool RenderHorizontalBlurToTexture();

	// for collision detection
	bool RaySphereIntersect(D3DXVECTOR3, D3DXVECTOR3, float);

private:
	InputClass* m_Input;
	D3DClass* m_Direct3D;
	CameraClass* m_Camera;
	TerrainClass* m_Terrain;
	TimerClass* m_Timer;
	PositionClass* m_Position;
	FpsClass* m_Fps;
	CpuClass* m_Cpu;
	
	TextClass* m_Text;
	LightClass* m_Light;

	ModelClass* m_Sphere, *m_Catch, *m_WallModel[3];

	FrustumClass* m_Frustum;
	QuadTreeClass* m_QuadTree;

	ModelListClass* m_ModelList;

	int m_pointsCollected;
	bool m_sphereCollided;
	int m_screenWidth, m_screenHeight;

	int m_Screen, m_Collide; // To keep count of Screens  // = 1 -> Intro, then game, and when = 3 then exit screen
	BitmapClass* m_Bitmap;
	bool m_Transfer;

	// Post Processing variables
	RenderTextureClass* m_RenderTexture;

	ShaderManagerClass* m_ShaderManager;

	int m_level;
};

#endif