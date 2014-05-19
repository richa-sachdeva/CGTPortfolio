// Based on tutorials from http://www.rastertek.com
// For core module AG1102A - Game Programming for PC and Xbox, DirectX Game, University of Abertay, Dundee.
// By Richa Sachdeva, 1304869, M.Sc. Computer Games Technology
////////////////////////////////////////////////////////////////////////////////
// Filename: applicationclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "applicationclass.h"


ApplicationClass::ApplicationClass()
{
	m_Input = 0;
	m_Direct3D = 0;
	m_Camera = 0;
	m_Terrain = 0;
	m_Timer = 0;
	m_Position = 0;
	m_Fps = 0;
	m_Cpu = 0;
	m_Text = 0;
	m_Light = 0;
	m_Sphere = 0;
	m_Frustum = 0;
	m_QuadTree = 0;
	m_ModelList = 0;
	m_pointsCollected = 0;
	m_sphereCollided = false;
	m_Screen = 0;
	m_Bitmap = 0;
	m_Transfer = false;
	m_Collide = 0;
	m_Catch = 0;
	m_RenderTexture = 0;
	m_WallModel[0] = 0;
	m_WallModel[1] = 0;
	m_WallModel[2] = 0;
	m_ShaderManager = 0;
	m_level = 0;
}


ApplicationClass::ApplicationClass(const ApplicationClass& other)
{
}


ApplicationClass::~ApplicationClass()
{
}


