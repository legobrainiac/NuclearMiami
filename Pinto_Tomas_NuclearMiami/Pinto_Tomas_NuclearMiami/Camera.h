#ifndef CAMERA_H
#define CAMERA_H

#include "Vector2f.h"
#include "utils.h"

class Camera
{
public:
	Camera(float width, float height);
	void SetBoundaries(const Rectf& boundaries);
	Point2f GetPosition(const Vector2f& target) const;
	
	float GetHeight() const;
	float GetWidth() const;
	
private:
	float m_Width;
	float m_Height;
	Rectf m_Boundaries;
};

#endif //!CAMERA_H
