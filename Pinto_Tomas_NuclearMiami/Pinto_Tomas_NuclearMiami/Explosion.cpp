#include "pch.h"
#include "Explosion.h"

#include "Player.h"

Explosion::Explosion(const Vector2f& position)
: GameObject(position, Vector2f { 1.f, 1.f }, 0.f)
, m_pExplosion(new Sprite("splode", 5, 5, 0.04f))
, m_pExplosionSound(ResourceManager::Get()->GetSoundEffect("explosion"))
{
	Player* pPlayer = Scene::Get()->GetPlayer();
	Vector2f direction { pPlayer->GetPosition().ToPoint2f(), m_Position.ToPoint2f() };
	float rotaion = std::atan2(direction.y, direction.x) * (180 / PI) - 90.f;
	float distance = direction.Length() / 4.f;
	
	m_pExplosionSound->PlayDirectional(rotaion, distance);	
}

void Explosion::Draw() const
{
	glPushMatrix();
	glTranslatef(m_Position.x, m_Position.y, m_ZLayer);
	glScalef(m_Scale.x, m_Scale.y, 0.f);

	m_pExplosion->Draw(Point2f{-(m_pExplosion->GetFrameWidth() / 2), -(m_pExplosion->GetFrameHeight() / 2)}, 1.f);
	
	glPopMatrix();
}

void Explosion::Update(float dt)
{
	m_pExplosion->Update(dt);
	m_DeleteTimer += dt;
	
	if(m_DeleteTimer > 1.f)
		m_pScene->Delete(this);
}

Explosion::~Explosion()
{
	
}