bool ApplicationClass::Initialize(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight)
{
	bool result;
	float cameraX, cameraY, cameraZ;
	D3DXMATRIX baseViewMatrix;
	char videoCard[128];
	int videoMemory;

	

	// Store the screen width and height.
	m_screenWidth = screenWidth;
	m_screenHeight = screenHeight;

	// Create the input object.  The input object will be used to handle reading the keyboard and mouse input from the user.
	m_Input = new InputClass;
	if(!m_Input)
	{
		return false;
	}

	// Initialize the input object.
	result = m_Input->Initialize(hinstance, hwnd, screenWidth, screenHeight);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the input object.", L"Error", MB_OK);
		return false;
	}

	// Create the Direct3D object.
	m_Direct3D = new D3DClass;
	if(!m_Direct3D)
	{
		return false;
	}

	// Initialize the Direct3D object.
	result = m_Direct3D->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize DirectX 11.", L"Error", MB_OK);
		return false;
	}

	// Create the camera object.
	m_Camera = new CameraClass;
	if(!m_Camera)
	{
		return false;
	}

	// Initialize a base view matrix with the camera for 2D user interface rendering.
	m_Camera->SetPosition(0.0f, 0.0f, -1.0f);
	m_Camera->Render();
	m_Camera->GetViewMatrix(baseViewMatrix);

	// Set the initial position of the camera.
	cameraX = 70.0f;
	cameraY = 8.0f;
	cameraZ = -7.0f;

	m_Camera->SetPosition(cameraX, cameraY, cameraZ);
	//m_Camera->SetRotation(0.0, 90.0, 0);

	// Create the terrain object.
	m_Terrain = new TerrainClass;
	if(!m_Terrain)
	{
		return false;
	}

	// Initialize the terrain object.
	result = m_Terrain->InitializeTerrain(m_Direct3D->GetDevice(), 512,512);   //initialise the flat terrain.
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the terrain object.", L"Error", MB_OK);
		return false;
	}

	// Create the timer object.
	m_Timer = new TimerClass;
	if(!m_Timer)
	{
		return false;
	}

	// Initialize the timer object.
	result = m_Timer->Initialize();
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the timer object.", L"Error", MB_OK);
		return false;
	}

	// Create the position object.
	m_Position = new PositionClass;
	if(!m_Position)
	{
		return false;
	}

	// Set the initial position of the viewer to the same as the initial camera position.
	m_Position->SetPosition(cameraX, cameraY, cameraZ);

	// Create the fps object.
	m_Fps = new FpsClass;
	if(!m_Fps)
	{
		return false;
	}

	// Initialize the fps object.
	m_Fps->Initialize();

	// Create the cpu object.
	m_Cpu = new CpuClass;
	if(!m_Cpu)
	{
		return false;
	}

	// Initialize the cpu object.
	m_Cpu->Initialize();

	// Create the text object.
	m_Text = new TextClass;
	if(!m_Text)
	{
		return false;
	}

	// Initialize the text object.
	result = m_Text->Initialize(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(), hwnd, screenWidth, screenHeight, baseViewMatrix);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the text object.", L"Error", MB_OK);
		return false;
	}

	// Retrieve the video card information.
	m_Direct3D->GetVideoCardInfo(videoCard, videoMemory);

	//// Set the video card information in the text object.
	//result = m_Text->SetVideoCardInfo(videoCard, videoMemory, m_Direct3D->GetDeviceContext());
	//if(!result)
	//{
	//	MessageBox(hwnd, L"Could not set video card info in the text object.", L"Error", MB_OK);
	//	return false;
	//}

	// Create the light object.
	m_Light = new LightClass;
	if(!m_Light)
	{
		return false;
	}

	// Initialize the light object.
	m_Light->SetAmbientColor(0.5f, 0.5f, 0.5f, 1.0f);
	m_Light->SetDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_Light->SetDirection(1.0f,0.0f, 1.0f);


	// initial position for the player, catch and camera
	D3DXVECTOR3 playerPos = D3DXVECTOR3(m_Terrain->getStartX() + 2, 2, m_Terrain->getEndZ()/2 - 5);  // 512 -> 2, 2, 247

	// Sphere Player Model
	m_Sphere = new ModelClass;
	if(!m_Sphere)
		return false;

	// Initialize the model object.
	result = m_Sphere->Initialize(m_Direct3D->GetDevice(), "../Engine/data/sphere.txt", L"../Engine/data/seafloor.dds");
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}

	m_Sphere->SetPosition(playerPos.x, playerPos.y, playerPos.z);
	m_Position->SetSpherePosition(playerPos.x, playerPos.y, playerPos.z);
	m_Camera->SetPosition(playerPos.x - 30, playerPos.y + 10, playerPos.z);   // x, + 8, -47///later its -30, +10, same
	m_Camera->SetRotation(0, 90, 0);
	m_Position->SetSphereRotation(0, 90, 0);

	// Catch the Sphere model -------------------
	m_Catch = new ModelClass;
	if(!m_Catch)
		return false;

	// Initialize the model object.
	result = m_Catch->Initialize(m_Direct3D->GetDevice(), "../Engine/data/sphere.txt", L"../Engine/data/stripes.dds");
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}

	m_Catch->SetPosition(playerPos.x + 30, 2.5, playerPos.z);

	//m_WallModel[0]->SetPosition(-15.0f, 6.0f, 18.0f);
	float add = 170.0f;
	for (int i=0; i<3; i++)
	{
		// Create the second wall model object.
		m_WallModel[i] = new ModelClass;
		if(!m_WallModel[i])
		{
			return false;
		}

		// Initialize the wall model object.
		result = m_WallModel[i]->Initialize(m_Direct3D->GetDevice(), "../Engine/data/wall.txt", L"../Engine/data/seafloor.dds");
		if(!result)
		{
			MessageBox(hwnd, L"Could not initialize the wall model object.", L"Error", MB_OK);
			return false;
		}
		m_WallModel[i]->SetPosition(m_Terrain->getEndX() - 10, 0.0f, 80 + (add*i));
	}

	// Create the frustum object.
	m_Frustum = new FrustumClass;
	if(!m_Frustum)
	{
		return false;
	}
	// Create the quad tree object.
	m_QuadTree = new QuadTreeClass;
	if(!m_QuadTree)
	{
		return false;
	}

	// Initialize the quad tree object.
	result = m_QuadTree->Initialize(m_Terrain, m_Direct3D->GetDevice());
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the quad tree object.", L"Error", MB_OK);
		return false;
	}

	// Create the model list object.
	m_ModelList = new ModelListClass;
	if(!m_ModelList)
	{
		return false;
	}

	// Initialize the model list object.
	result = m_ModelList->Initialize(32);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the model list object.", L"Error", MB_OK);
		return false;
	}

	// ------------------------------------  Bitmap objects --> Screens -> Init/Exit  starts ------------
	// Create the bitmap object.
	m_Bitmap = new BitmapClass;
	if(!m_Bitmap)
	{
		return false;
	}

	// Initialize the bitmap object.
	result = m_Bitmap->Initialize(m_Direct3D->GetDevice(), screenWidth, screenHeight, L"../Engine/data/startScreen.jpg", screenWidth, screenHeight);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the bitmap object.", L"Error", MB_OK);
		return false;
	}

	// ------------------------------------  Bitmap objects --> Screens -> Init/Exit  ends ------------

	// ------------------------------------ Post Processing Starts ------------------------------------
	// Create the render to texture object.
	m_RenderTexture = new RenderTextureClass;
	if(!m_RenderTexture)
	{
		return false;
	}

	// Initialize the render to texture object.
	result = m_RenderTexture->Initialize(m_Direct3D->GetDevice(), screenWidth, screenHeight, SCREEN_DEPTH, SCREEN_NEAR);
	if(!result)
	{
		return false;
	}

	///////////////////////////////////// Post Processing Ends ----------------------------------------

	// Create the shader manager object.
	m_ShaderManager = new ShaderManagerClass;
	if(!m_ShaderManager)
	{
		return false;
	}

	// Initialize the shader manager object.
	result = m_ShaderManager->Initialize(m_Direct3D->GetDevice(), hwnd);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the shader manager object.", L"Error", MB_OK);
		return false;
	}
	return true;
}


