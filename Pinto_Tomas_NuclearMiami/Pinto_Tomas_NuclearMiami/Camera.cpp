#include "pch.h"
#include "Camera.h"

Camera::Camera(float scale, Window* pWindow, Point2f* pMousePos)
: m_Width(pWindow->width / scale)
, m_Height(pWindow->height / scale)
, m_Scale(scale)
, m_pWindow(pWindow)
, m_pMousePos(pMousePos)
{
}

Point2f Camera::GetPosition(const Vector2f& target) 
{
	float xRaw = target.x - (m_Width / 2.f);
	float yRaw = target.y - (m_Height / 2.f);
	
	return Point2f { xRaw, yRaw };
}

Point2f Camera::GetMouseWS(const Vector2f& cameraTarget)
{
	float mouseX = m_pMousePos->x * GetRatio(*m_pWindow);
	float mouseY = m_pMousePos->y * GetRatio(*m_pWindow);
	
	return Point2f{mouseX, mouseY} + GetPosition(cameraTarget);
}

void Camera::Transform(Point2f camPosition)
{
	glScalef(m_pWindow->width / m_Width, m_pWindow->height / m_Height, 0.f);
	glTranslatef(-camPosition.x, -camPosition.y, 0.f);
}

