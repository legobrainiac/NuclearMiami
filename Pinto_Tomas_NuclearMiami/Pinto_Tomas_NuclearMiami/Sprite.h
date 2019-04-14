#pragma once
#include <string>

class Texture;

// TODO(tomas): rule of five, this old code
class Sprite
{
public:
	Sprite(const std::string& texName, int nrCols, int nrRows, float frameSec);
	Sprite(const Sprite& other) = delete;
	Sprite& operator=(const Sprite& other) = delete;
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

