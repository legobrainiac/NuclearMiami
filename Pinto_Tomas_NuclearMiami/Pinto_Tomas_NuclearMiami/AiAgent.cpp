#include "pch.h"
#include "AiAgent.h"

#include "utils.h"
#include "Scene.h"
#include "Projectile.h"
#include "Texture.h"
#include "Sprite.h"
#include "ResourceManager.h"
#include "Weapon.h"

AiAgent::AiAgent(const Vector2f& position, const Vector2f& scale, float rotation, GameObject* pTarget)
	: GameObject(position, scale, rotation)
	, m_pTarget(pTarget)
	, m_MinDistance(30.f)
	, m_MaxDistance(1000.f)
	, m_MovementSpeed(50.f)
	, m_pTorsoTexture(ResourceManager::Get()->GetTexture("charTorso"))
	, m_pLegsSprite(new Sprite("charLegsAnimated", 10, 1, 0.05f))
	, m_Health(15)
{
	m_Friction = 10.f;
	m_MaxAcceleration = 100.f;
}

AiAgent::~AiAgent()
{
	delete m_pLegsSprite;
	
	if(m_Weapons.size() > 0)
	{
		m_Weapons[m_Weapons.size() - 1]->SetInWorld(true);
		m_Weapons[m_Weapons.size() - 1]->SetPosition(m_Position);
		m_Weapons[m_Weapons.size() - 1]->SetRotation(m_Rotation);
		m_pScene->Add(m_Weapons[m_Weapons.size() - 1]);
		m_Weapons.pop_back();
	}
}

void AiAgent::ChangeTarget(GameObject* pTarget)
{
	m_pTarget = pTarget;
}

void AiAgent::Update(float dt)
{
	m_Timer += dt;
	
	// Update weapons
	for(Weapon* wp : m_Weapons)
		wp->Update(dt);
	
	// Update sprite animator
	m_pLegsSprite->Update(dt);
	
	// Process Ai
	Ai(dt);

	// Process look direction
	Vector2f dir{ m_Position.ToPoint2f(), m_pTarget->GetPosition().ToPoint2f() };
	m_Rotation = -(std::atan2(dir.x, dir.y) * 180 / PI);
	
	// Base Update
	GameObject::Update(dt);
}

// TODO(tomas): all of this is temporary, there will be multiple different ai's with different textures, drawing will be implemented in them not here
void AiAgent::DrawBottom() const
{
	if (m_Accelleration.Length() < 50.f) return;

	glPushMatrix();
	glTranslatef(m_Position.x, m_Position.y, m_ZLayer);

	glRotatef(m_Rotation, 0.f, 0.f, 1.f);
	glScalef(m_Scale.x, m_Scale.y, 0.f);

	//utils::DrawRect(-20.f, -10.f, 40.f, 20.f, 2.f);
	m_pLegsSprite->Draw(Point2f{ -(m_pLegsSprite->GetFrameWidth() / 2), -(m_pLegsSprite->GetFrameHeight() / 2) }, 1.f);
	glPopMatrix();
}

void AiAgent::DrawTop() const
{
	glPushMatrix();
	glTranslatef(m_Position.x, m_Position.y, m_ZLayer);

	float angle = -(m_Accelleration.AngleWith(Vector2f{ 0.f, 1.f }) * 180 / PI);

	glRotatef(angle, 0.f, 0.f, 1.f);
	glScalef(m_Scale.x, m_Scale.y, 0.f);

	m_pTorsoTexture->Draw(Point2f{ -(m_pTorsoTexture->GetWidth() / 2), -(m_pTorsoTexture->GetHeight() / 2) });

	DrawWeapon();
	
	glPopMatrix();
}

void AiAgent::DrawWeapon() const
{
	glColor4f(1.f, 1.f, 0.f, 1.f);
	
	glPushMatrix();
	
	// m_Weapons[m_SelectedSlot]->Draw();
	
	if(m_Weapons.size() > 0)
		m_Weapons[0]->Draw();
	
	glPopMatrix();
}

void AiAgent::DrawHealth() const
{
	glPushMatrix();
	glTranslatef(m_Position.x, m_Position.y, 0.f);

	glColor4f(1.f, 0.f, 0.f, 1.f);
	utils::FillRect(-12.5f, 10.f, 25.f, 2.f);

	glColor4f(0.f, 1.f, 0.f, 1.f);
	utils::FillRect(-12.5f, 10.f, m_Health / 4.f, 2.f);

	glPopMatrix();
}

void AiAgent::Draw() const
{
	DrawBottom();
	DrawTop();
	DrawHealth();
	GameObject::Draw();
}

void AiAgent::SendMessage(MessageType message, int value)
{
	if (message == MessageType::dammage)
	{
		m_Health -= value;

		if (m_Health <= 0)
			m_pScene->Delete(this);
		
		m_pScene->AddBlood(m_Position, 10);
	}
}

bool AiAgent::ProcessPickUp(PickUp* pickUp)
{
	// TODO(tomas): this is repeated code from the player class. Find a way to make this code shared between both. IHasInventory, teach don't like multiple inheritence doe 
	Weapon* pu = dynamic_cast<Weapon*>(pickUp);
	
	if(pu != nullptr && HasEmptySlot())
	{
		m_pScene->Remove(pickUp);
		m_Weapons.push_back(pu);
		pu->SetOwner(this);
		pu->SetInWorld(false);
		
		return true;
	}
	return false;
}

bool AiAgent::InSight(utils::HitInfo& hitOut, Point2f tail, Point2f head)
{
	for (std::vector<Point2f> collider : m_pScene->GetSceneCollider())
		if (Raycast(collider, tail, head, hitOut))
			return false;
	
	return true;
}

void AiAgent::Ai(float dt)
{
	utils::HitInfo hit;

	Vector2f ray{ m_Position.ToPoint2f(), m_pTarget->GetPosition().ToPoint2f() };

	// Cast first ray, if target in sight, move to it
	for (std::vector<Point2f> collider : m_pScene->GetSceneCollider())
	{ 
		if (InSight(hit, m_Position.ToPoint2f(), m_pTarget->GetPosition().ToPoint2f()))
		{
			if (ray.Length() > m_MinDistance)
				ApplyForce(ray.Normalized() * m_MovementSpeed);

			Shoot(ray.Normalized());
		}
		else // If target not in sight, attempt bounce path-find
		{
			utils::HitInfo hitBounce;
			Vector2f reflection = ray.Reflect(hit.normal);

			// First bounce
			if (Raycast(collider, hit.intersectPoint, (reflection.Normalized() * m_MaxDistance).ToPoint2f(), hitBounce))
			{
				utils::HitInfo hitFinal;

				// Last Bounce
				if (InSight(hitFinal, hitBounce.intersectPoint, m_pTarget->GetPosition().ToPoint2f()))
				{
					// From this point if no obstacle is in sight, move to point
					float tX = (hit.intersectPoint.x + hitBounce.intersectPoint.x) / 2.f;
					float tY = (hit.intersectPoint.y + hitBounce.intersectPoint.y) / 2.f;

					Point2f targetTemp{ tX, tY };
					Vector2f direction{ m_Position.ToPoint2f(), Point2f{ tX, tY } };

					ApplyForce(direction.Normalized() * m_MovementSpeed);
				}
			}
		}
	}
}

void AiAgent::Shoot(Vector2f direction)
{
	if(m_Weapons.size() > 0)
		m_Weapons[0]->Shoot(m_Position, direction, m_pScene, 4);
}