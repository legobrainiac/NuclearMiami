#ifndef CAMERA_H
#define CAMERA_H

#include "Vector2f.h"
#include "utils.h"

class Camera
{
public:
	Camera(float width, float height, Window* window, Point2f* mousePos);
	void SetBoundaries(const Rectf& boundaries);
	Point2f GetPosition(const Vector2f& target);
	
	float GetHeight() const;
	float GetWidth() const;
	
	float GetRatio(const Window& window) { return (m_Width / window.width); }
	
	Point2f GetMouseWS(const Vector2f& cameraTarget);
private:
	float m_Width;
	float m_Height;
	Rectf m_Boundaries;
	
	Window* m_pWindow;
	Point2f* m_pMousePos;
};

#endif //!CAMERA_H
