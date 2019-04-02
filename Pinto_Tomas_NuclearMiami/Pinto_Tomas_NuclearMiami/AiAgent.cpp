#include "pch.h"
#include "AiAgent.h"

#include "utils.h"
#include "Scene.h"
#include "Projectile.h"
#include "Texture.h"
#include "Sprite.h"

AiAgent::AiAgent(const Vector2f& position, const Vector2f& scale, float rotation, GameObject* pTarget, Scene* scene)
: GameObject(position, scale, rotation)
, m_pTarget(pTarget)
, m_pScene(scene)
, m_MinDistance(30.f)
, m_MaxDistance(1000.f)
, m_MovementSpeed(50.f)
, m_pTorsoTexture(new Texture("Resources/Images/Characters/charTorso.png"))
, m_pLegsSprite(new Sprite("Resources/Images/Characters/charLegsAnimated.png", 10, 1, 0.05f))
, m_Health(20)
{
	m_Friction = 10.f;
	m_MaxAcceleration = 100.f;
}

AiAgent::~AiAgent()
{
}

void AiAgent::ChangeTarget(GameObject* pTarget)
{
	m_pTarget = pTarget;
}

void AiAgent::Update(float dt)
{
	m_Timer += dt;
	m_pLegsSprite->Update(dt);
	Ai(dt);
	
	Vector2f dir { m_Position.ToPoint2f(), m_pTarget->GetPosition().ToPoint2f() };
	m_Rotation = -(std::atan2(dir.x, dir.y) * 180 / PI);
	
	GameObject::Update(dt);
}

// TODO(tomas): all of this is temporary, there will be multiple different ai's with different textures, drawing will be implemented in them not here
void AiAgent::DrawBottom() const
{
	if(m_Accelleration.Length() < 50.f) return;
	
	glPushMatrix();
	glTranslatef(m_Position.x, m_Position.y, m_ZLayer);
	
	glRotatef(m_Rotation, 0.f, 0.f, 1.f);
	glScalef(m_Scale.x, m_Scale.y, 0.f);
	
	//utils::DrawRect(-20.f, -10.f, 40.f, 20.f, 2.f);
	m_pLegsSprite->Draw(Point2f{-(m_pLegsSprite->GetFrameWidth() / 2), -(m_pLegsSprite->GetFrameHeight() / 2)}, 1.f);
	glPopMatrix();
}

void AiAgent::DrawTop() const
{
	glPushMatrix();
	glTranslatef(m_Position.x, m_Position.y, m_ZLayer);
	
	float angle = -(m_Accelleration.AngleWith(Vector2f{0.f, 1.f}) * 180 / PI);
	
	glRotatef(angle, 0.f, 0.f, 1.f);
	glScalef(m_Scale.x, m_Scale.y, 0.f);
	
	m_pTorsoTexture->Draw(Point2f{-(m_pTorsoTexture->GetWidth() / 2), -(m_pTorsoTexture->GetHeight() / 2)});
	
	GameObject::Draw();
	glPopMatrix();
}
						   
void AiAgent::Draw() const
{
	DrawBottom();
	DrawTop();
	GameObject::Draw();
}

void AiAgent::SendMessage(MessageType message, int value)
{
	if(message == MessageType::dammage)
	{
		m_Health -= value;
		
		if(m_Health <= 0) 
			m_pScene->Delete(this);
	}
}

void AiAgent::Ai(float dt)
{
	utils::HitInfo hit;
	
	Vector2f ray{ m_Position.ToPoint2f(), m_pTarget->GetPosition().ToPoint2f() };
	
	// Cast first ray, if target in sight, move to it
	if(!Raycast(m_pScene->GetSceneCollider(), m_Position.ToPoint2f(), m_pTarget->GetPosition().ToPoint2f(), hit))
	{
		if(ray.Length() > m_MinDistance)
			ApplyForce(ray.Normalized() * m_MovementSpeed);
		
		Shoot(ray.Normalized());
	}
	else // If target not in sight, attempt bounce path-find
	{
		utils::HitInfo hitBounce;
		Vector2f reflection = ray.Reflect(hit.normal);
		
		// First bounce
		if(Raycast(m_pScene->GetSceneCollider(), hit.intersectPoint, (reflection.Normalized() * m_MaxDistance).ToPoint2f(), hitBounce))
		{
			utils::HitInfo hitFinal;
			
			// Last Bounce
			if(!Raycast(m_pScene->GetSceneCollider(), hitBounce.intersectPoint, m_pTarget->GetPosition().ToPoint2f(), hitFinal))
			{
				// From this point if no obstacle is in sight, move to point
				float tX = (hit.intersectPoint.x + hitBounce.intersectPoint.x) / 2.f;
				float tY = (hit.intersectPoint.y + hitBounce.intersectPoint.y) / 2.f;
				
				Point2f targetTemp {tX, tY};
				Vector2f direction {m_Position.ToPoint2f(), Point2f{ tX, tY }};
				
				ApplyForce(direction.Normalized() * m_MovementSpeed);
			}
		}	
	}
}

void AiAgent::Shoot(Vector2f direction)
{	
	if(m_Timer < 1.f) return;
	
	m_Timer = 0.f;
	Projectile* projectile = new Projectile(m_Position, Vector2f {0.f, 0.f}, 0.f, direction.Normalized(), m_pScene, this);
	m_pScene->Add(projectile);
	
	Vector2f kickBack = Vector2f{direction.Normalized().ToPoint2f(), Point2f{ 0.f, 0.f }};
	
	kickBack *= 50.f;
	
	m_Accelleration += kickBack;
}