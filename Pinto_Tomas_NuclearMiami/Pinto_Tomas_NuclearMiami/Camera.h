#ifndef CAMERA_H
#define CAMERA_H

#include "Vector2f.h"

class Camera
{
public:
	Camera(float scale, Window* pWindow, Point2f* pMousePos);

	void Transform(Point2f camPosition);
	void Update(float dt);
	Point2f GetPosition(const Vector2f& target);
	
	float GetRatio(const Window& window) { return (m_Width / window.width); }
	void SetScale(float scale);
	void ForceScale(float scale);
	Point2f GetMouseWS(const Vector2f& cameraTarget);
	
private:
	float m_Width;
	float m_Height;
	float m_Scale;
	float m_IntendedScale;
	
	Window* m_pWindow;
	Point2f* m_pMousePos;
	
	Vector2f m_CameraShake;
};

#endif //!CAMERA_H
