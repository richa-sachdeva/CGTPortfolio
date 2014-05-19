// Based on tutorials from http://www.rastertek.com
// For core module AG1102A - Game Programming for PC and Xbox, DirectX Game, University of Abertay, Dundee.
// By Richa Sachdeva, 1304869, M.Sc. Computer Games Technology
////////////////////////////////////////////////////////////////////////////////
// Filename: verticalblur.vs
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

cbuffer ScreenSizeBuffer
{
	float screenHeight;
	float screenWidth;
	float2 padding;
};


//////////////
// TYPEDEFS //
//////////////
struct VertexInputType
{
    float4 position : POSITION;
    float2 tex : TEXCOORD0;
};

struct PixelInputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
	float2 texCoord1 : TEXCOORD1;
	float2 texCoord2 : TEXCOORD2;
	float2 texCoord3 : TEXCOORD3;
	float2 texCoord4 : TEXCOORD4;
	float2 texCoord5 : TEXCOORD5;
	float2 texCoord6 : TEXCOORD6;
	float2 texCoord7 : TEXCOORD7;
	float2 texCoord8 : TEXCOORD8;
	float2 texCoord9 : TEXCOORD9;
};


////////////////////////////////////////////////////////////////////////////////
// Vertex Shader
////////////////////////////////////////////////////////////////////////////////
PixelInputType VerticalBlurVertexShader(VertexInputType input)
{
    PixelInputType output;
	float texelSize, texelSize2;


	// Change the position vector to be 4 units for proper matrix calculations.
    input.position.w = 1.0f;

	// Calculate the position of the vertex against the world, view, and projection matrices.
    output.position = mul(input.position, worldMatrix);
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);
    
	// Store the texture coordinates for the pixel shader.
	output.tex = input.tex;
    
	// Determine the floating point size of a texel for a screen with this specific height.
	texelSize = 1.0f / screenHeight;

	texelSize2 = 1.0f / screenWidth;

	// Create UV coordinates for the pixel and its four vertical neighbors on either side.
	output.texCoord1 = input.tex + float2(0.0f, texelSize * -4.0f) + float2(0.0f, texelSize2 * -4.0f);
	output.texCoord2 = input.tex + float2(0.0f, texelSize * -3.0f) + float2(0.0f, texelSize2 * -3.0f);
	output.texCoord3 = input.tex + float2(0.0f, texelSize * -2.0f) + float2(0.0f, texelSize2 * -2.0f);
	output.texCoord4 = input.tex + float2(0.0f, texelSize * -1.0f) + float2(0.0f, texelSize2 * -1.0f);
	output.texCoord5 = input.tex + float2(0.0f, texelSize *  0.0f) + float2(0.0f, texelSize2 * 0.0f);
	output.texCoord6 = input.tex + float2(0.0f, texelSize *  1.0f) + float2(0.0f, texelSize2 * 1.0f);
	output.texCoord7 = input.tex + float2(0.0f, texelSize *  2.0f) + float2(0.0f, texelSize2 * 2.0f);
	output.texCoord8 = input.tex + float2(0.0f, texelSize *  3.0f) + float2(0.0f, texelSize2 * 3.0f);
	output.texCoord9 = input.tex + float2(0.0f, texelSize *  4.0f) + float2(0.0f, texelSize2 * 4.0f);

    return output;
}