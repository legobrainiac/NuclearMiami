#ifndef NEXT_LEVEL_PAD_H
#define NEXT_LEVEL_PAD_H

#include "GameObject.h"

class Texture;

class NextLevelPad :
	public GameObject
{
public:
	NextLevelPad(const Vector2f& position, int level, float range);
	~NextLevelPad();
	
	void Draw() const override;
	void Update(float dt) override;
	
	static int GetCount() { return m_pPadCount; }
	
private:
	Texture* m_pTexture;
	int m_Level;
	float m_Range;
	
	static int m_pPadCount;
};

#endif // !NEXT_LEVEL_PAD_H