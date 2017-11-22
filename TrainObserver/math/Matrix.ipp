inline Matrix::Matrix(void)
{
	memset(m, 0, sizeof(Matrix));
}

inline Matrix::Matrix( bool init )
{
	if(init)
	{
		id();
	}
	else
	{
		memset(m,0,sizeof(Matrix));
	}
}

inline float& Matrix::operator()( size_t x, size_t y )
{
	return m[x][y];
}

const float& Matrix::operator()(size_t x, size_t y) const
{
	return m[x][y];
}


inline const Matrix Matrix::operator+( Matrix& mat )
{
	Matrix t;
	for (size_t i = 0; i < 3; i++) 
		for (size_t j = 0; j < 3; j++) 
			t(i, j) = m[i][j] + mat(i, j);
	return t;
}

inline const Matrix Matrix::operator-( Matrix& mat )
{
	Matrix t;
	for (size_t i = 0; i < 3; i++) 
		for (size_t j = 0; j < 3; j++) 
			t(i, j) = m[i][j] - mat(i, j);
	return t;
}

inline const Matrix Matrix::operator*( const Matrix& mat ) const
{
	Matrix t;
	for (size_t i = 0; i < 3; i++)
	{
		for (size_t j = 0; j < 3; j++)
		{
			for (size_t k = 0; k < 3; k++ )
				t(i, j) += m[i][k] * mat(k, j);
		}
	}
	return t;
}

inline Vector3 Matrix::operator*( Vector3& v )
{
	Vector3 t;
	for (size_t i = 0; i < 3; i++)
	{
		for (size_t j = 0; j < 3; j++)
		{
			t(i) += m[i][j] * v(j);
		}
	}
	return t;
}

inline const Matrix Matrix::operator*( const float t )
{
	Matrix o;
	for (size_t i = 0; i < 3; i++)
	{
		for (size_t j = 0; j < 3; j++)
		{
			o(i, j) = m[i][j] * t;
		}
	}
	return o;
}

inline const Matrix Matrix::T()
{
	Matrix mat;
	for (size_t i = 0; i < 3; i++)
	{
		for (size_t j = 0; j < 3; j++)
		{
			mat(i, j) = m[j][i];
		}
	}
	return mat;
}

inline const float Matrix::Determinant()
{
	return	m[0][0] * (m[1][1] * m[2][2] - m[1][2] * m[2][1]) -
			m[0][1] * (m[1][0] * m[2][2] - m[1][2] * m[2][0]) +
			m[0][2] * (m[1][0] * m[2][1] - m[1][1] * m[2][0]);
}

inline const Matrix Matrix::Reverse()
{
	Matrix res;
	res.m[0][0] = m[1][1] * m[2][2] - m[1][2] * m[2][1];
	res.m[1][0] = -m[1][0] * m[2][2] + m[1][2] * m[2][0];
	res.m[2][0] = m[1][0] * m[2][1] - m[1][1] * m[2][0];

	res.m[0][1] = -m[0][1] * m[2][2] + m[0][2] * m[2][1];
	res.m[1][1] = m[0][0] * m[2][2] - m[0][2] * m[2][0];
	res.m[2][1] = -m[0][0] * m[2][1] + m[0][1] * m[2][0];

	res.m[0][2] = m[0][1] * m[1][2] - m[0][2] * m[1][1];
	res.m[1][2] = -m[0][0] * m[1][2] + m[0][2] * m[1][0];
	res.m[2][2] = m[0][0] * m[1][1] - m[0][1] * m[1][0];

	float d = res.m[0][0]*m[0][0]	+ res.m[0][1]*m[1][0] +res.m[0][2] * m[2][0];
	if(d==0.0f)
	{
		return res;
	}
	return res;
}


inline void Matrix::operator*=( const float t )
{
	for (size_t i = 0; i < 3; i++)
		for (size_t j = 0; j < 3; j++)
			m[i][j] *= t;
}

