#include "pch.h"
#include "Camera.h"
#include "structs.h"

Camera::Camera(float width, float height, Window* window, Point2f* mousePos)
: m_Width(width)
, m_Height(height)
, m_pWindow(window)
, m_pMousePos(mousePos)
{
}

void Camera::SetBoundaries(const Rectf& boundaries)
{
	m_Boundaries = boundaries;
}

Point2f Camera::GetPosition(const Vector2f& target) 
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

Point2f Camera::GetMouseWS(const Vector2f& cameraTarget)
{
	float mouseX = m_pMousePos->x * GetRatio(*m_pWindow);
	float mouseY = m_pMousePos->y * GetRatio(*m_pWindow);
	
	Point2f mousePosWS = Point2f{mouseX, mouseY} + GetPosition(cameraTarget);
	
	return mousePosWS;
}


