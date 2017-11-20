#include "camera.h"

Camera::Camera() :
	m_nearPlane(0.1f),
	m_farPlane(10000.f),
	m_fov(D3DXToRadian(102.0f)),
	m_aspectRatio(1.333f),
	m_viewHeight(100)
{
	// Setup View Matrix Values
	lookAt(Vector3(0.0f, 0.0f, 0.0f),
		Vector3(0.0f, 0.0f, -20.0f),
		Vector3(0.0f, 1.0f, 0.0f));
	updateProjection();
}

void Camera::init(float nearPlane, float farPlane, float fov, float aspectRatio)
{
	m_nearPlane = nearPlane;
	m_farPlane = farPlane;
	m_fov = fov;
	m_aspectRatio = aspectRatio;

	// Setup View Matrix Values
	lookAt(Vector3(1.0f, 2.0f, 1.0f),
		Vector3(0.0f, 0.0f, 0.0f),
		Vector3(0.0f, 1.0f, 0.0f));
	updateProjection();
}

float Camera::nearPlane() const
{
	return m_nearPlane;
}

void Camera::nearPlane(float f)
{
	m_nearPlane = f;
	updateProjection();
}


float Camera::farPlane() const
{
	return m_farPlane;
}

void Camera::farPlane(float f)
{
	m_farPlane = f;
	updateProjection();
}


float Camera::fov() const
{
	return m_fov;
}

void Camera::fov(float f)
{
	m_fov = f;
	updateProjection();
}


float Camera::aspectRatio() const
{
	return m_aspectRatio;
}

void Camera::aspectRatio(float f)
{
	m_aspectRatio = f;
	updateProjection();
}


float Camera::viewHeight() const
{
	return m_viewHeight;
}

void Camera::viewHeight(float height)
{
	m_viewHeight = height;
}


float Camera::viewWidth() const
{
	return m_aspectRatio * m_viewHeight;
}

void Camera::lookAt(const Vector3& eye, const Vector3& at, const Vector3& up)
{
	m_pos = eye;
	m_look = at - eye;
	m_look.Normalize();
	m_up = up;

	// Setup View Matrix Values
	D3DXMatrixLookAtLH(&m_view, &m_pos, &at, &m_up);
	D3DXMatrixInverse(&m_invView, NULL, &m_view);
	D3DXMatrixMultiply(&m_viewProjection, &m_view, &m_proj);
}

/**
*	Finds the point on the near plane in camera space,
*	given x and y in clip space.
*/
graph::Vector3 Camera::nearPlanePoint(float xClip, float yClip) const
{
	const float yLength = m_nearPlane * tanf(m_fov * 0.5f);
	const float xLength = yLength * m_aspectRatio;

	return graph::Vector3(xLength * xClip, yLength * yClip, m_nearPlane);
}

graph::Vector3 Camera::farPlanePoint(float xClip, float yClip) const
{
	const float yLength = m_farPlane * tanf(m_fov * 0.5f);
	const float xLength = yLength * m_aspectRatio;

	return graph::Vector3(xLength * xClip, yLength * yClip, m_farPlane);
}

const graph::Matrix& Camera::projection() const
{
	return m_proj;
}

const graph::Matrix& Camera::view() const
{
	return m_view;
}

const graph::Matrix& Camera::viewProjection() const
{
	return m_viewProjection;
}

const graph::Vector3& Camera::up() const
{
	return m_up;
}

const graph::Vector3& Camera::look() const
{
	return m_look;
}

void Camera::look(const Vector3& look)
{
	D3DXVec3Normalize(&m_look,&look);
	Vector3 right;
	D3DXVec3Cross(&right, &m_up, &m_look);
	right.y = 0;
	D3DXVec3Normalize(&right, &right);
	D3DXVec3Cross(&m_up, &m_look, &right);
	//D3DXVec3Normalize(&m_up, &m_up);

	D3DXMatrixLookAtLH(&m_view, &m_pos, &(m_pos + m_look), &m_up);
	D3DXMatrixInverse(&m_invView, NULL, &m_view);
	D3DXMatrixMultiply(&m_viewProjection, &m_view, &m_proj);
}

const graph::Vector3& Camera::pos() const
{
	return m_pos;
}

void Camera::pos(const Vector3& pos)
{
	m_pos = pos;
	D3DXMatrixLookAtLH(&m_view, &m_pos, &(m_pos + m_look), &m_up);
	D3DXMatrixInverse(&m_invView, NULL, &m_view);
	D3DXMatrixMultiply(&m_viewProjection, &m_view, &m_proj);
}

void Camera::updateProjection()
{
	D3DXMatrixPerspectiveFovLH(&m_proj, m_fov, m_aspectRatio, m_nearPlane, m_farPlane);
	D3DXMatrixInverse(&m_invProj, NULL, &m_proj);

	D3DXMatrixMultiply(&m_viewProjection, &m_view, &m_proj);
}