inline void Matrix::operator*=( Matrix& mat )
{
	Matrix t;
	for (size_t i = 0; i < 3; i++)
	{
		for (size_t j = 0; j < 3; j++)
		{
			for (size_t k = 0; k < 3; k++ )
				t(i, j) = m[i][k] * mat(k, j);
		}
	}
	*this = t;
}

inline Matrix::Matrix( const Matrix& mat )
{
	for(size_t i = 0; i < 4; i++)
		for(size_t j = 0; j < 4; j++)
			m[i][j] = mat(i, j);
}

inline void Matrix::RotateX( float a )
{
	float cosA;
	float sinA;
	SimpleMath::SinCos(a, sinA, cosA);
	Matrix temp(*this);
	m[0][1] = temp.m[0][1] * cosA + temp.m[0][2] * sinA;
	m[0][2] = temp.m[0][2] * cosA - temp.m[0][1] * sinA;
	m[1][1] = temp.m[1][1] * cosA + temp.m[1][2] * sinA;
	m[1][2] = temp.m[1][2] * cosA - temp.m[1][1] * sinA;
	m[2][1] = temp.m[2][1] * cosA + temp.m[2][2] * sinA;
	m[2][2] = temp.m[2][2] * cosA - temp.m[2][1] * sinA;
}

inline void Matrix::RotateY( float a )
{
	float cosA;
	float sinA;
	SimpleMath::SinCos(a, sinA, cosA);
	Matrix temp(*this);
	m[0][0] = temp.m[0][0] * cosA - temp.m[0][2] * sinA;
	m[0][2] = temp.m[0][2] * cosA + temp.m[0][0] * sinA;
	m[1][0] = temp.m[1][0] * cosA - temp.m[1][2] * sinA;
	m[1][2] = temp.m[1][2] * cosA + temp.m[1][0] * sinA;
	m[2][0] = temp.m[2][0] * cosA - temp.m[2][2] * sinA;
	m[2][2] = temp.m[2][2] * cosA + temp.m[2][0] * sinA;
}

inline void Matrix::RotateZ( float a )
{
	float cosA;
	float sinA;
	SimpleMath::SinCos(a, sinA, cosA);
	Matrix temp(*this);
	m[0][0] = temp.m[0][0] * cosA +temp.m[0][1] * sinA;
	m[0][1] = temp.m[0][1] * cosA -temp.m[0][0] * sinA;
	m[1][0] = temp.m[1][0] * cosA +temp.m[1][1] * sinA;
	m[1][1] = temp.m[1][1] * cosA -temp.m[1][0] * sinA;
	m[2][0] = temp.m[2][0] * cosA +temp.m[2][1] * sinA;
	m[2][1] = temp.m[2][1] * cosA -temp.m[2][0] * sinA;
}

inline void Matrix::id()
{
	m[0][0] = m[1][1] = m[2][2] = m[3][3] = 1.0;   
	m[1][0] = m[2][0] = m[0][1] = m[2][1] = m[0][2] = m[1][2] = 0.0;
}

inline void Matrix::SetCol( Vector3& val, unsigned char num )
{
	m[0][num] = val.x;
	m[1][num] = val.y;
	m[2][num] = val.z;
}

inline void Matrix::SetCol( float x, float y, float z, unsigned char num )
{
	m[0][num] = x;
	m[1][num] = y;
	m[2][num] = z;
}
inline void Matrix::SetRow( Vector3& val, unsigned char num )
{
	m[num][0] = val.x;
	m[num][1] = val.y;
	m[num][2] = val.z;
}

inline void Matrix::SetRow( float x, float y, float z, unsigned char num )
{
	m[num][0] = x;
	m[num][1] = y;
	m[num][2] = z;
}

inline Vector3 Matrix::GetCol( unsigned char n )
{
	return Vector3(m[0][n], m[1][n], m[2][n]);
}

inline Vector3 Matrix::GetRow( unsigned char n )
{
	return Vector3(m[n][0], m[n][1], m[n][2]);
}

