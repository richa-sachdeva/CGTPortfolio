// Based on tutorials from http://www.rastertek.com
// For core module AG1101A - Programming for Games, DirectX Coursework, University of Abertay, Dundee.
// By Richa Sachdeva, 1304869, M.Sc. Computer Games Technology
////////////////////////////////////////////////////////////////////////////////
// Filename: graphicsclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "graphicsclass.h"


GraphicsClass::GraphicsClass()
{
	m_D3D = 0;
	m_Camera = 0;
	m_GroundModel = 0;
	m_WallModel[0] = 0;
	m_SphereModel = 0;
	m_Light = 0;
	m_LightShader = 0;
	m_Text = 0;
	m_TranslateShader = 0;
	m_SphereModelList = 0;
	m_SkyDome = 0;
	m_WallModel[1] = 0;
	m_WallModel[2] = 0;
	m_RenderTexture = 0;
	m_ReflectionShader = 0;
	m_ParticleShader = 0;
	m_ParticleSystem = 0;
	m_Bitmap = 0;
	m_Screen = 0;
	m_TextureShader = 0;
}


GraphicsClass::GraphicsClass(const GraphicsClass& other)
{
}


GraphicsClass::~GraphicsClass()
{
}


bool GraphicsClass::Initialize(int screenWidth, int screenHeight, HWND hwnd)
{
	bool result;
	D3DXMATRIX baseViewMatrix;
	float cameraX, cameraY, cameraZ, rotX, rotY, rotZ;
		
	// Create the Direct3D object.
	m_D3D = new D3DClass;
	if(!m_D3D)
	{
		return false;
	}

	// Initialize the Direct3D object.
	result = m_D3D->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize Direct3D.", L"Error", MB_OK);
		return false;
	}

	// Create the camera object.
	m_Camera = new CameraClass;
	if(!m_Camera)
	{
		return false;
	}
	// Initialize a base view matrix with the camera for 2D user interface rendering.
	m_Camera->SetPosition(0.0f, 0.0f, -10.0f);
	m_Camera->Render();
	m_Camera->GetViewMatrix(baseViewMatrix);
	// Set the initial position of the camera.
	cameraX = -5.0f;
	cameraY = 8.0f;
	cameraZ = -40.0f;
	rotX = 0.0f;
	rotY = 0.0f;
	rotZ = 0.0f;
	// Set the position and rotation of the camera.
	m_Camera->SetPosition(cameraX, cameraY, cameraZ);
	m_Camera->SetRotation(rotX, rotY, rotZ);

	// Create the ground model object.
	m_GroundModel = new ModelClass;
	if(!m_GroundModel)
	{
		return false;
	}

	// Initialize the ground model object.
	result = m_GroundModel->Initialize(m_D3D->GetDevice(), L"data/Checkered_Floor.dds", "data/ground.txt");
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the ground model object.", L"Error", MB_OK);
		return false;
	}

	m_GroundModel->SetPosition(0.0f, 1.0f, 0.0f);

	//m_WallModel[0]->SetPosition(-15.0f, 6.0f, 18.0f);
	float add = 15.0f;
	for (int i=0; i<3; i++)
	{
		// Create the second wall model object.
		m_WallModel[i] = new ModelClass;
		if(!m_WallModel[i])
		{
			return false;
		}

		// Initialize the wall model object.
		result = m_WallModel[i]->Initialize(m_D3D->GetDevice(), L"data/Checkered_Floor.dds", "data/wall.txt");
		if(!result)
		{
			MessageBox(hwnd, L"Could not initialize the wall model object.", L"Error", MB_OK);
			return false;
		}
		m_WallModel[i]->SetPosition(-15.0f + (add*i), 6.0f, 18.0f);
	}

	// Create the wall model object.
	m_SphereModel = new ModelClass;
	if(!m_SphereModel)
	{
		return false;
	}

	// Initialize the sphere model object.
	result = m_SphereModel->Initialize(m_D3D->GetDevice(), L"data/Checkered_Floor.dds", "data/sphere.txt");
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the sphere model object.", L"Error", MB_OK);
		return false;
	}
	m_SphereModel->SetPosition(-5.0, 10.0, -12.0);

	// Create the model list object.
	m_SphereModelList = new SphereModelListClass;
	if(!m_SphereModelList)
	{
		return false;
	}

	// Initialize the model list object.
	result = m_SphereModelList->Initialize(25);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the sphere model list object.", L"Error", MB_OK);
		return false;
	}

	// Create the light object.
	m_Light = new LightClass;
	if(!m_Light)
	{
		return false;
	}

	// Initialize the light object.
	m_Light->SetAmbientColor(0.15f, 0.15f, 0.15f, 1.0f);
	m_Light->SetDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_Light->SetDirection(0.0f, -1.0f, 0.5f);

	// Create the light shader object.
	m_LightShader = new LightShaderClass;
	if(!m_LightShader)
	{
		return false;
	}

	// Initialize the light shader object.
	result = m_LightShader->Initialize(m_D3D->GetDevice(), hwnd);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the light shader object.", L"Error", MB_OK);
		return false;
	}

	// Create the text object.
	m_Text = new TextClass;
	if(!m_Text)
	{
		return false;
	}

	// Initialize the text object.
	result = m_Text->Initialize(m_D3D->GetDevice(), m_D3D->GetDeviceContext(), hwnd, screenWidth, screenHeight, baseViewMatrix);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the text object.", L"Error", MB_OK);
		return false;
	}

	// Create the texture translation shader object.
	m_TranslateShader = new TranslateShaderClass;
	if(!m_TranslateShader)
	{
		return false;
	}

	// Initialize the texture translation shader object.
	result = m_TranslateShader->Initialize(m_D3D->GetDevice(), hwnd);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the texture translation shader object.", L"Error", MB_OK);
		return false;
	}

	// Create the sky dome object.
	m_SkyDome = new SkyDomeClass;
	if(!m_SkyDome)
	{
		return false;
	}

	// Initialize the sky dome object.
	result = m_SkyDome->Initialize(m_D3D->GetDevice());
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the sky dome object.", L"Error", MB_OK);
		return false;
	}

	// Create the render to texture object.
	m_RenderTexture = new RenderTextureClass;
	if(!m_RenderTexture)
	{
		return false;
	}

	// Initialize the render to texture object.
	result = m_RenderTexture->Initialize(m_D3D->GetDevice(), screenWidth, screenHeight);
	if(!result)
	{
		return false;
	}

	// Create the reflection shader object.
	m_ReflectionShader = new ReflectionShaderClass;
	if(!m_ReflectionShader)
	{
		return false;
	}

	// Initialize the reflection shader object.
	result = m_ReflectionShader->Initialize(m_D3D->GetDevice(), hwnd);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the reflection shader object.", L"Error", MB_OK);
		return false;
	}

	// Create the particle shader object.
	m_ParticleShader = new ParticleShaderClass;
	if(!m_ParticleShader)
	{
		return false;
	}

	// Initialize the particle shader object.
	result = m_ParticleShader->Initialize(m_D3D->GetDevice(), hwnd);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the particle shader object.", L"Error", MB_OK);
		return false;
	}
	// Create the particle system object.
	m_ParticleSystem = new ParticleSystemClass;
	if(!m_ParticleSystem)
	{
		return false;
	}

	// Initialize the particle system object.
	result = m_ParticleSystem->Initialize(m_D3D->GetDevice(), L"data/snow.dds");
	if(!result)
	{
		return false;
	}

	// Create the bitmap object.
	m_Bitmap = new BitmapClass;
	if(!m_Bitmap)
	{
		return false;
	}

	// Initialize the bitmap object.
	result = m_Bitmap->Initialize(m_D3D->GetDevice(), screenWidth, screenHeight, L"data/startScreen.png", screenWidth, screenHeight);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the bitmap object.", L"Error", MB_OK);
		return false;
	}

	// Create the texture shader object.
	m_TextureShader = new TextureShaderClass;
	if(!m_TextureShader)
	{
		return false;
	}

	// Initialize the texture shader object.
	result = m_TextureShader->Initialize(m_D3D->GetDevice(), hwnd);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the texture shader object.", L"Error", MB_OK);
		return false;
	}

	return true;
}


