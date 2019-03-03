#include "pch.h"
#include "Camera.h"

Camera::Camera(float width, float height)
: m_Width(width)
, m_Height(height)
{
}

void Camera::SetBoundaries(const Rectf& boundaries)
{
	m_Boundaries = boundaries;
}

Point2f Camera::GetPosition(const Vector2f& target) const
{
	return Point2f {target.x - m_Width / 2, target.y - m_Height / 2};
}

float Camera::GetHeight() const
{
	return m_Height;
}

float Camera::GetWidth() const
{
	return m_Width;
}