void ApplicationClass::Shutdown()
{
	// Release the shader manager object.
	if(m_ShaderManager)
	{
		m_ShaderManager->Shutdown();
		delete m_ShaderManager;
		m_ShaderManager = 0;
	}
	// Release the second wall model object.
	if(m_WallModel[2])
	{
		m_WallModel[2]->Shutdown();
		delete m_WallModel[2];
		m_WallModel[2] = 0;
	}

	// Release the second wall model object.
	if(m_WallModel[1])
	{
		m_WallModel[1]->Shutdown();
		delete m_WallModel[1];
		m_WallModel[1] = 0;
	}

	// Release the wall model object.
	if(m_WallModel[0])
	{
		m_WallModel[0]->Shutdown();
		delete m_WallModel[0];
		m_WallModel[0] = 0;
	}

	// Release the render to texture object.
	if(m_RenderTexture)
	{
		m_RenderTexture->Shutdown();
		delete m_RenderTexture;
		m_RenderTexture = 0;
	}

	// Release the catch object.
	if(m_Catch)
	{
		m_Catch->Shutdown();
		delete m_Catch;
		m_Catch = 0;
	}
	
	// Release the bitmap object.
	if(m_Bitmap)
	{
		m_Bitmap->Shutdown();
		delete m_Bitmap;
		m_Bitmap = 0;
	}
	// Release the model list object.
	if(m_ModelList)
	{
		m_ModelList->Shutdown();
		delete m_ModelList;
		m_ModelList = 0;
	}
	// Release the quad tree object.
	if(m_QuadTree)
	{
		m_QuadTree->Shutdown();
		delete m_QuadTree;
		m_QuadTree = 0;
	}

	// Release the frustum object.
	if(m_Frustum)
	{
		delete m_Frustum;
		m_Frustum = 0;
	}

	// Release the model object.
	if(m_Sphere)
	{
		m_Sphere->Shutdown();
		delete m_Sphere;
		m_Sphere = 0;
	}
	// Release the light object.
	if(m_Light)
	{
		delete m_Light;
		m_Light = 0;
	}

	// Release the text object.
	if(m_Text)
	{
		m_Text->Shutdown();
		delete m_Text;
		m_Text = 0;
	}

	// Release the cpu object.
	if(m_Cpu)
	{
		m_Cpu->Shutdown();
		delete m_Cpu;
		m_Cpu = 0;
	}

	// Release the fps object.
	if(m_Fps)
	{
		delete m_Fps;
		m_Fps = 0;
	}

	// Release the position object.
	if(m_Position)
	{
		delete m_Position;
		m_Position = 0;
	}

	// Release the timer object.
	if(m_Timer)
	{
		delete m_Timer;
		m_Timer = 0;
	}

	// Release the terrain object.
	if(m_Terrain)
	{
		m_Terrain->Shutdown();
		delete m_Terrain;
		m_Terrain = 0;
	}

	// Release the camera object.
	if(m_Camera)
	{
		delete m_Camera;
		m_Camera = 0;
	}

	// Release the Direct3D object.
	if(m_Direct3D)
	{
		m_Direct3D->Shutdown();
		delete m_Direct3D;
		m_Direct3D = 0;
	}

	// Release the input object.
	if(m_Input)
	{
		m_Input->Shutdown();
		delete m_Input;
		m_Input = 0;
	}

	return;
}


