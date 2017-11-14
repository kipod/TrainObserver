
inline Vector3::Vector3(void): Vector3Base(0.0f, 0.0f, 0.0f)
{
}

inline Vector3::Vector3( float _x, float _y, float _z ): Vector3Base(_x, _y, _z)
{
}

inline Vector3::Vector3( float _x): Vector3Base(_x, _x, _x)
{
}

inline Vector3::Vector3( int _x, int _y, int _z ): Vector3Base((float)_x, (float)_y, (float)_z)//, m_refCount(0) 
{
}

inline Vector3::Vector3( const Vector3& v ): Vector3Base(v)//, m_refCount(v.m_refCount+1)
{
}

inline Vector3::Vector3(const gRGB& rgb): Vector3Base(float(rgb.r)/255.0f, float(rgb.g)/255.0f, float(rgb.b)/255.0f)
{
}

#if defined(_WIN32) && 0
inline Vector3::Vector3( __m128 v4 )
{
	__m128 vy = _mm_shuffle_ps( v4, v4, _MM_SHUFFLE( 1, 1, 1, 1 ) );
	__m128 vz = _mm_shuffle_ps( v4, v4, _MM_SHUFFLE( 2, 2, 2, 2 ) );
	_mm_store_ss( &x, v4 );
	_mm_store_ss( &y, vy );
	_mm_store_ss( &z, vz );
}
#endif


inline float& Vector3::operator()( size_t i )
{
	if(i == 0)
		return x;
	if(i == 1)
		return y;
	if(i == 2)
		return z;
	throw "Index of vector is out of range";
}

inline const float& Vector3::operator()( size_t i ) const
{
	if(i == 0)
		return x;
	if(i == 1)
		return y;
	if(i == 2)
		return z;
	throw "Index of vector is out of range";
}


inline Vector3 Vector3::operator+( const Vector3& v ) const
{
	return Vector3(x + v.x, y + v.y, z + v.z);
}

inline Vector3 Vector3::operator+( float f ) const
{
	return Vector3(x + f, y + f, z + f);
}

inline Vector3 Vector3::operator-( const Vector3& v ) const
{
	return Vector3(x - v.x, y - v.y, z - v.z);
}

inline Vector3 Vector3::operator*( const Vector3& v ) const
{
	Vector3 temp;
#if EXT_MATH
	XPVec3Cross( &temp, this, &v );
#else
	temp.x = (y * v.z) - (z * v.y);
	temp.y = (z * v.x) - (x * v.z);
	temp.z = (x * v.y) - (y * v.x);
#endif
	return temp;
}

inline Vector3 Vector3::operator^(const Vector3& v) const
{
	Vector3 temp(*this);
	temp.x *= v.x;
	temp.y *= v.y;
	temp.z *= v.z;
	return temp;
}


inline Vector3 Vector3::operator*( const float& t ) const
{
	Vector3 temp(x * t, y * t, z * t);
	return temp;
}

inline Vector3 Vector3::operator/( const float& t ) const
{
	Vector3 temp(x / t, y / t, z / t);
	return temp;
}

inline Vector3 Vector3::operator/( const Vector3& v ) const
{
	Vector3 temp(x / v.x, y / v.y, z / v.z);
	return temp;
}


inline float Vector3::operator&( const Vector3& v ) const
{
#if EXT_MATH
	return XPVec3Dot( this, &v );
#else
	return x * v.x + y * v.y + z * v.z;
#endif
}

inline const float Vector3::LengthSquared() const
{
#if EXT_MATH
	return XPVec3LengthSq(this);
#else
	return x*x + y*y + z*z; 
#endif
}

inline void Vector3::Normalize()
{
#if EXT_MATH
	XPVec3Normalize(this, this);
#else
	float len = sqrt(LengthSquared());
	if(len)
	{
		x /= len;
		y /= len;
		z /= len;
	}
#endif
}

inline Vector3 Vector3::operator-() const
{
	return Vector3(-x,-y,-z);
}

//inline void Vector::operator*=( Matrix& m )
//{
//#if EXT_MATH
//	D3DXVECTOR4 vec4;
//	XPVec3Transform(&vec4, this, &m);
//	x = vec4.x;
//	y = vec4.y;
//	z = vec4.z;
//#else
//	Vector t;
//	for (size_t i = 0; i < 3; i++)
//	{
//		for (size_t j = 0; j < 3; j++)
//		{
//			t(i) += (*this)(j) * m(j, i);
//		}
//	}
//	*this = t;
//#endif
//}



inline void Vector3::operator+=( const Vector3& v )
{
	x+=v.x;
	y+=v.y;
	z+=v.z;
}
inline void Vector3::operator-=( const Vector3& v )
{
	x-=v.x;
	y-=v.y;
	z-=v.z;
}

inline void Vector3::operator*=( float t )
{
	x*=t;
	y*=t;
	z*=t;
}

inline Vector3& Vector3::operator=(const gRGB& rgb)
{
	x = float(rgb.r) / 255.0f;
	y = float(rgb.g) / 255.0f;
	z = float(rgb.b) / 255.0f;
	return *this;
}

inline void Vector3::rotateRandomly(float angle)
{
	Vector3 randomN(RANDF, RANDF, RANDF);
	randomN *= *this;
	randomN.Normalize();
	float cosa, sina;
	SimpleMath::SinCos(angle, sina, cosa);
	*this *= cosa;
	*this += randomN*sina;
	Normalize();
}

inline void Vector3::operator*=(const Vector3& v)
{
	Vector3 temp;
	temp.x = (y * v.z) - (z * v.y);
	temp.y = (z * v.x) - (x * v.z);
	temp.z = (x * v.y) - (y * v.x);
	*this = temp;
}

