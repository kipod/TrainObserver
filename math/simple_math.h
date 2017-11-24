#pragma once

namespace SimpleMath
{

	inline void sinCos( float a, float& s, float& c )
	{
#ifdef WIN32
		float localCos, localSin;
		float local = a;
		_asm	fld		local
		_asm	fsincos
		_asm	fstp	localCos
		_asm	fstp	localSin
		c = localCos;
		s = localSin;
#else
		c = cosf(a);
		s = sinf(a);
#endif
	};

	//template<class T> 
	//T max(const T& a, const T& b)
	//{
	//	return a > b ? a : b;
	//};

	//template<class T> 
	//T min(const T& a, const T& b)
	//{
	//	return a < b ? a : b;
	//};

	inline float clamp(float a, float mn, float mx)  
	{ 
		return a < mn ? mn : (a > mx ? mx : a);
	}

	inline bool almostEqual(float f1,  float f2, float epsilon)
	{
		return fabsf( f1 - f2 ) < epsilon;
	}

}

