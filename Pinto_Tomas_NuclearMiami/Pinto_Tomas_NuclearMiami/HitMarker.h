#ifndef HITMARKER_H
#define HITMARKER_H

#include "GameObject.h"

class TextRenderer;

class HitMarker :
	public GameObject
{
public:
	HitMarker(const Vector2f& position, const std::string& text);
	~HitMarker();
	
	void Draw() const override;
	void Update(float dt) override;
	
private:
	TextRenderer* m_pTextRenderer;
	std::string m_HitText;
	float m_DeleteTimer;
	
	const float m_LifeTime = 1.f;
};

#endif // !HITMARKER_H
