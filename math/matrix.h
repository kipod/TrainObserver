#pragma once
#include "defs.h"
#include "ext_math.h"
#include "vector4.h"
#include <assert.h>

struct Matrix: public MatrixBase
{
public:
	Matrix(void);
	Matrix( bool init );
	Matrix(const Matrix& m);

	inline void id();
	inline float& operator()(size_t x, size_t y);
	inline const float& operator()(size_t x, size_t y) const;
	inline Vector3 & operator []( int i );
	inline const Vector3 & operator []( int i ) const;

	inline const Matrix operator +(Matrix& m);
	inline const Matrix operator -(Matrix& m);
	inline const Matrix operator *(const Matrix& m) const;
	inline Vector3 operator *(Vector3& v);
	inline const Matrix operator *(const float t);
	inline void operator *=(const float t);
	inline void operator *=(Matrix& m);
	inline const Matrix T();
	inline const float Determinant();
	inline const Matrix Reverse();
	inline void RotateX(float a);
	inline void RotateY(float a);
	inline void RotateZ(float a);
	inline void Scale(float a);
	inline void Scale(float x, float y, float z);
	inline void SetTranslation(const Vector3& v);
	inline void SetTranslation(float x, float y, float z);
	inline const Vector3& applyToOrigin() const;

	inline Vector3	applyPoint( const Vector3& v2 ) const;
	inline void	applyPoint( Vector3&v1, const Vector3& v2) const;
	inline void	applyPoint( Vector4&v1, const Vector3& v2) const;
	inline void	applyPoint( Vector4&v1, const Vector4& v2) const;

	inline Vector3	applyVector( const Vector3& v2 ) const;
	inline void applyVector( Vector3& v1, const Vector3& v2 ) const;

	inline void SetRotationAroundVector(const Vector3& v, float angle);

	inline void SetCol(Vector3& val, unsigned char num);
	inline void SetRow(Vector3& val, unsigned char num);
	inline void SetCol(float x, float y, float z, unsigned char num);
	inline void SetRow(float x, float y, float z, unsigned char num);
	inline Vector3 GetCol(unsigned char n);
	inline Vector3 GetRow(unsigned char n);
};

#include "Matrix.ipp"
