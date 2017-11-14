#ifndef VECTOR4_H
#define VECTOR4_H

#include "Vector3.h"

namespace graph
{
	struct Vector4: public Vector4Base
	{
	public:
		Vector4(void);
		Vector4(float x, float y, float z, float w);
		Vector4(int x, int y, int z, int w);
		Vector4(const Vector4& v);
		explicit Vector4( const Vector4Base & v );
		Vector4( const Vector3 & v, float w );
#ifdef _WIN32
		Vector4( __m128 v4 );
#endif

		void setZero();
		void set( float a, float b, float c, float d ) ;
		void scale( const Vector4& v, float s );
		void scale( float s );
		float length() const;
		float lengthSquared() const;
		void normalise();
		bool almostEqual( const Vector4& v1, const Vector4& v2, float epsilon = 0.0004f );

		float& operator()(size_t i);
		const float& operator()(size_t i) const;
		Vector4 operator +(const Vector4& v) const;
		Vector4 operator -(const Vector4& v) const;
		Vector4 operator -() const;
		Vector4 operator *(const Vector4& v) const;
		Vector4 operator ^(const Vector4& v) const;
		float operator &(const Vector4& v) const;
		Vector4 operator *(const float& t) const;
		Vector4 operator /(const float& t) const;
		//Vector operator *( Matrix& m) const;
		//void operator *=( Matrix& m );
		void operator *=(float t);
		void operator *=(const Vector4& t);
		void operator +=(const Vector4& v);
		void operator -=(const Vector4& v);
		inline bool operator > (const Vector4& v) const
		{
			return (x > v.x && y > v.y && z > v.z && w > v.w);
		}
		inline bool operator == (const Vector4& v) const
		{
			return (x == v.x && y == v.y && z == v.z && w == v.w);
		}
		inline bool operator < (const Vector4& v) const
		{
			return (x < v.x && y < v.y && z < v.z && w < v.w);
		}
		inline bool operator >= (const Vector4& v) const
		{
			return (x >= v.x && y >= v.y && z >= v.z);
		}
		inline bool operator <= (const Vector4& v) const
		{
			return (x <= v.x && y <= v.y && z <= v.z && w <= v.w);
		}
		inline bool isZero() const
		{
			return (x == 0.0f) && (y == 0.0f) && (z == 0.0f) && (w == 0.0f);
		}

		inline unsigned int ToRGB()
		{
			return GRGB((unsigned char)(x*255.f),(unsigned char)(y*255.f),(unsigned char)(z*255.f));
		}

		inline float luminance() const
		{
			return (x+y+z+w) / 4.f;
		}

		inline float contrast(const Vector4& v) const
		{
			return fabsf(x - v.x) + fabsf(y - v.y) + fabsf(z - v.z) + fabsf(w - v.w);
		}

		// this must me normalized!
		void rotateRandomly(float angle);


		void AddRef();
		void Release();
	};

#include "Vector4.ipp"
}

#endif //VECTOR_H