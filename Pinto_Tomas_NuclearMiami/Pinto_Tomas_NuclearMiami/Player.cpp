#include "pch.h"
#include "Player.h"
#include "Texture.h"
#include "Scene.h"
#include "Camera.h"
#include "Projectile.h"
#include "PickUp.h"
#include "Weapon.h"
#include "Sprite.h"
#include "AiAgent.h"

Player::Player(const Vector2f& position, const Vector2f& scale, float rotation, Scene* pScene)
: GameObject(position, scale, rotation, pScene)
, m_pTorsoTexture(new Texture("Resources/Images/Characters/charTorso.png"))
, m_pLegsSprite(new Sprite("Resources/Images/Characters/charLegsAnimated.png", 10, 1, 0.03f))
, m_Timer(0.f)
, m_Health(100)
, m_WeaponPivot {10.f, 0.f}
{
}

Player::~Player()
{
	delete m_pTorsoTexture;
	delete m_pLegsSprite;
	
	for(Weapon* w : m_Weapons)
		if(w) delete w;
}

void Player::DrawBottom() const
{
	// TODO(tomas): get actual movement speed so this doesnt look goofy af
	if(m_Accelleration.Length() < 50.f) return;
	
	glPushMatrix();
	glTranslatef(m_Position.x, m_Position.y, m_ZLayer);
	
	float angle = -(m_Accelleration.AngleWith(Vector2f{0.f, 1.f}) * 180 / PI);
	
	glRotatef(angle, 0.f, 0.f, 1.f);
	glScalef(m_Scale.x, m_Scale.y, 0.f);
	
	m_pLegsSprite->Draw(Point2f{-(m_pLegsSprite->GetFrameWidth() / 2), -(m_pLegsSprite->GetFrameHeight() / 2)}, 1.f);
	glPopMatrix();
}

void Player::DrawTop() const
{
	glPushMatrix();
	glTranslatef(m_Position.x, m_Position.y, m_ZLayer);
	
	glRotatef(m_Rotation, 0.f, 0.f, 1.f);
	glScalef(m_Scale.x, m_Scale.y, 0.f);
	
	m_pTorsoTexture->Draw(Point2f{-(m_pTorsoTexture->GetWidth() / 2), -(m_pTorsoTexture->GetHeight() / 2)});
	
	DrawWeapon();
	
	GameObject::Draw();
	glPopMatrix();
}

void Player::DrawWeapon() const
{
	glColor4f(1.f, 1.f, 0.f, 1.f);

	glPushMatrix();

	// m_Weapons[m_SelectedSlot]->Draw();
	
	if(m_Weapons.size() > 0)
		m_Weapons[0]->Draw();
	
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
	
	// Update weapons
	for(Weapon* wp : m_Weapons)
		wp->Update(dt);
	
	// Update sprite animator
	m_pLegsSprite->Update(dt);
	
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

void Player::SendMessage(MessageType message, int value)
{
	if(message == MessageType::dammage)
	{
		m_Health -= value;
		
		if(m_Health <= 0) 
			m_pScene->Delete(this);

	}
}

void Player::Shoot(const Vector2f& direction, float dt)
{
	if(SDL_GetMouseState(nullptr, nullptr) == SDL_BUTTON_LEFT && m_Weapons.size())
    {
		m_Weapons[0]->Shoot(m_Position, direction, m_pScene);
	}
	
	if(SDL_GetMouseState(nullptr, nullptr) == SDL_BUTTON_X1 && m_Timer > 0.1f) // This is weird, SDL returns the wrong id for my mouse button
	{
		m_Timer = 0.f;
		AiAgent* aiAgent = new AiAgent(m_Position, Vector2f { 1.f, 1.f }, 0.f, this, m_pScene);
		aiAgent->SetZLayer(-1.f);
		m_pScene->Add(aiAgent);
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