void GraphicsClass::Shutdown()
{
	// Release the texture shader object.
	if(m_TextureShader)
	{
		m_TextureShader->Shutdown();
		delete m_TextureShader;
		m_TextureShader = 0;
	}
	// Release the bitmap object.
	if(m_Bitmap)
	{
		m_Bitmap->Shutdown();
		delete m_Bitmap;
		m_Bitmap = 0;
	}
	// Release the particle system object.
	if(m_ParticleSystem)
	{
		m_ParticleSystem->Shutdown();
		delete m_ParticleSystem;
		m_ParticleSystem = 0;
	}

	// Release the particle shader object.
	if(m_ParticleShader)
	{
		m_ParticleShader->Shutdown();
		delete m_ParticleShader;
		m_ParticleShader = 0;
	}
	// Release the reflection shader object.
	if(m_ReflectionShader)
	{
		m_ReflectionShader->Shutdown();
		delete m_ReflectionShader;
		m_ReflectionShader = 0;
	}
	// Release the render to texture object.
	if(m_RenderTexture)
	{
		m_RenderTexture->Shutdown();
		delete m_RenderTexture;
		m_RenderTexture = 0;
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

	// Release the sky dome object.
	if(m_SkyDome)
	{
		m_SkyDome->Shutdown();
		delete m_SkyDome;
		m_SkyDome = 0;
	}
	// Release the model list object.
	if(m_SphereModelList)
	{
		m_SphereModelList->Shutdown();
		delete m_SphereModelList;
		m_SphereModelList = 0;
	}
	// Release the texture translation shader object.
	if(m_TranslateShader)
	{
		m_TranslateShader->Shutdown();
		delete m_TranslateShader;
		m_TranslateShader = 0;
	}
	// Release the wall model object.
	if(m_SphereModel)
	{
		m_SphereModel->Shutdown();
		delete m_SphereModel;
		m_SphereModel = 0;
	}
	// Release the text object.
	if(m_Text)
	{
		m_Text->Shutdown();
		delete m_Text;
		m_Text = 0;
	}
	// Release the light shader object.
	if(m_LightShader)
	{
		m_LightShader->Shutdown();
		delete m_LightShader;
		m_LightShader = 0;
	}
	// Release the light object.
	if(m_Light)
	{
		delete m_Light;
		m_Light = 0;
	}

	// Release the wall model object.
	if(m_WallModel[0])
	{
		m_WallModel[0]->Shutdown();
		delete m_WallModel[0];
		m_WallModel[0] = 0;
	}

	// Release the ground model object.
	if(m_GroundModel)
	{
		m_GroundModel->Shutdown();
		delete m_GroundModel;
		m_GroundModel = 0;
	}

	// Release the camera object.
	if(m_Camera)
	{
		delete m_Camera;
		m_Camera = 0;
	}

	// Release the D3D object.
	if(m_D3D)
	{
		m_D3D->Shutdown();
		delete m_D3D;
		m_D3D = 0;
	}

	return;
}


bool GraphicsClass::Frame(int mouseX, int mouseY, int fps, int cpu, float frameTime,
	D3DXVECTOR3 camera_rotation, D3DXVECTOR3 camera_position, bool transfer)
{
	bool result;

	if(transfer)
		m_Screen++;

	// Render the graphics scene.
	result = Render(mouseX, mouseY, camera_position);
	if(!result)
	{
		return false;
	}
	
	// Run the frame processing for the particle system.
	m_ParticleSystem->Frame(frameTime, m_D3D->GetDeviceContext());

	// Set the frames per second.
	result = m_Text->SetFps(fps, m_D3D->GetDeviceContext());
	if(!result)
	{
		return false;
	}

	// Set the cpu usage.
	result = m_Text->SetCpu(cpu, m_D3D->GetDeviceContext());
	if(!result)
	{
		return false;
	}
	// Set the position of the camera. Initial position is -5, 8, -40
	m_Camera->SetPosition(camera_position.x - 5.0f, camera_position.y + 8.0f, camera_position.z-40.0f);

	//// Set the rotation of the camera.
	m_Camera->SetRotation(camera_rotation.x, camera_rotation.y, camera_rotation.z);


	return true;
}


bool GraphicsClass::Render(int mouseX, int mouseY, D3DXVECTOR3 camera_position)
{
	bool result;

	// Render the entire scene as a reflection to the texture first.
	result = RenderToTexture();
	if(!result)
	{
		return false;
	}

	// Render the scene as normal to the back buffer.
	result = RenderScene();
	if(!result)
	{
		return false;
	}

	return true;
}

// Render to Texture function
bool GraphicsClass::RenderToTexture()
{
	D3DXMATRIX worldMatrix, reflectionViewMatrix, projectionMatrix;
	static float textureTranslation = 0.0f;
	float posX, posY, posZ, radius;
	int sphereModelCount, renderCount, index;
	D3DXVECTOR4 color;

	// Increment the texture translation position.
	textureTranslation += 0.01f;
	if(textureTranslation > 1.0f)
	{
		textureTranslation -= 1.0f;
	}

	// Set the render target to be the render to texture.
	m_RenderTexture->SetRenderTarget(m_D3D->GetDeviceContext(), m_D3D->GetDepthStencilView());
	// Clear the render to texture.
	m_RenderTexture->ClearRenderTarget(m_D3D->GetDeviceContext(), m_D3D->GetDepthStencilView(), 0.0f, 0.0f, 0.0f, 1.0f);
	// Use the camera to calculate the reflection matrix.
	m_Camera->RenderReflection(-1.0f); // was -1.0 
	// Get the camera reflection view matrix instead of the normal view matrix.
	reflectionViewMatrix = m_Camera->GetReflectionViewMatrix();

	// List of sphere model rendering begins
	m_D3D->GetWorldMatrix(worldMatrix);
	m_D3D->GetProjectionMatrix(projectionMatrix);
	// Get the number of models that will be rendered.
	sphereModelCount = m_SphereModelList->GetModelCount();

	// Initialize the count of models that have been rendered.
	renderCount = 0;

	// Go through all the models and render them only if they can be seen by the camera view.
	for(index=0; index<sphereModelCount; index++)
	{
		// Reset to the original world matrix.
		m_D3D->GetWorldMatrix(worldMatrix);
		// Get the position and color of the sphere model at this index.
		m_SphereModelList->GetData(index, posX, posY, posZ, color);		
		// Set the radius of the sphere to 1.0 since this is already known.
		radius = 1.0f;	//(((float)rand()-(float)rand())/RAND_MAX) * 4.0f;//1.0f;
		// Move the model to the location it should be rendered at.
		D3DXMatrixTranslation(&worldMatrix, posX, 0.0, posZ); 
		// Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.
		m_SphereModel->Render(m_D3D->GetDeviceContext());
		// Render the model using the light shader.
		m_TranslateShader->Render(m_D3D->GetDeviceContext(), m_SphereModel->GetIndexCount(), worldMatrix, reflectionViewMatrix, 
					   projectionMatrix, m_SphereModel->GetTexture(), textureTranslation);
		
		// Since this model was rendered then increase the count for this frame.
		renderCount++;
	}
	
	m_D3D->SetBackBufferRenderTarget();

	return true;
}

bool GraphicsClass::RenderScene()
{
	D3DXMATRIX worldMatrix, viewMatrix, projectionMatrix, orthoMatrix, reflectionMatrix, translateMatrix;
	bool result;
	float posX, posY, posZ, radius;
	static float textureTranslation = 0.0f;
	int sphereModelCount, renderCount, index;
	D3DXVECTOR4 color, currPos;
	D3DXVECTOR3 cameraPosition, modelPosition;
	double angle;
	float rotation;

	// Increment the texture translation position.
	textureTranslation += 0.005f;
	if(textureTranslation > 1.0f)
	{
		textureTranslation -= 1.0f;
	}

	// Clear the buffers to begin the scene.
	m_D3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	// Generate the view matrix based on the camera's position.
	m_Camera->Render();

	// Get the world, view, and projection matrices from the camera and d3d objects.
	m_D3D->GetWorldMatrix(worldMatrix);
	m_Camera->GetViewMatrix(viewMatrix);
	m_D3D->GetProjectionMatrix(projectionMatrix);
	m_D3D->GetOrthoMatrix(orthoMatrix);

	// Get the position of the camera.
	cameraPosition = m_Camera->GetPosition();

	// Translate the sky dome to be centered around the camera position.
	D3DXMatrixTranslation(&worldMatrix, cameraPosition.x, cameraPosition.y, cameraPosition.z);

	// Turn off back face culling.
	m_D3D->TurnOffCulling();

	// Turn off the Z buffer.
	m_D3D->TurnZBufferOff();
	// Render the sky dome using the sky dome shader.
	m_SkyDome->Render(m_D3D->GetDeviceContext());
	m_TextureShader->Render(m_D3D->GetDeviceContext(), m_SkyDome->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix, 
		m_SkyDome->GetTexture());
	// Turn back face culling back on.
	m_D3D->TurnOnCulling();

	// Turn the Z buffer back on.
	m_D3D->TurnZBufferOn();

	// Rendering of 3 walls
	for(int i=0; i<3; i++)
	{
		// Reset the world matrix.
		m_D3D->GetWorldMatrix(worldMatrix);
		// Translate to where the wall model will be rendered.
		m_WallModel[i]->GetPosition(posX, posY, posZ);
		D3DXMatrixTranslation(&worldMatrix, posX, posY, posZ);

		// Put the wall model vertex and index buffers on the graphics pipeline to prepare them for drawing.
		m_WallModel[i]->Render(m_D3D->GetDeviceContext());

		// Render the wall model using the light shader.
		result = m_LightShader->Render(m_D3D->GetDeviceContext(), m_WallModel[i]->GetIndexCount(), worldMatrix, viewMatrix, 
									   projectionMatrix, m_WallModel[i]->GetTexture(), m_Light->GetDirection(), 
									   m_Light->GetAmbientColor(), m_Light->GetDiffuseColor());
		if(!result)
		{
			return false;
		}
	}
	// Rendering of 3 walls end

	// List of sphere model rendering begins
	
	m_D3D->GetWorldMatrix(worldMatrix);
	// Get the number of models that will be rendered.
	sphereModelCount = m_SphereModelList->GetModelCount();
	// Initialize the count of models that have been rendered.
	renderCount = 0;

	// Go through all the models and render them only if they can be seen by the camera view.
	for(index=0; index<sphereModelCount; index++)
	{
		// Get the position and color of the sphere model at this index.
		m_SphereModelList->GetData(index, posX, posY, posZ, color);		
		// Set the radius of the sphere to 1.0 since this is already known.
		radius = 1.0f;	//(((float)rand()-(float)rand())/RAND_MAX) * 4.0f;//1.0f;
		// Move the model to the location it should be rendered at.
		D3DXMatrixTranslation(&worldMatrix, posX, posY, posZ); 
		// Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.
		m_SphereModel->Render(m_D3D->GetDeviceContext());
		// Render the model using the light shader.
		m_TranslateShader->Render(m_D3D->GetDeviceContext(), m_SphereModel->GetIndexCount(), worldMatrix, viewMatrix, 
					   projectionMatrix, m_SphereModel->GetTexture(), textureTranslation);

		// Reset to the original world matrix.
		m_D3D->GetWorldMatrix(worldMatrix);
		// Since this model was rendered then increase the count for this frame.
		renderCount++;
	}
	// rendering of spheres ended

	// Get the world matrix again and translate down for the floor model to render underneath the cube.
	m_D3D->GetWorldMatrix(worldMatrix);

	m_GroundModel->GetPosition(posX, posY, posZ);
	// Translate to where the ground model will be rendered.
	D3DXMatrixTranslation(&worldMatrix, posX, posY, posZ);

	// Get the camera reflection view matrix.
	reflectionMatrix = m_Camera->GetReflectionViewMatrix();

	// Put the floor model vertex and index buffers on the graphics pipeline to prepare them for drawing.
	m_GroundModel->Render(m_D3D->GetDeviceContext());

	// Render the floor model using the reflection shader, reflection texture, and reflection view matrix.
	result = m_ReflectionShader->Render(m_D3D->GetDeviceContext(), m_GroundModel->GetIndexCount(), worldMatrix, viewMatrix,
										projectionMatrix, m_GroundModel->GetTexture(), m_RenderTexture->GetShaderResourceView(), 
										reflectionMatrix);

	// setting up billboarding for particle effect
	// Get the position of the camera.
	cameraPosition = m_Camera->GetPosition();

	// Set the position of the billboard model.
	modelPosition.x =  0.0f;
	modelPosition.y =  0.0f;
	modelPosition.z =  0.0f;
	
	// Calculate the rotation that needs to be applied to the billboard model to face the current camera position using the arc tangent function.
	angle = atan2(modelPosition.x - cameraPosition.x, modelPosition.z - cameraPosition.z) * (180.0 / D3DX_PI);

	// Convert rotation into radians.
	rotation = (float)angle * 0.0174532925f;
	
	// Setup the rotation the billboard at the origin using the world matrix.
	D3DXMatrixRotationY(&worldMatrix, rotation);

	// Setup the translation matrix from the billboard model.
	D3DXMatrixTranslation(&translateMatrix, modelPosition.x, modelPosition.y, modelPosition.z);

	// Finally combine the rotation and translation matrices to create the final world matrix for the billboard model.
	D3DXMatrixMultiply(&worldMatrix, &worldMatrix, &translateMatrix); 
	
	// Particle effect
	//m_D3D->GetWorldMatrix(worldMatrix);
	// Turn on alpha blending.
	m_D3D->TurnOnAlphaBlending();
	// Put the particle system vertex and index buffers on the graphics pipeline to prepare them for drawing.
	m_ParticleSystem->Render(m_D3D->GetDeviceContext());

	// Render the model using the texture shader.
	result = m_ParticleShader->Render(m_D3D->GetDeviceContext(), m_ParticleSystem->GetVertexCount(), m_ParticleSystem->GetInstanceCount(), worldMatrix, viewMatrix, projectionMatrix, 
					  m_ParticleSystem->GetTexture());
	if(!result)
	{
		return false;
	}

	// Turn off alpha blending.
	//m_D3D->TurnOffAlphaBlending();

	m_D3D->GetWorldMatrix(worldMatrix);
	// Text rendering
	// Turn off the Z buffer to begin all 2D rendering.
	m_D3D->TurnZBufferOff();
	// Turn on the alpha blending before rendering the text.
	//m_D3D->TurnOnAlphaBlending();

	// Render the text strings.
	result = m_Text->Render(m_D3D->GetDeviceContext(), worldMatrix, orthoMatrix);
	if(!result)
	{
		return false;
	}


	// render the 1st control screen
	m_D3D->GetOrthoMatrix(orthoMatrix);
	
	if(m_Screen == 0){
		// Put the bitmap vertex and index buffers on the graphics pipeline to prepare them for drawing.
		result = m_Bitmap->Render(m_D3D->GetDeviceContext(), 0, 0);
		if(!result)
		{
			return false;
		}
		// Render the bitmap with the texture shader.
		result = m_TextureShader->Render(m_D3D->GetDeviceContext(), m_Bitmap->GetIndexCount(), worldMatrix, viewMatrix, orthoMatrix, m_Bitmap->GetTexture());
		if(!result)
		{
			return false;
		}
	}
	// Turn off alpha blending after rendering the text.
	m_D3D->TurnOffAlphaBlending();

	// Turn the Z buffer back on now that all 2D rendering has completed.
	m_D3D->TurnZBufferOn();

	// Present the rendered scene to the screen.
	m_D3D->EndScene();

	return true;
}