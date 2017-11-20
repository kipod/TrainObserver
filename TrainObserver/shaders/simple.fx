///////////////////////////////////////////////////////////////////////////////
// Variables / Shader Constants
///////////////////////////////////////////////////////////////////////////////
// Transformation Matrices

#include "common.fxh"

float waterHeight = 0.0f;

const vector DiffuseLightIntensity = {0.9f, 0.9f, 0.8f, 1.0f};
const vector AmbientLightIntensity = {0.3f, 0.3f, 0.3f, 1.0f};


// Textures
texture diffuseTex;
texture normalTex;

sampler diffuseSml				= SAMPLER(diffuseTex, CLAMP)
sampler normalSml				= SAMPLER(normalTex, CLAMP)


struct VS_INPUT
{
	float4 position : POSITION;
	float4 normal   : NORMAL;
	float2 tc		: TEXCOORD0;
	float4 tangent	: TEXCOORD1;
	float4 binormal	: TEXCOORD2;
};

struct PS_INPUT
{
	float4 position : POSITION;
	half3 normal   : NORMAL;
	float4 diffuse	: COLOR;
	float2 tc		: TEXCOORD0;
	half3 tangent	: TEXCOORD1;
	half3 binormal	: TEXCOORD2;
};

struct PS_OUT
{
	float4 color	: COLOR0;
	//float4 z		: COLOR1;
};

PS_INPUT vs(VS_INPUT i)
{
     PS_INPUT o = (PS_INPUT)0;
     
     o.tc = i.tc;
	
     matrix wvp = mul(World, ViewProjection);
     o.position = mul(i.position, wvp);
	 o.normal = mul(i.normal.xyz, World);
	 o.tangent = mul(i.tangent.xyz, World);
	 o.binormal = mul(i.binormal.xyz, World);
     return o;
}

PS_OUT ps(PS_INPUT i)
{
	PS_OUT o = (PS_OUT)0;
		
	float4 diffuse = tex2D(diffuseSml, i.tc);
	
	float4 nn = mul(tex2D(normalSml, i.tc), World);
	half3x3 TBN = half3x3(i.tangent, i.binormal, i.normal);
	half3  normal = normalize(mul(nn.xyz, TBN));

	float s = saturate(dot(-g_sunLight.dir, normal.xyz)) + 0.3f;
	diffuse *= s;
	o.color.xyz = saturate(diffuse);
	o.color.w = 1;
	return o;
}

/////////////////////////////////////////////////////////////////////////////////////////
// Techniques
/////////////////////////////////////////////////////////////////////////////////////////
Technique Default_VS_1_1
{
    pass p0
    {
		FillMode			= solid;
		//ZEnable				= false;
//		ZWriteEnable		= true;
//		ZFUNC				= LESSEQUAL;
		//COLORWRITEENABLE	= 0xFFFFFFFF;
		
		VertexShader = compile vs_3_0 vs();
		PixelShader = compile ps_3_0 ps();
 	}
}