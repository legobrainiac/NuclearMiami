#pragma once
#include <string>

class Texture;

class Sprite
{
public:
	Sprite(const std::string& filename, int nrCols, int nrRows, float frameSec);
	~Sprite();

	void Draw(const Point2f& pos, float scale, bool invertDirection = false) const;
	void Update(float dt);
	float GetFrameWidth() const; 
	float GetFrameHeight() const; 

private:
	Texture* m_pTexture;
	int m_Cols;
	int m_Rows;

	float m_FrameSec;
	float m_AccuSec;
	int m_ActFrame;
};

