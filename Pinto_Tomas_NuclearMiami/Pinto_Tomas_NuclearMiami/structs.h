#pragma once
#include <string>
#include <vector>

struct Window
{
	Window(const std::string& title = "Title", float width = 320.0f,
           float height = 180.0f, bool isVSyncOn = true);
    
	std::string title;
	float width;
	float height;
	bool isVSyncOn;
};

struct ExitFlags
{
    bool shouldExit;
	bool isFullScreen;
	SDL_Window* pWindow;
};

struct Point2f
{
	Point2f();
	explicit Point2f(float x, float y);
    
	float x;
	float y;
    
	float DistanceTo(const Point2f& other)
	{
		return std::sqrt(std::pow(other.x - this->x, 2.f) + std::pow(other.y - this->y, 2.f));
	}
	
	Point2f& operator+(Point2f rhs)
	{
        x += rhs.x;
		y += rhs.y;
		return *this;
	}
	
	Point2f& operator-(Point2f rhs)
	{
        x -= rhs.x;
		y -= rhs.y;
		return *this;
	}
};

struct Rectf
{
	Rectf();
	explicit Rectf(float left, float bottom, float width, float height);
    
	float left;
	float bottom;
	float width;
	float height;
    
	bool ContainsPoint(const Point2f& other) const
	{
		return (other.x > left && other.x < left + width) &&
			(other.y > bottom && other.y < bottom + height);
	}
    
	bool IsOverlapping(const Rectf& other) const
	{
		return (left < other.width + other.left && width + left > other.left) &&
			(height + bottom > other.bottom && bottom < other.height + other.bottom);
	}
    
	Rectf& operator*=(Rectf rhs)
	{
		this->left *= rhs.left;
		this->bottom *= rhs.bottom;
		this->height *= rhs.height;
		this->width *= rhs.width;
        
		return *this;
	}
};

struct Color4f
{
	Color4f();
	explicit Color4f(float r, float g, float b, float a);
    
	float r;
	float g;
	float b;
	float a;
};

struct Circlef
{
	Circlef();
	explicit Circlef(const Point2f& center, float radius);
	explicit Circlef(float centerX, float centerY, float radius);
    
	Point2f center;
	float radius;
    
	bool ContainsPoint(const Point2f& other)
	{
		return (center.DistanceTo(other) < radius);
	}
    
	bool IsOverlapping(const Circlef& other)
	{
		return ((radius + other.radius) > center.DistanceTo(other.center));
	}
};


struct Ellipsef
{
	Ellipsef();
	explicit Ellipsef(const Point2f& center, float radiusX, float radiusY);
	explicit Ellipsef(float centerX, float centerY, float radiusX, float radiusY);
    
	Point2f center;
	float radiusX;
	float radiusY;
};
