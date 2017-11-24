
matrix World;//: WORLD;
matrix ViewProjection;//: VIEWPROJECTION;

// Textures
texture bbTex;

sampler diffuseSml =
sampler_state
{
	Texture = bbTex;
	MAGFILTER = ANISOTROPIC; 
	MINFILTER = ANISOTROPIC; 
	MIPFILTER = ANISOTROPIC; 
	AddressU = Clamp;
	AddressV = Clamp;
};

struct VS_INPUT
{
	float4 pos		: POSITION;
	float2 tc		: TEXCOORD;
};

struct PS_INPUT
{
	float4 pos		: POSITION;
	float2 tc		: TEXCOORD0;
};

PS_INPUT vs(VS_INPUT i)
{
	// ќбнул€ем все члены экземпл€ра выходной структуры
	PS_INPUT o = (PS_INPUT)0;

	o.tc = i.tc;

	o.pos = float4(i.pos.xy, 0, 1);
	o.tc = i.tc;
	return o;
}

float4 ps(PS_INPUT i) : COLOR
{
	float4 diffuse = tex2D(diffuseSml, i.tc);
	return diffuse;
}

/////////////////////////////////////////////////////////////////////////////////////////
// Techniques
/////////////////////////////////////////////////////////////////////////////////////////
Technique Default_VS_1_1
{
	pass p0
	{
		FillMode = solid;
		ZEnable = false;
		ZWriteEnable = false;

		VertexShader = compile vs_3_0 vs();
		PixelShader = compile ps_3_0 ps();

	}
}