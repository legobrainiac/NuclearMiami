#include "pch.h"
#include "Player.h"
#include "Texture.h"
#include "Scene.h"
#include "Camera.h"
#include "Projectile.h"
#include "PickUp.h"
#include "Weapon.h"

Player::Player(const Vector2f& position, const Vector2f& scale, float rotation, Scene* scene)
: GameObject(position, scale, rotation)
, m_pTexture(new Texture("Resources/Images/Characters/char1.png"))
, m_pScene(scene)
, m_Timer(0.f)
, m_Collider(position.ToPoint2f(), 10.f)
, m_Health(100)
{
}

Player::~Player()
{
	delete m_pTexture;
	
	for(Weapon* w : m_Weapons)
		if(w) delete w;
}

void Player::DrawBottom() const
{
	glPushMatrix();
	glTranslatef(m_Position.x, m_Position.y, m_ZLayer);
	
	float angle = -(m_Accelleration.AngleWith(Vector2f{0.f, 1.f}) * 180 / PI);
	
	glRotatef(angle, 0.f, 0.f, 1.f);
	glScalef(m_Scale.x, m_Scale.y, 0.f);
	
	//utils::DrawRect(-20.f, -10.f, 40.f, 20.f, 2.f);
	
	m_pTexture->Draw(Point2f{-(m_pTexture->GetWidth() / 2), -(m_pTexture->GetHeight() / 2)});
	glPopMatrix();
}

void Player::DrawTop() const
{
	glPushMatrix();
	glTranslatef(m_Position.x, m_Position.y, m_ZLayer);
	
	// Draw debug line before rotating
#ifdef DEBUG_DRAW
	utils::DrawLine(Point2f {}, m_Accelleration.ToPoint2f(), 2.f);
	
	glColor4f(1.f, 0.f, 0.f, 1.f);
	utils::DrawEllipse(Point2f {}, m_Collider.radius, m_Collider.radius);
#endif
	
	glRotatef(m_Rotation, 0.f, 0.f, 1.f);
	glScalef(m_Scale.x, m_Scale.y, 0.f);
	
	m_pTexture->Draw(Point2f{-(m_pTexture->GetWidth() / 2), -(m_pTexture->GetHeight() / 2)});
	
	GameObject::Draw();
	glPopMatrix();
}

void Player::Draw() const 
{
	DrawBottom();
	DrawTop();
}

void Player::Update(float dt)
{
	m_Timer += dt;
	Vector2f previousPos = m_Position;
	
	//Check keyboard state
    const Uint8 *pStates = SDL_GetKeyboardState( nullptr );
    Move(pStates, dt);
	
	// Get direction from player to cursor
	Camera* pCamera = m_pScene->GetMainCamera();
	
	Point2f mousePosWS = pCamera->GetMouseWS(m_Position);
	Vector2f dir {Point2f{m_Position.x, m_Position.y}, mousePosWS };	
	
	// Look at cursor
	m_Rotation = -(std::atan2(dir.x, dir.y) * 180 / PI);
	
	// Test if it should be shooting
	Shoot(dir, dt);
	
	// Base Update
	GameObject::Update(dt);
	
	// Collision test
	m_Collider.center = m_Position.ToPoint2f();
	
	if(utils::IsOverlapping(m_pScene->GetSceneCollider(), m_Collider))
	{
		m_Accelleration = Vector2f {};
		m_Position = previousPos;
	}
}

bool Player::HasEmptySlot() const
{
	return (m_Weapons.size() < 3);
}

void Player::ProcessPickUp(GameObject* pickUp)
{
	Weapon* pu = dynamic_cast<Weapon*>(pickUp);

	if(pu != nullptr && m_Weapons.size() < 3)
	{
		m_pScene->Remove(pickUp);
		m_Weapons.push_back(static_cast<Weapon*>(pickUp));
	}
}

void Player::SendMessage(std::string message, int value)
{
	if(message == "damage")
	{
		m_Health -= value;
		LOG(message << ": " << value);
		LOG("health: " << m_Health);

		if(m_Health <= 0) 
			m_pScene->Delete(this);

	}
}

void Player::Shoot(const Vector2f& direction, float dt)
{
	if(SDL_GetMouseState(nullptr, nullptr) == SDL_BUTTON_LEFT && m_Weapons.size() > 0 && m_Timer > 0.1f)
    {
		m_Timer = 0.f;
		Projectile* projectile = new Projectile(m_Position, Vector2f {0.f, 0.f}, 0.f, direction.Normalized(), m_pScene, this);
		m_pScene->Add(projectile);
		
		Vector2f kickBack = Vector2f{direction.Normalized().ToPoint2f(), Point2f{ 0.f, 0.f }};
		
		kickBack *= 50.f;
		
		m_Accelleration += kickBack;
	}
}

void Player::Move(const Uint8* keyStates, float dt)
{	
	if(keyStates[SDL_SCANCODE_W])
    	ApplyForce(Vector2f{0.f, 50.f});
	
	if(keyStates[SDL_SCANCODE_S])
    	ApplyForce(Vector2f{0.f, -50.f});
	
	if(keyStates[SDL_SCANCODE_D])
    	ApplyForce(Vector2f{50.f, 0.f});
	
	if(keyStates[SDL_SCANCODE_A])
    	ApplyForce(Vector2f{-50.f, 0.f});
}