/**
 *	This method returns a row of the matrix as a Vector3. The rows are indexed
 *	from 0 to 3.
 *
 *	@param i	The index of the desired row.
 *
 *	@return The row with the input index.
 */
inline Vector3& Matrix::operator []( int i )
{
	assert( 0 <= i && i < 4 );
	return *reinterpret_cast<Vector3 *>( m[i] );
}


/**
 *	This method returns a row of the matrix as a Vector3. The rows are indexed
 *	from 0 to 3.
 *
 *	@param i	The index of the desired row.
 *
 *	@return The row with the input index.
 */
inline const Vector3 & Matrix::operator []( int i ) const
{
	assert( 0 <= i && i < 4 );
	return *reinterpret_cast< const Vector3 * >( m[i] );
}


inline void graph::Matrix::Scale( float a )
{
	*this *= a;
}

inline void graph::Matrix::Scale( float x, float y, float z )
{
	(*this)[0] *= x;
	(*this)[1] *= y;
	(*this)[2] *= z;
}

inline void graph::Matrix::SetTranslation(const Vector3& v)
{
	_41 = v.x;
	_42 = v.y;
	_43 = v.z;
}

inline void graph::Matrix::SetTranslation(float x, float y, float z)
{
	_41 = x;
	_42 = y;
	_43 = z;
}

inline const graph::Vector3& graph::Matrix::applyToOrigin() const
{
	return (*this)[3];
}

/**
 *	This method multiplies the point represented by the input vector with this
 *	matrix. The input vector is on the left of the multiplication. i.e. it
 *	produces vM, where v is the input point and M is this matrix.
 *
 *	@return The resulting point represented by a Vector3.
 */

inline Vector3 Matrix::applyPoint( const Vector3& v2 ) const
{
	Vector4 res;
	this->applyPoint( res, v2 );
	return Vector3( res.x, res.y, res.z );
}


/**
 *	This method multiplies the point represented by the input v2 with this
 *	matrix. The input vector is on the left of the multiplication. i.e. it
 *	produces vM, where v1 is the input point and M is this matrix. The resulting
 *	point is placed in v1.
 *
 *	@param v1	The vector to place the resulting point in.
 *	@param v2	The vector representing the point to be transformed.
 */

inline void Matrix::applyPoint( Vector3& v1, const Vector3& v2 ) const
{
	v1 = this->applyPoint( v2 );
}


/**
 *	This method applies this matrix to an input Vector3 and produces a resulting
 *	Vector4.
 *
 *	@param v1	The Vector4 that will be set to the result.
 *	@param v2	The Vector3 that the matrix will be applied to.
 */

inline void Matrix::applyPoint( Vector4& v1, const Vector3& v2 ) const
{
#if defined( SSE_MATH3 )
	__m128 v = _mm_loadu_ps( &v2.x );

	__m128 mx = _mm_loadu_ps( m[0] );
	__m128 my = _mm_loadu_ps( m[1] );
	__m128 mz = _mm_loadu_ps( m[2] );
	__m128 mw = _mm_loadu_ps( m[3] );

	__m128 vx = _mm_shuffle_ps( v, v, _MM_SHUFFLE( 0, 0, 0, 0 ) );
	__m128 vy = _mm_shuffle_ps( v, v, _MM_SHUFFLE( 1, 1, 1, 1 ) );
	__m128 vz = _mm_shuffle_ps( v, v, _MM_SHUFFLE( 2, 2, 2, 2 ) );

	vx = _mm_mul_ps( mx, vx );
	vy = _mm_mul_ps( my, vy );
	vz = _mm_mul_ps( mz, vz );

	__m128 va = _mm_add_ps( vx, vy );
	__m128 vb = _mm_add_ps( vz, mw );

	v = _mm_add_ps( va, vb );
	_mm_storeu_ps( &v1.x, v );

#elif defined( EXT_MATH )
	XPVec3Transform( &v1, &v2, this );
#else
	v1[0] = v2[0] * m[0][0] + v2[1] * m[1][0] + v2[2] * m[2][0] + m[3][0];
	v1[1] = v2[0] * m[0][1] + v2[1] * m[1][1] + v2[2] * m[2][1] + m[3][1];
	v1[2] = v2[0] * m[0][2] + v2[1] * m[1][2] + v2[2] * m[2][2] + m[3][2];
	v1[3] = v2[0] * m[0][3] + v2[1] * m[1][3] + v2[2] * m[2][3] + m[3][3];
#endif
}


