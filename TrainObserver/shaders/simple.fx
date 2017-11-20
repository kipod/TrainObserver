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
	float2 tc		: TEXCOORD;
};

struct PS_INPUT
{
	float4 position : POSITION;
	float4 normal   : NORMAL;
	float4 diffuse	: COLOR;
	float2 tc		: TEXCOORD0;
	float height	: TEXCOORD1;
	float z			: TEXCOORD2;		
};

struct PS_OUT
{
	float4 color	: COLOR0;
	//float4 z		: COLOR1;
};

PS_INPUT vs(VS_INPUT i)
{
     // Обнуляем все члены экземпляра выходной структуры
     PS_INPUT o = (PS_INPUT)0;
     
     o.tc = i.tc;
	
     //
     // Преобразуем местоположение вершины в однородное пространство
     // отсечения и сохраняем его в члене output.position
     //
     matrix wvp = mul(World, ViewProjection);
     o.position = mul(i.position, wvp);
    // o.height = mul(i.position, World).y;
	// o.z = o.position.z * 0.001f;// * g_farPlane.y;

	// float4 AmbientMtrl = float4(0.8,1,0.8,1);
	// vector DiffuseMtrl = float4(1,1,1,1);

     //
     // Преобразуем вектор освещения и нормаль в пространство вида.
     // Присваиваем компоненте w значение 0, поскольку мы преобразуем
     // векторы, а не точки.
     //
     o.normal = mul(i.normal,   World);

     //
     // Вычисляем косинус угла между вектором света и нормалью
     //
    // float s = dot(-g_sunLight.dir, o.normal);

     //
     // Вспомните, что если угол между нормалью поверхности
     // и вектором освещения больше 90 градусов, поверхность
     // не получает света. Следовательно, если угол больше
     // 90 градусов, мы присваиваем s ноль, сообщая тем самым,
     // что поверхность не освещена.
     //
    // if(s < 0.0f)
      //    s = 0.0f;

     //
     // Отраженный фоновый свет вычисляется путем покомпонентного
     // умножения вектора фоновой составляющей материала и вектора
     // интенсивности фонового света.
     //
     // Отраженный рассеиваемый свет вычисляется путем покомпонентного
     // умножения вектора рассеиваемой составляющей материала на вектор
     // интенсивности рассеиваемого света. Затем мы масштабируем полученный
     // вектор, умножая каждую его компоненту на коэффициент затенения s,
     // чтобы затемнить цвет в зависимости от того, сколько света получает
     // вершина от источника.
     //
     // Сумма фоновой и рассеиваемой компонент дает нам
     // итоговый цвет вершины.
     //
     //o.diffuse = (AmbientMtrl * AmbientLightIntensity);// +
                          //(s * (DiffuseLightIntensity * DiffuseMtrl));

     return o;
}

PS_OUT ps(PS_INPUT i)
{
	PS_OUT o = (PS_OUT)0;
	//o.color = float4(1,1,1,1);// saturate(i.diffuse * diffuse);
	//o.z = float4(i.z,0,0,0 );
	//if(i.height <= waterHeight)
	//	discard;
		
	//float4 diffuse = float4(1, 1, 1, 1);
	float4 diffuse = tex2D(diffuseSml, i.tc);
	//diffuse *= tex2D(diffuseSml, -i.tc*0.3f);
	
	float4 normal = mul(tex2D(normalSml, i.tc), World);
	//					tex2D(normalSml, -i.tc*0.623f), World);
	//normalize(normal);
	float s = saturate(dot(-g_sunLight.dir, normal.xyz));
	diffuse *= s;
	o.color = diffuse;
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