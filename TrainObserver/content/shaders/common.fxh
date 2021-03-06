
matrix World			: WORLD;
matrix View				: VIEW;
matrix ViewProjection	: VIEWPROJECTION;

float2 g_farPlane; // fp, 1.f/fp

// Textures
texture diffuseTex;
texture normalTex;

#define SAMPLER(map, addressType)\
sampler_state\
{\
	Texture = (map);\
	ADDRESSU = addressType;\
	ADDRESSV = addressType;\
	ADDRESSW = addressType;\
	MAGFILTER = ANISOTROPIC;\
	MINFILTER = ANISOTROPIC;\
	MIPFILTER = ANISOTROPIC;\
	MAXANISOTROPY = 1;\
	MAXMIPLEVEL = 1;\
	MIPMAPLODBIAS = 0;\
};

sampler diffuseSml = SAMPLER(diffuseTex, WRAP);
sampler normalSml = SAMPLER(normalTex, WRAP);

struct SunLight
{
	float3 dir;
	float3 color;
	float  scale;
	float  power;
};

SunLight	g_sunLight;
float3		g_cameraPos = { 0,0,0 };

////////////////////////////////////

struct VS_INPUT
{
	float4 position : POSITION;
	float4 normal   : NORMAL;
	float2 tc		: TEXCOORD0;
#ifdef NORMALMAPPING
	float4 tangent	: TEXCOORD1;
	float4 binormal	: TEXCOORD2;
#endif
};

struct PS_INPUT
{
	float4 position : POSITION;
	float3 normal   : NORMAL;
	float4 diffuse	: COLOR;
	float2 tc		: TEXCOORD0;
	float3 worldPos	: TEXCOORD1;
#ifdef NORMALMAPPING
	float3 tangent	: TEXCOORD2;
	float3 binormal	: TEXCOORD3;
#endif
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
	o.worldPos = mul(i.position, World).xyz;
	o.normal = normalize(mul(i.normal.xyz, (float3x3)World));
#ifdef NORMALMAPPING
	o.tangent = normalize(mul(i.tangent.xyz, World));
	o.binormal = normalize(mul(i.binormal.xyz, World));
#endif
	return o;
}


half3 getSunLight(half3 eye, half3 normal)
{
	half3 halfAngle = normalize(eye - g_sunLight.dir);
	half specular = g_sunLight.scale * pow(saturate(dot(normal, halfAngle)), g_sunLight.power);
	return (specular * g_sunLight.color);
}

PS_OUT ps(PS_INPUT i)
{
	PS_OUT o = (PS_OUT)0;

	float4 diffuse = tex2D(diffuseSml, i.tc);

#ifdef NORMALMAPPING
	float4 nn = mul(tex2D(normalSml, i.tc), World);
	float3x3 TBN = half3x3(i.tangent, i.binormal, i.normal);
	float3  normal = normalize(mul(nn.xyz, TBN));
#else
	float3  normal = normalize(i.normal.xyz);
#endif
	float ndotl = dot(-g_sunLight.dir, normal);
	float s = saturate(ndotl) + 0.3f;
	diffuse *= s;


	o.color.xyz = saturate(diffuse);

	if (ndotl > 0)
	{
		float3 eye = normalize(g_cameraPos.xyz - i.worldPos);
		o.color.xyz += getSunLight(eye, normal) * 0.4f;//float3(i.tc, 0);
	}

	o.color.w = diffuse.w;
	return o;
}

/////////////////////////////////////////////////////////////////////////////////////////
// Techniques
/////////////////////////////////////////////////////////////////////////////////////////
Technique Default_VS_1_1
{
	pass p0
	{
		FillMode = solid;
		ZEnable = true;
		ZWriteEnable = true;
		ZFUNC = LESSEQUAL;
		//COLORWRITEENABLE	= 0xFFFFFFFF;

		VertexShader = compile vs_3_0 vs();
		PixelShader = compile ps_3_0 ps();
	}
}