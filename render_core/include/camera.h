#pragma once
#include "math\matrix.h"
#include "math\vector3.h"

struct ScreenPos
{
	unsigned int x = 0;
	unsigned int y = 0;
};

class Camera
{
public:
	Camera();

	void init(float nearPlane, float farPlane, float fov, unsigned int screenWidth, unsigned int screenHeight);

	float	nearPlane() const;
	void	nearPlane(float f);

	float	farPlane() const;
	void	farPlane(float f);

	float	fov() const;
	void	fov(float f);

	float	aspectRatio() const;
	void	aspectRatio(float f);

	float	viewHeight() const;
	void	viewHeight(float height);

	float	viewWidth() const;

	float	yaw() const;
	void	yaw(float val);
	float	pitch() const;
	void	pitch(float val);

	void	lookAt(const Vector3& eye, const Vector3& at, const Vector3& up = Vector3(0.0f, 1.0f, 0.0f));
	void	look(const Vector3& look);
	void	pos(const Vector3& pos);

	Vector3	nearPlanePoint(float xClip, float yClip) const;
	Vector3 farPlanePoint(float xClip, float yClip) const;

	const Matrix& projection() const;
	const Matrix& view() const;
	const Matrix& viewProjection() const;
	const Matrix& invViewProjection() const;

	void beginZBIASDraw(float bias);
	void endZBIASDraw();

	const Vector3& up() const;
	const Vector3& look() const;
	const Vector3& pos() const;

	bool worldPosToScreenPos(const Vector3& worldPos, ScreenPos& screenPos);


private:
	void updateProjection();
private:
	float	m_nearPlane;
	float	m_farPlane;
	float	m_fov;
	float	m_aspectRatio;
	float	m_viewHeight;

	unsigned int	m_screenWidth;
	unsigned int	m_screenHeight;

	float	m_fYaw = 0.0f;
	float	m_fPitch = -30.0f;

	Vector3 m_pos;
	Vector3	m_look;
	Vector3 m_up;

	Matrix	m_view;
	Matrix	m_invView;
	Matrix	m_proj;
	Matrix	m_invProj;

	Matrix	m_viewProjection;
	Matrix	m_invViewProjection;
};

