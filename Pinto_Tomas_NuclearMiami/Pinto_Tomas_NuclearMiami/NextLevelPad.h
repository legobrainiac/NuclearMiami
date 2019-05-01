#ifndef NEXT_LEVEL_PAD_H
#define NEXT_LEVEL_PAD_H

#include "GameObject.h"

class Texture;

class NextLevelPad :
	public GameObject
{
public:
	NextLevelPad(const Vector2f& position, int level, float range);
	NextLevelPad(const NextLevelPad&) = delete;
	NextLevelPad& operator=(const NextLevelPad&) = delete;
	NextLevelPad(NextLevelPad&&) = delete;
	NextLevelPad& operator=(NextLevelPad&&) = delete;
	~NextLevelPad();
	
	void Draw() const override;
	void Update(float dt) override;
	
private:
	Texture* m_pTexture;
	int m_Level;
	float m_Range;
};

#endif // !NEXT_LEVEL_PAD_H