/*=============================================================================
	SimpleElementPixelShader.hlsl: Pixel shader for drawing simple elements.
	Copyright 1998-2007 Epic Games, Inc. All Rights Reserved.
=============================================================================*/

#include "Common.usf"

sampler2D Texture;

float4 TextureComponentReplicate;

void ReplicateRChannel(inout float4 BaseColor)
{
#if SM4_PROFILE
	//In DX10, doing a texture lookup from a G8 texture gives float4(value,0,0,0), so we need to replicate it to the other channels.  
	//On all other platforms, the value is replicated automatically.
	if(any(TextureComponentReplicate))
	{
		BaseColor = dot(BaseColor,TextureComponentReplicate);
	}
#endif
}

void Main(
	in float2 TextureCoordinate : TEXCOORD0,
	in float4 Color : TEXCOORD1,
	out float4 OutColor : COLOR0
	)
{
	float4 BaseColor = tex2D(Texture,TextureCoordinate);
	ReplicateRChannel(BaseColor);
	OutColor = RETURN_COLOR(BaseColor * Color);	
}

float Gamma;

void GammaMain(
	in float2 TextureCoordinate : TEXCOORD0,
	in float4 Color : TEXCOORD1,
	out float4 OutColor : COLOR0
	)
{
	float4 BaseColor = tex2D(Texture,TextureCoordinate);
	ReplicateRChannel(BaseColor);
	OutColor = BaseColor * Color;
	if( Gamma != 1.0 )
	{
		// Gamma correct the output color.
		OutColor.rgb = pow(saturate(OutColor.rgb),Gamma);	
	}
	OutColor = RETURN_COLOR(OutColor);
}

float ClipRef;

void GammaMaskedMain(
	in float2 TextureCoordinate : TEXCOORD0,
	in float4 Color : TEXCOORD1,
	out float4 OutColor : COLOR0
	)
{
	float4 BaseColor = tex2D(Texture,TextureCoordinate);
	clip(BaseColor.a - ClipRef);
	ReplicateRChannel(BaseColor);
	OutColor = BaseColor * Color;
	if( Gamma != 1.0 )
	{
		// Gamma correct the output color.
		OutColor.rgb = pow(saturate(OutColor.rgb),Gamma);	
	}
	OutColor = RETURN_COLOR(OutColor);
}