/**
 *	This method applies this matrix to an input Vector4 and produces a resulting
 *	Vector4.
 *
 *	@param v1	The Vector4 that will be set to the result.
 *	@param v2	The Vector4 that the matrix will be applied to.
 */

inline void Matrix::applyPoint( Vector4& v1, const Vector4& v2 ) const
{
#if defined( SSE_MATH3 )
	__m128 v = _mm_loadu_ps( &v2.x );

	__m128 mx = _mm_loadu_ps( m[0] );
	__m128 my = _mm_loadu_ps( m[1] );
	__m128 mz = _mm_loadu_ps( m[2] );
	__m128 mw = _mm_loadu_ps( m[3] );

	__m128 vx = _mm_shuffle_ps( v, v, _MM_SHUFFLE( 0, 0, 0, 0 ) );
	__m128 vy = _mm_shuffle_ps( v, v, _MM_SHUFFLE( 1, 1, 1, 1 ) );
	__m128 vz = _mm_shuffle_ps( v, v, _MM_SHUFFLE( 2, 2, 2, 2 ) );
	__m128 vw = _mm_shuffle_ps( v, v, _MM_SHUFFLE( 3, 3, 3, 3 ) );

	vx = _mm_mul_ps( mx, vx );
	vy = _mm_mul_ps( my, vy );
	vz = _mm_mul_ps( mz, vz );
	vw = _mm_mul_ps( mw, vw );

	__m128 va = _mm_add_ps( vx, vy );
	__m128 vb = _mm_add_ps( vz, vw );

	v = _mm_add_ps( va, vb );
	_mm_storeu_ps( &v1.x, v );

#elif defined( EXT_MATH )
	XPVec4Transform( &v1, &v2, this );
#else
	v1[0] = v2[0] * m[0][0] + v2[1] * m[1][0] + v2[2] * m[2][0] + v2[3] * m[3][0];
	v1[1] = v2[0] * m[0][1] + v2[1] * m[1][1] + v2[2] * m[2][1] + v2[3] * m[3][1];
	v1[2] = v2[0] * m[0][2] + v2[1] * m[1][2] + v2[2] * m[2][2] + v2[3] * m[3][2];
	v1[3] = v2[0] * m[0][3] + v2[1] * m[1][3] + v2[2] * m[2][3] + v2[3] * m[3][3];
#endif
}


/**
 *	This method applies the transform to a vector. It is the same as apply point
 *	except that it does not add the translation. The v is on the left of the
 *	multiplication. i.e. it produces vM, where M is this matrix.
 *
 *	@see applyPoint
 */