bool ApplicationClass::Frame()
{
	bool result, foundHeight;
	D3DXVECTOR3 position;
	float height;
	D3DXMATRIX worldMatrix, viewMatrix, orthoMatrix;


	if(m_Transfer)
		m_Screen++;

	result = m_Input->Frame();
	if(!result)
	{
		return false;
	}
	
	// Check if the user pressed escape and wants to exit the application.
	if(m_Input->IsEscapePressed() == true)
	{
		return false;
	}

	// Update the system stats.
	m_Timer->Frame();
	m_Fps->Frame();
	m_Cpu->Frame();

	// Update the FPS value in the text object.
	result = m_Text->SetFps(m_Fps->GetFps(), m_Direct3D->GetDeviceContext());
	if(!result)
	{
		return false;
	}
	
	// Update the CPU usage value in the text object.
	/*result = m_Text->SetCpu(m_Cpu->GetCpuPercentage(), m_Direct3D->GetDeviceContext());
	if(!result)
	{
		return false;
	}*/

	// Do the frame input processing.
	result = HandleInput(m_Timer->GetTime());
	if(!result)
	{
		return false;
	}

	position = m_Sphere->GetPosition();

	// Get the height of the triangle that is directly underneath the given camera position.
	foundHeight =  m_QuadTree->GetHeightAtPosition(position.x, position.z, height);
	if(foundHeight)
	{
		// If there was a triangle under the camera then position the camera just above it by two units.
		m_Sphere->SetPosition(position.x, height + 1.5f, position.z);
	}

	int modelCount = m_ModelList->GetModelCount();
	for(int index=0; index<modelCount; index++)
	{
		m_ModelList->MoveModelX(index, m_Terrain->getStartZ(), m_Terrain->getEndZ());

		m_sphereCollided = RaySphereIntersect(m_ModelList->GetModelPosition(index), m_Sphere->GetPosition(), 1.0);
		if(m_sphereCollided){
		//	m_Text->SetIntersection(true, m_Direct3D->GetDeviceContext());
			m_Collide++;
			m_Text->SetCollision(m_Collide, m_Direct3D->GetDeviceContext());
			m_pointsCollected--;
			m_Text->SetPointsCollected(m_pointsCollected, m_Direct3D->GetDeviceContext());
			continue;
		}
		else{
			//m_Text->SetIntersection(false, m_Direct3D->GetDeviceContext());

		}
	}

	m_Direct3D->GetWorldMatrix(worldMatrix);
	m_Direct3D->GetOrthoMatrix(orthoMatrix);
	m_Camera->GetViewMatrix(viewMatrix);

	if(m_Screen == 0){
		// Render the 1st screen
		m_Direct3D->TurnZBufferOff();
		// Put the bitmap vertex and index buffers on the graphics pipeline to prepare them for drawing.
		result = m_Bitmap->Render(m_Direct3D->GetDeviceContext(), 0, 0);
		if(!result)
		{
			return false;
		}
		// Render the bitmap with the texture shader.
		result = m_ShaderManager->RenderTextureShader(m_Direct3D->GetDeviceContext(), m_Bitmap->GetIndexCount(), worldMatrix, viewMatrix, orthoMatrix, m_Bitmap->GetTexture());
		if(!result)
		{
			return false;
		}

		// Turn the Z buffer back on now that all 2D rendering has completed.
		m_Direct3D->TurnZBufferOn();
		// Present the rendered scene to the screen.
		m_Direct3D->EndScene();

	}
	//else if(m_Collide > 5){
	//	// Render the 1st screen
	//	m_Direct3D->TurnZBufferOff();
	//	// Put the bitmap vertex and index buffers on the graphics pipeline to prepare them for drawing.
	//	result = m_Bitmap->Render(m_Direct3D->GetDeviceContext(), 0, 0);
	//	if(!result)
	//	{
	//		return false;
	//	}
	//	// Render the bitmap with the texture shader.
	//	result = m_TextureShader->Render(m_Direct3D->GetDeviceContext(), m_Bitmap->GetIndexCount(), worldMatrix, viewMatrix, orthoMatrix, m_Bitmap->GetTexture());
	//	if(!result)
	//	{
	//		return false;
	//	}

	//	// Turn the Z buffer back on now that all 2D rendering has completed.
	//	m_Direct3D->TurnZBufferOn();
	//	// Present the rendered scene to the screen.
	//	m_Direct3D->EndScene();
	//} 
	else{
		// Render to Texture, so that m_RenderTexture is filled and can be used with Vertical blur shader
		result = RenderToTexture();
		if(!result)
			return false;

		// Render the graphics.
		result = RenderGraphics();
		if(!result)
		{
			return false;
		}
	}
	return result;
}


