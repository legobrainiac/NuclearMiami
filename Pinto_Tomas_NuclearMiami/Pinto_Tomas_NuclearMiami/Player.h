#ifndef PLAYER_H
#define PLAYER_H

#include "GameObject.h"

class Texture;
class Scene;
class Camera;
struct Window;

// TODO(tomas): for now it's a single sprite, eventually this will become a sprite sheet
class Player : public GameObject
{
public:
	Player(const Vector2f& position, const Vector2f& scale, float rotation, Scene* scene, Camera* camera);	
	~Player();
	
	void Draw() const override;
	void Update(float dt) override;
	
private:	
	void Shoot(const Vector2f& direction);
	
	Texture* m_pTexture;	
	Scene* m_pScene;
	Camera* m_pCamera;
	float m_Timer;
};

#endif // !PLAYER_H
