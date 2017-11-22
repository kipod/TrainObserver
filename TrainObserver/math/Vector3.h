#ifndef VECTOR_H
#define VECTOR_H

#include "ext_math.h"
#include "RGB.h"
#include "SimpleMath.h"
#include "defs.h"

namespace graph
{


	struct Vector3: public Vector3Base
	{
	public:
		Vector3(void);
		Vector3(float x, float y, float z);
		Vector3(float x);
		Vector3(int x, int y, int z);
		Vector3(const Vector3& v);
		Vector3(const gRGB& rgb);
#ifdef _WIN32
		Vector3( __m128 v4 );
#endif


		float& operator()(size_t i);
		const float& operator()(size_t i) const;
		Vector3 operator +(const Vector3& v) const;
		Vector3 operator +(float f) const;
		Vector3 operator -(const Vector3& v) const;
		Vector3 operator -() const;
		Vector3 operator *(const Vector3& v) const;
		Vector3 operator /(const Vector3& v) const;
		Vector3 operator ^(const Vector3& v) const;
		float operator &(const Vector3& v) const;
		Vector3 operator *(const float& t) const;
		Vector3 operator /(const float& t) const;
		//Vector operator *( Matrix& m) const;
		//void operator *=( Matrix& m );
		void operator *=(float t);
		void operator *=(const Vector3& t);
		void operator +=(const Vector3& v);
		void operator -=(const Vector3& v);
		inline bool operator > (const Vector3& v) const
		{
			return (x > v.x && y > v.y && z > v.z);
		}
		inline bool operator == (const Vector3& v) const
		{
			return (x == v.x && y == v.y && z == v.z);
		}
		inline bool operator < (const Vector3& v) const
		{
			return (x < v.x && y < v.y && z < v.z);
		}
		inline bool operator >= (const Vector3& v) const
		{
			return (x >= v.x && y >= v.y && z >= v.z);
		}
		inline bool operator <= (const Vector3& v) const
		{
			return (x <= v.x && y <= v.y && z <= v.z);
		}
		inline bool isZero() const
		{
			return (x == 0.0f) && (y == 0.0f) && (z == 0.0f);
		}

		Vector3& operator= (const gRGB& rgb);

		inline unsigned int ToRGB()
		{
			return GRGB((unsigned char)(x*255.f),(unsigned char)(y*255.f),(unsigned char)(z*255.f));
		}

		inline float luminance() const
		{
			return (x+y+z) / 3.f;
		}

		inline float contrast(const Vector3& v) const
		{
			return fabsf(x - v.x) + fabsf(y - v.y) + fabsf(z - v.z);
		}

		inline bool almostEqual(const Vector3 &v1, float epsilon = 0.00001f) const 
		{
			return ((*this - v1).LengthSquared() < epsilon);
		}

		// this must me normalized!
		void rotateRandomly(float angle);

		const float LengthSquared() const;
		const float length() const;
		void Normalize();
	};


	#include "Vector3.ipp"
}

#endif //VECTOR_H