bool ApplicationClass::HandleInput(float frameTime)
{
	bool keyDown, result;
	float posX, posY, posZ, rotX, rotY, rotZ;
	float sphX, sphY, sphZ;

	keyDown = false;
	// Set the frame time for calculating the updated position.
	m_Position->SetFrameTime(frameTime);


	keyDown = m_Input->IsLeftPressed();
	m_Position->TurnSphereLeft(keyDown);

	keyDown = m_Input->IsRightPressed();
	m_Position->TurnSphereRight(keyDown);

	keyDown = m_Input->IsUpPressed();
	m_Position->MoveSphereForward(keyDown);

	keyDown = m_Input->IsDownPressed();
	m_Position->MoveSphereBackward(keyDown);

	/*keyDown = m_Input->IsAPressed();
	m_Position->RotateSphereLeft(keyDown);

	keyDown = m_Input->IsDPressed();
	m_Position->RotateSphereRight(keyDown);

	m_Position->GetSphereRotation(rotX, rotY, rotZ);
	m_Camera->SetRotation(rotX, rotY, rotZ);*/
	//m_Camera->SetRotation(rotX, rotY, rotZ);  -> camera's initial rotation is 0, 90, 0
	
	/*m_Position->GetSphereRotation(rotX, rotY, rotZ);
	m_Camera->SetRotation(rotX, rotY, rotZ);*/

	m_Position->GetSpherePosition(sphX, sphY, sphZ);
	m_Sphere->SetMovingPosition(sphX, sphY, sphZ, m_Terrain->getStartZ(), m_Terrain->getEndZ());
	//m_Sphere->SetPosition(sphX, sphY, sphZ);

	m_Camera->SetPosition(sphX - 30, sphY + 10, sphZ);

	// Test the Player and Catch interaction
	result = RaySphereIntersect(m_Sphere->GetPosition(), m_Catch->GetPosition(), 1.0);
	static float c = 0;
	if(result == true)
	{
		m_pointsCollected++;
		
		m_Text->SetPointsCollected(m_pointsCollected, m_Direct3D->GetDeviceContext());
		float height = 0;
		float randomX, randomZ;
		bool foundHt = false;

		int x = m_Terrain->getEndX();
		int z = m_Terrain->getEndZ();
		while(!foundHt){
			randomX = 30 + ( std::rand() % ( x - 0 + 1 ) );
			randomZ = 30 + ( std::rand() % ( z - 0 + 1 ) );
			if(m_QuadTree->GetHeightAtPosition(randomX, randomZ, height)){
				if(height == 2.0){
					foundHt = true;
				}
				else
					break;
			}
		}
		m_Catch->SetPosition(randomX, 2.8, randomZ);
		
	}
		// Check for screens
	if(m_Input->IsSpacePressed())
		m_Transfer = true;

	// Handle the input.
	keyDown = m_Input->IsLPressed();
	if(keyDown){
		m_level++;
		if(m_level < 3){
			int sent = (int)pow(2.0, m_level);
   			m_Terrain->DoubleTerrain(m_Direct3D->GetDevice(), sent);		// c = 1, 2, 4, 8
			m_QuadTree->Initialize(m_Terrain, m_Direct3D->GetDevice());
			
			m_Sphere->SetPosition(m_Terrain->getEndX() - 100, 2, m_Terrain->getEndZ() - 100);
			m_Sphere->SetMovingPosition(m_Terrain->getEndX() - 100, 2, m_Terrain->getEndZ() - 100, m_Terrain->getStartZ(), m_Terrain->getEndZ());
			D3DXVECTOR3 p = m_Sphere->GetPosition();
			m_Catch->SetPosition(p.x + 20, 2.5, p.z );

			// set the walls
			int offset = 100;
			for(int i = 0; i<3; i++){
				p = m_WallModel[i]->GetPosition();
				m_WallModel[i]->SetPosition(m_Terrain->getEndX() - 15, p.y, 65 + (i*offset));
			}
		}
	}

	return true;
}


