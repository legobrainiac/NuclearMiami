#include "pch.h"
#include "Turret.h"
#include "Projectile.h"
#include "SoundEffect.h"
#include "Player.h"

Turret::Turret(const Vector2f& position, const Vector2f& scale, float rotation)
: GameObject(position, scale, rotation)
, m_pShootingSound(ResourceManager::Get()->GetSoundEffect("shot_basic"))
{
	m_DoesCollision = false;
}

void Turret::Draw() const
{
	glPushMatrix();
	
	glTranslatef(m_Position.x, m_Position.y, 0.f);
	glRotatef(m_Rotation, 0.f, 0.f, 1.f);
	
	glColor4f(1.f, 0.f, 0.f, 1.f);
	utils::FillRect(0.f, -5.f, 20.f, 5.f);
	
	GameObject::Draw();
	glPopMatrix();	
}

void Turret::Update(float dt)
{
	m_ShotTimer += dt;
	
	Point2f playerPos = m_pScene->GetPlayer()->GetPosition().ToPoint2f();
	
	if(InSight(m_Position.ToPoint2f(), playerPos))
	{
		Vector2f direction = Vector2f { m_Position.ToPoint2f(), playerPos }.Normalized();
		float dirRot = atan2(direction.y, direction.x) * 180.f / PI;
		m_Rotation = utils::LerpDegrees(m_Rotation, dirRot, dt * 6.f);
		
		if(m_ShotTimer > BULLET_INTERVAL)
		{
			m_ShotTimer -= m_ShotTimer;
			
			Vector2f dir { cos(m_Rotation * PI / 180.f), sin(m_Rotation * PI / 180.f) };
			
			Projectile* projectile = new Projectile(m_Position + dir * 20.f, Vector2f {1.f, 1.f}, 0.f, dir, this);
			m_pScene->Add(projectile);
			
			// Play sound
			Point2f myPos = GetPosition().ToPoint2f();
			
			dir = { playerPos, myPos };
			float rotationToPlayer = std::atan2(dir.y, dir.x)  * (180.f / PI) - 90.f;
			float distanceToPlayer = dir.Length() / 4.f;
			
			m_pShootingSound->PlayDirectional(rotationToPlayer, distanceToPlayer);
		}
	}
}

bool Turret::InSight(Point2f tail, Point2f head)
{
	utils::HitInfo hit;
	
	for (std::vector<Point2f> collider : m_pScene->GetSceneCollider())
	{
		if (Raycast(collider, tail, head, hit))
			return false;
	}
	
	return true;
}