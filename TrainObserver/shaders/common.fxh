
matrix World			: WORLD;
matrix View				: VIEW;
matrix ViewProjection	: VIEWPROJECTION;

float2 g_farPlane; // fp, 1.f/fp

struct SunLight
{
	float3 dir;
	float3 color;
	float  scale;
	float  power;
};

SunLight	g_sunLight;

#define SAMPLER(map, addressType)\
sampler_state\
{\
	Texture = (map);\
	ADDRESSU = addressType;\
	ADDRESSV = addressType;\
	ADDRESSW = addressType;\
	MAGFILTER = LINEAR;\
	MINFILTER = LINEAR;\
	MIPFILTER = LINEAR;\
	MAXANISOTROPY = 1;\
	MAXMIPLEVEL = 1;\
	MIPMAPLODBIAS = 0;\
};

//float vertexFog(in float4 wPos, in float linearZ)
//{
//	//-- fog enabled if spaceBB is not zero vector.
//	float isBBFogEnabled = all(g_fogParams.m_outerBB);
//
//	//-- calculate the camera's far plane fog. Make sure that is clamped between 0.0f and 1.0f.
//	float outFog = saturate((g_fogParams.m_end - linearZ) / (g_fogParams.m_end - g_fogParams.m_start));
//	
//	//-- calculate space bounds fog. This an optimized vectors form. Make sure we can't try to divide by zero.
//	float4 coeff = max(g_fogParams.m_density * ((wPos.xzxz - g_fogParams.m_innerBB) / ((g_fogParams.m_outerBB - g_fogParams.m_innerBB) + (1.0f - isBBFogEnabled))), 0.0f);
//	float fogBB  = 1.0f - max(max(max(coeff.x, coeff.y), coeff.z), coeff.w);
//		
//	//-- Is spaceBB fog enabled?
//	outFog = min(outFog, fogBB) * isBBFogEnabled + outFog * (1.0f - isBBFogEnabled);
//	
//	return outFog;
//}