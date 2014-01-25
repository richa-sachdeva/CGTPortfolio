// Based on tutorials from http://www.rastertek.com
// For core module AG1101A - Programming for Games, DirectX Coursework, University of Abertay, Dundee.
// By Richa Sachdeva, 1304869, M.Sc. Computer Games Technology
////////////////////////////////////////////////////////////////////////////////
// Filename: skydome.vs
////////////////////////////////////////////////////////////////////////////////


/////////////
// GLOBALS //
/////////////
cbuffer MatrixBuffer
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
};


//////////////
// TYPEDEFS //
//////////////
struct VertexInputType
{
    float4 position : POSITION;
//	float2 tex : TEXCOORD0;
};

struct PixelInputType
{
    float4 position : SV_POSITION;
	float4 domePosition : TEXCOORD0;
//	float2 tex : TEXCOORD0;
};


////////////////////////////////////////////////////////////////////////////////
// Vertex Shader
////////////////////////////////////////////////////////////////////////////////
PixelInputType SkyDomeVertexShader(VertexInputType input)
{
    PixelInputType output;
    

	// Change the position vector to be 4 units for proper matrix calculations.
    input.position.w = 1.0f;

	// Calculate the position of the vertex against the world, view, and projection matrices.
    output.position = mul(input.position, worldMatrix);
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);
    
	// Send the unmodified position through to the pixel shader.
	output.domePosition = input.position;

	 // Store the texture coordinates for the pixel shader.
   // output.tex = input.tex;

    return output;
}