bool ApplicationClass::RenderGraphics()
{
	D3DXMATRIX worldMatrix, viewMatrix, projectionMatrix, orthoMatrix;
	bool renderModel, result;
	D3DXVECTOR3 cameraPosition;
	int modelCount, renderCount, index;
	float positionX, positionY, positionZ, radius;
	D3DXVECTOR4 color;
	D3DXVECTOR3 pos;

	// Clear the scene.
	m_Direct3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	// Generate the view matrix based on the camera's position.
	m_Camera->Render();

	// Get the world, view, projection, and ortho matrices from the camera and Direct3D objects.
	m_Direct3D->GetWorldMatrix(worldMatrix);
	m_Camera->GetViewMatrix(viewMatrix);
	m_Direct3D->GetProjectionMatrix(projectionMatrix);
	m_Direct3D->GetOrthoMatrix(orthoMatrix);

	// Construct the frustum.
	// Terrain is rendered at the origin
	m_Frustum->ConstructFrustum(SCREEN_DEPTH, projectionMatrix, viewMatrix);
	
	// Set the terrain shader parameters that it will use for rendering.
	result = m_ShaderManager->RenderTerrainShader(m_Direct3D->GetDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, m_Light->GetAmbientColor(), 
						      m_Light->GetDiffuseColor(), m_Light->GetDirection());// m_Terrain->GetTexture());
	if(!result)
	{
		return false;
	}

	// Render the terrain using the quad tree and terrain shader.
	m_QuadTree->Render(m_Frustum, m_Direct3D->GetDeviceContext(),m_ShaderManager->GetTerrainShaderObject());
	
	//////////////--------------------------------- Render the Player starts -------------------------------
	m_Direct3D->GetWorldMatrix(worldMatrix);
	pos = m_Sphere->GetPosition();
	D3DXMatrixTranslation(&worldMatrix, pos.x, pos.y, pos.z);

	// Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.
	m_Sphere->Render(m_Direct3D->GetDeviceContext());

	// Render the model using the color shader.
	result = m_ShaderManager->RenderColorShader(m_Direct3D->GetDeviceContext(), m_Sphere->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix);
	if(!result)
	{
		return false;
	}
	// Rendering of Player ends ---------------------------------------------------------

	// Rendering of three walls starts ----------------------------------------------------
	int width = m_Terrain->getEndX();
	int render;
	if(width == 512)
		render = 3;
	else if ( width == 256)
		render = 2;
	else if(width == 128)
		render = 1;
	for(int i=0; i<render; i++)
	{
		// Reset the world matrix.
		m_Direct3D->GetWorldMatrix(worldMatrix);
		// Translate to where the wall model will be rendered.
		pos = m_WallModel[i]->GetPosition();
		D3DXMatrixTranslation(&worldMatrix, pos.x, pos.y, pos.z);

		// Put the wall model vertex and index buffers on the graphics pipeline to prepare them for drawing.
		m_WallModel[i]->Render(m_Direct3D->GetDeviceContext());

		// Render the wall model using the light shader.
		result = m_ShaderManager->RenderVerticalShader(m_Direct3D->GetDeviceContext(), m_WallModel[i]->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix, 
		m_RenderTexture->GetShaderResourceView(), m_RenderTexture->GetTextureHeight()/2, m_RenderTexture->GetTextureWidth()/2);
		if(!result)
		{
			return false;
		}
	}
	// Rendering of three walls ends ----------------------------------------------------------------------

	// Rendering the Catch Sphere ------------------------------------------------------------------------
	m_Direct3D->GetWorldMatrix(worldMatrix);
	pos = m_Catch->GetPosition();
	D3DXMatrixTranslation(&worldMatrix, pos.x, pos.y, pos.z);

	// Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.
	m_Catch->Render(m_Direct3D->GetDeviceContext());

	// Render the model using the light shader.
	result = m_ShaderManager->RenderTextureShader(m_Direct3D->GetDeviceContext(), m_Catch->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix, m_Catch->GetTexture());
		//RenderVerticalShader(m_Direct3D->GetDeviceContext(), m_Catch->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix, 
		//m_RenderTexture->GetShaderResourceView(), m_RenderTexture->GetTextureHeight()/2, m_RenderTexture->GetTextureWidth()/2);
	if(!result)
	{
		return false;
	}
	// Rendering ends -------------------------------------------------------------------

	// Rendering all the sphere ----------------------------------------------
	m_Direct3D->GetWorldMatrix(worldMatrix);
	// Get the number of models that will be rendered.
	modelCount = m_ModelList->GetModelCount();
	// Initialize the count of models that have been rendered.
	renderCount = 0;
	// Go through all the models and render them only if they can be seen by the camera view.
	if(render == 2)
		modelCount = 20;
	else if (render == 1)
		modelCount = 12;
	
	for(index=0; index<modelCount; index++)
	{
		// Get the position and color of the sphere model at this index.
		m_ModelList->GetData(index, positionX, positionY, positionZ, color);
		// Set the radius of the sphere to 1.0 since this is already known.
		radius = 1.0f;
		// Move the model to the location it should be rendered at.
		D3DXMatrixTranslation(&worldMatrix, positionX, positionY, positionZ); 
		// Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.
		m_Sphere->Render(m_Direct3D->GetDeviceContext());
		// Render the model using the light shader.
		
		m_ShaderManager->RenderLightShader(m_Direct3D->GetDeviceContext(), m_Sphere->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix, 
					    m_Sphere->GetTexture(), D3DXVECTOR3(1.0, -0.50, -0.50), color);
		// Reset to the original world matrix.
		m_Direct3D->GetWorldMatrix(worldMatrix);
		// Since this model was rendered then increase the count for this frame.
		renderCount++;
	}

	// -----------------------------------------------------------------------
	// Set the number of rendered terrain triangles since some were culled.
	/*result = m_Text->SetRenderCount(m_QuadTree->GetDrawCount(), m_Direct3D->GetDeviceContext());
	if(!result)
	{
		return false;
	}*/

	// Turn off the Z buffer to begin all 2D rendering.
	m_Direct3D->TurnZBufferOff();
		
	// Turn on the alpha blending before rendering the text.
	m_Direct3D->TurnOnAlphaBlending();

	// Render the text user interface elements.
	result = m_Text->Render(m_Direct3D->GetDeviceContext(), m_ShaderManager->GetFontShaderObject(), worldMatrix, orthoMatrix);
	if(!result)
	{
		return false;
	}

	// Turn off alpha blending after rendering the text.
	m_Direct3D->TurnOffAlphaBlending();

	// Turn the Z buffer back on now that all 2D rendering has completed.
	m_Direct3D->TurnZBufferOn();

	// Present the rendered scene to the screen.
	m_Direct3D->EndScene();

	return true;
}

