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

sampler diffuseSml				= SAMPLER(diffuseTex, WRAP)
sampler normalSml				= SAMPLER(normalTex, WRAP)


struct VS_INPUT
{
	float4 position : POSITION;
	//float4 normal   : NORMAL;
	float2 tc		: TEXCOORD;
};

struct PS_INPUT
{
	float4 position : POSITION;
	float4 diffuse	: COLOR;
	float2 tc		: TEXCOORD0;
	float height	: TEXCOORD1;
	float z			: TEXCOORD2;		
};

struct PS_OUT
{
	float4 color	: COLOR0;
	float4 z		: COLOR1;
};

PS_INPUT vs(VS_INPUT i)
{
     // �������� ��� ����� ���������� �������� ���������
     PS_INPUT o = (PS_INPUT)0;
     
     o.tc = i.tc;
	
     //
     // ����������� �������������� ������� � ���������� ������������
     // ��������� � ��������� ��� � ����� output.position
     //
     matrix wvp = mul(World, ViewProjection);
     o.position = mul(i.position, wvp);
     o.height = mul(i.position, World).y;
	 o.z = o.position.z * g_farPlane.y;

	 float4 AmbientMtrl = float4(0.8,1,0.8,1);
	 vector DiffuseMtrl = float4(1,1,1,1);

     //
     // ����������� ������ ��������� � ������� � ������������ ����.
     // ����������� ���������� w �������� 0, ��������� �� �����������
     // �������, � �� �����.
     //
     //float4 normal = 0;
     //normal     = mul(i.normal,   World);

     //
     // ��������� ������� ���� ����� �������� ����� � ��������
     //
     //float s = dot(-g_sunLight.dir, normal);

     //
     // ���������, ��� ���� ���� ����� �������� �����������
     // � �������� ��������� ������ 90 ��������, �����������
     // �� �������� �����. �������������, ���� ���� ������
     // 90 ��������, �� ����������� s ����, ������� ��� �����,
     // ��� ����������� �� ��������.
     //
     //if(s < 0.0f)
     //     s = 0.0f;

     //
     // ���������� ������� ���� ����������� ����� ���������������
     // ��������� ������� ������� ������������ ��������� � �������
     // ������������� �������� �����.
     //
     // ���������� ������������ ���� ����������� ����� ���������������
     // ��������� ������� ������������ ������������ ��������� �� ������
     // ������������� ������������� �����. ����� �� ������������ ����������
     // ������, ������� ������ ��� ���������� �� ����������� ��������� s,
     // ����� ��������� ���� � ����������� �� ����, ������� ����� ��������
     // ������� �� ���������.
     //
     // ����� ������� � ������������ ��������� ���� ���
     // �������� ���� �������.
     //
     o.diffuse = (AmbientMtrl * AmbientLightIntensity);// +
                         // (s * (DiffuseLightIntensity * DiffuseMtrl));

     return o;
}

PS_OUT ps(PS_INPUT i) : COLOR
{
	PS_OUT o = (PS_OUT)0;
	//if(i.height <= waterHeight)
	//	discard;
		
	float4 diffuse = tex2D(diffuseSml, i.tc);
	diffuse *= tex2D(diffuseSml, -i.tc*0.3f);
	
	float4 normal = mul(tex2D(normalSml, i.tc*0.13f) * 
						tex2D(normalSml, -i.tc*0.623f), World);
	normalize(normal);
	float s = dot(-g_sunLight.dir, normal.xyz);
	//diffuse *= s;
	
	o.color = float4(1,1,1,1);// saturate(i.diffuse * diffuse);
	o.z = float4(i.z,0,0,0 );
	return o;
}

/////////////////////////////////////////////////////////////////////////////////////////
// Techniques
/////////////////////////////////////////////////////////////////////////////////////////
Technique Default_VS_1_1
{
    pass p0
    {
		Ambient				= float4(1.0f, 1.0f, 1.0f, 1.0f);
		Lighting			= true;
		SpecularEnable		= true;
		DitherEnable		= true;
		FillMode			= solid;
		ShadeMode			= gouraud;
		ZEnable				= true;
		ZWriteEnable		= true;
		ZFUNC				= LESSEQUAL;
		COLORWRITEENABLE1	= 0xFFFFFFFF;
		
		VertexShader = compile vs_3_0 vs();
		PixelShader = compile ps_3_0 ps();
    
 	}
}