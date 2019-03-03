#ifndef PLAYER_H
#define PLAYER_H

#include "GameObject.h"

class Texture;

// TODO(tomas): for now it's a single sprite, eventually this will become a sprite sheet
class Player : public GameObject
{
public:
	Player(const Vector2f& position, const Vector2f& scale, float rotation);	
	~Player();
	
	void Draw() const override;
	void Update(float dt) override;
	
private:
	Texture* m_pTexture;	
};

#endif // !PLAYER_H

