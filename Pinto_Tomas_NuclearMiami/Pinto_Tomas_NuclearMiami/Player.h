#ifndef PLAYER_H
#define PLAYER_H

#include "GameObject.h"

class Texture;
class Scene;
class Camera;

// TODO(tomas): for now it's a single sprite, eventually this will become a sprite sheet
class Player : public GameObject
{
public:
	Player(const Vector2f& position, const Vector2f& scale, float rotation, const Point2f& mousePos, Scene* scene, Camera* camera);	
	~Player();
	
	void Draw() const override;
	void Update(float dt) override;
	
private:	
	void DrawShootRay() const;
	void Shoot();
	
	Texture* m_pTexture;	
	Scene* m_pScene;
	Camera* m_pCamera;
	const Point2f& m_MousePos;
};

#endif // !PLAYER_H