inline Vector3 Matrix::applyVector( const Vector3& v2 ) const
{
#if defined( SSE_MATH3 )
	__m128 v = _mm_loadu_ps( &v2.x );

	__m128 mx = _mm_loadu_ps( m[0] );
	__m128 my = _mm_loadu_ps( m[1] );
	__m128 mz = _mm_loadu_ps( m[2] );

	__m128 vx = _mm_shuffle_ps( v, v, _MM_SHUFFLE( 0, 0, 0, 0 ) );
	__m128 vy = _mm_shuffle_ps( v, v, _MM_SHUFFLE( 1, 1, 1, 1 ) );
	__m128 vz = _mm_shuffle_ps( v, v, _MM_SHUFFLE( 2, 2, 2, 2 ) );

	vx = _mm_mul_ps( mx, vx );
	vy = _mm_mul_ps( my, vy );
	vz = _mm_mul_ps( mz, vz );

	__m128 va = _mm_add_ps( vx, vy );

	v = _mm_add_ps( va, vz );
	return Vector3( v );

#elif defined( EXT_MATH )
	Vector3 vout;
	XPVec3TransformNormal( &vout, &v2, this );
	return vout;
#else
	return Vector3(
		v2.x * m[0][0] + v2.y * m[1][0] + v2.z * m[2][0],
		v2.x * m[0][1] + v2.y * m[1][1] + v2.z * m[2][1],
		v2.x * m[0][2] + v2.y * m[1][2] + v2.z * m[2][2]);
#endif
}

/**
 *	This method applies the transform to a vector. It is the same as apply point
 *	except that it does not add the translation. The v1 is on the left of the
 *	multiplication. i.e. it produces vM, where M is this matrix.
 *
 *	@param v1	The vector that is set to the resulting vector.
 *	@param v2	The vector that is to be transformed.
 *
 *	@see applyPoint
 */
inline void Matrix::applyVector( Vector3& v1, const Vector3& v2 ) const
{
#if defined( SSE_MATH3 )
	__m128 v = _mm_loadu_ps( &v2.x );

	__m128 mx = _mm_loadu_ps( m[0] );
	__m128 my = _mm_loadu_ps( m[1] );
	__m128 mz = _mm_loadu_ps( m[2] );

	__m128 vx = _mm_shuffle_ps( v, v, _MM_SHUFFLE( 0, 0, 0, 0 ) );
	__m128 vy = _mm_shuffle_ps( v, v, _MM_SHUFFLE( 1, 1, 1, 1 ) );
	__m128 vz = _mm_shuffle_ps( v, v, _MM_SHUFFLE( 2, 2, 2, 2 ) );

	vx = _mm_mul_ps( mx, vx );
	vy = _mm_mul_ps( my, vy );
	vz = _mm_mul_ps( mz, vz );

	__m128 va = _mm_add_ps( vx, vy );

	vx = _mm_add_ps( va, vz );
	vy = _mm_shuffle_ps( vx, vx, _MM_SHUFFLE( 1, 1, 1, 1 ) );
	vz = _mm_shuffle_ps( vx, vx, _MM_SHUFFLE( 2, 2, 2, 2 ) );

	_mm_store_ss( &v1.x, vx );
	_mm_store_ss( &v1.y, vy );
	_mm_store_ss( &v1.z, vz );

#elif defined( EXT_MATH )
	XPVec3TransformNormal( &v1, &v2, this );
#else
	v1.set(
		v2.x * m[0][0] + v2.y * m[1][0] + v2.z * m[2][0],
		v2.x * m[0][1] + v2.y * m[1][1] + v2.z * m[2][1],
		v2.x * m[0][2] + v2.y * m[1][2] + v2.z * m[2][2]);
#endif
}


inline void graph::Matrix::SetRotationAroundVector(const Vector3& v, float angle)
{
	float cosa, sina, _1_cosa;
	SimpleMath::SinCos(angle, sina, cosa);
	_1_cosa = 1.0f - cosa;
	float xy = v.x*v.y;
	float yz = v.y*v.z;
	float xz = v.x*v.z;

	m[0][0] = cosa + _1_cosa*v.x*v.x;	m[0][1] = _1_cosa*xy - sina*v.z;		m[0][2] = _1_cosa*xz + sina*v.y;
	m[1][0] = _1_cosa*xy + sina*v.z;		m[1][1] = cosa + _1_cosa*v.y*v.y;	m[1][2] = _1_cosa*yz - sina*v.x;
	m[2][0] = _1_cosa*xz - sina*v.y;		m[2][1] = _1_cosa*yz + sina*v.x;		m[2][2] = cosa + _1_cosa*v.z*v.z;
}
