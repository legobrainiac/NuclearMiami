#include "pch.h"
#include "RocketLauncher.h"
#include "Scene.h"
#include "Rocket.h"
#include "SoundEffect.h"

RocketLauncher::RocketLauncher(const Vector2f& position, const Vector2f& scale, float rotation)
: Weapon(position, scale, rotation, "rocketLauncher", 500, 0)
{
	m_pShootingSound = ResourceManager::Get()->GetSoundEffect("shotgun");
	m_AmmoCount = 10;
}

void RocketLauncher::Shoot(const Vector2f& position, const Vector2f& direction, float rofMod)
{
	if(m_AmmoCount > 0 && m_Timer > 2.f)
	{
		m_Timer -= m_Timer;
		Rocket* projectile = new Rocket(position, Vector2f {1.f, 1.f}, 0.f, direction.Normalized(), m_pOwner);
		m_pScene->Add(projectile);
		
		Vector2f kickBack = Vector2f{direction.Normalized().ToPoint2f(), Point2f{ 0.f, 0.f }};
		
		kickBack *= m_KickBack;
		m_pOwner->ApplyForce(kickBack);
		
		if(rofMod > 1)
			m_pShootingSound->PlayDirectional(m_RotationToPlayer, m_DistanceToPlayer);		
		else
			m_pShootingSound->Play(0);
		
		m_AmmoCount--;
	}
}

RocketLauncher::~RocketLauncher()
{
}