bool ApplicationClass::RenderToTexture()
{
	D3DXMATRIX worldMatrix, viewMatrix, projectionMatrix, n1;
	bool result;
	static float rotation = 0.0f;

	//m_Camera->SetRotation(0,0,0);
	// Set the render target to be the render to texture.
	m_RenderTexture->SetRenderTarget(m_Direct3D->GetDeviceContext());

	// Clear the render to texture.
	m_RenderTexture->ClearRenderTarget(m_Direct3D->GetDeviceContext(), 0.5f, 0.0f, 1.0f, 1.0f);

	// Generate the view matrix based on the camera's position.
	m_Camera->Render();

	// Get the world, view, and projection matrices from the camera and d3d objects.
	m_Direct3D->GetWorldMatrix(worldMatrix);
	m_Camera->GetViewMatrix(viewMatrix);
	m_Direct3D->GetProjectionMatrix(projectionMatrix);

	// Update the rotation variable each frame.
	rotation += (float)D3DX_PI * 0.005f;
	if(rotation > 360.0f)
	{
		rotation -= 360.0f;
	}

	m_Direct3D->GetWorldMatrix(worldMatrix);
	//D3DXMatrixRotationZ(&worldMatrix, rotation);
	// Construct the frustum.
	m_Frustum->ConstructFrustum(SCREEN_DEPTH, projectionMatrix, viewMatrix);
	
	// Set the terrain shader parameters that it will use for rendering.
	result = m_ShaderManager->RenderTerrainShader(m_Direct3D->GetDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, m_Light->GetAmbientColor(), 
						      m_Light->GetDiffuseColor(), m_Light->GetDirection());// m_Terrain->GetTexture());
	if(!result)
	{
		return false;
	}

	// Render the terrain using the quad tree and terrain shader.
	m_QuadTree->Render(m_Frustum, m_Direct3D->GetDeviceContext(), m_ShaderManager->GetTerrainShaderObject());
	if(!result)
	{
		return false;
	}

	// Reset the render target back to the original back buffer and not the render to texture anymore.
	m_Direct3D->SetBackBufferRenderTarget();

	return true;
}

bool ApplicationClass::RaySphereIntersect(D3DXVECTOR3 rayOrigin, D3DXVECTOR3 rayDirection, float radius)
{
	//float a, b, c, discriminant;
	D3DXVECTOR3 distance = rayOrigin - rayDirection;
	
	if( (distance.x * distance.x + distance.z * distance.z) <= (radius + radius)*(radius * radius)){
		//m_Text->SetIntersection(true, m_Direct3D->GetDeviceContext());
		return true;
	}

	return false;
}