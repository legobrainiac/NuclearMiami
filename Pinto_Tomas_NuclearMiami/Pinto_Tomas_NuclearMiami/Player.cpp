#include "pch.h"
#include "Player.h"

#include "TextureObject.h"
#include "HealthPickup.h"
#include "Projectile.h"
#include "HitMarker.h"
#include "AiAgent.h"
#include "Camera.h"
#include "PickUp.h"
#include "Weapon.h"

#include "Ui/TUiDynamicLabel.h"
#include "Ui/TUiManager.h"

Player::Player(const Vector2f& position, const Vector2f& scale, float rotation)
: GameObject(position, scale, rotation)
, m_pWeaponHoldTexture(ResourceManager::Get()->GetTexture("charWeaponHold"))
, m_pLegsSprite(new Sprite("charLegsAnimated", 10, 1, 0.04f))
, m_pTorsoSprite(new Sprite("charTorsoAnimated", 10, 1, 0.04f))
, m_Timer(0.f)
, m_Health(100)
, m_WeaponPivot {5.f, 0.f}
{
	m_VertexCollider.push_back(Point2f {-10.f, -5.f});
	m_VertexCollider.push_back(Point2f {-10.f, 5.f});
	m_VertexCollider.push_back(Point2f {10.f, 5.f});
	m_VertexCollider.push_back(Point2f {10.f, -5.f});
	
	m_Scale = Vector2f{1.25f, 1.25f};
}

Player::~Player()
{
	delete m_pLegsSprite;
	delete m_pTorsoSprite;

	for(Weapon* pW : m_wWeapons)
		if(pW) delete pW;
}

void Player::DrawBottom() const
{
	// TODO(tomas): get actual movement speed so this doesnt look goofy af
	if(m_Accelleration.Length() < 50.f) return;
	
	glPushMatrix();
	glTranslatef(m_Position.x, m_Position.y, m_ZLayer);
	glScalef(m_Scale.x, m_Scale.y, 0.f);
	
	float angle = -(m_Accelleration.AngleWith(Vector2f{0.f, 1.f}) * 180 / PI);
	glRotatef(angle, 0.f, 0.f, 1.f);
	
	m_pLegsSprite->Draw(Point2f{-(m_pLegsSprite->GetFrameWidth() / 2), -(m_pLegsSprite->GetFrameHeight() / 2)}, 1.f);
	glPopMatrix();
}

void Player::DrawTop() const
{
	glPushMatrix();
	glTranslatef(m_Position.x, m_Position.y, m_ZLayer);
	glScalef(m_Scale.x, m_Scale.y, 0.f);
	
	glRotatef(m_Rotation, 0.f, 0.f, 1.f);
	
	if(m_wWeapons.size() > 0|| m_Accelleration.Length() < 50.f)
		m_pWeaponHoldTexture->Draw(Point2f{-(m_pWeaponHoldTexture->GetWidth() / 2), -(m_pWeaponHoldTexture->GetHeight() / 2)});
	else
		m_pTorsoSprite->Draw(Point2f{-(m_pTorsoSprite->GetFrameWidth() / 2), -(m_pTorsoSprite->GetFrameHeight() / 2)}, 1.f);
	
	DrawWeapon();
	
	GameObject::Draw();
	glPopMatrix();
}

void Player::DrawWeapon() const
{
	glColor4f(1.f, 1.f, 0.f, 1.f);

	glPushMatrix();

	if(m_wWeapons.size() > 0)
		m_wWeapons[0]->Draw();
	
	glPopMatrix();
}

void Player::DrawHealth() const
{
	glPushMatrix();
	glTranslatef(m_Position.x, m_Position.y, 0.f);
	
	glColor4f(1.f, 0.f, 0.f, 1.f);
	utils::FillRect(-12.5f, 10.f, 25.f, 2.f);
	
	glColor4f(0.f, 1.f, 0.f, 1.f);
	utils::FillRect(-12.5f, 10.f, m_Health / 4.f, 2.f);
	
	glPopMatrix();
}

void Player::Draw() const 
{
	if(IsDead()) return;
	
	DrawBottom();
	DrawTop();
	DrawHealth();
	
	// Frame time render
	glPushMatrix();
	glTranslatef(m_Position.x, m_Position.y, 0.f);
	
	GameObject::Draw();
	glPopMatrix();
}

void Player::DrawHUD() const
{
}

void Player::Update(float dt)
{
	if(IsDead()) return;
	
	m_Timer += dt;
	
	// Update weapons
	for(Weapon* pW : m_wWeapons)
		pW->Update(dt);
	
	// Update sprite animator
	m_pLegsSprite->Update(dt);
	m_pTorsoSprite->Update(dt);
	
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
	
	// Update hud components
	TUiManager::Get()->GetComponent<TUiDynamicLabel>("HUD.health")->SetText(std::to_string(m_Health));
	
	if(m_wWeapons.size() > 0)
	{
		TUiManager::Get()->GetComponent<TUiDynamicLabel>("HUD.ammo")->SetActive(true);
		TUiManager::Get()->GetComponent<TUiDynamicLabel>("HUD.ammo")->SetText(std::to_string(m_wWeapons[0]->GetAmmo()));
	}
	else
	{
		TUiManager::Get()->GetComponent<TUiDynamicLabel>("HUD.ammo")->SetActive(false);
	}
	
	// Base Update
	GameObject::Update(dt);
	
	if(GetAcceleration() > 10.f)
		m_pScene->GetMainCamera()->SetScale(5);
	else
		m_pScene->GetMainCamera()->SetScale(3);
}

// NOTETODO(tomas): for now this is kept at two since i dont have a way of switching weapons :), do this later
bool Player::HasEmptySlot() const
{
	return (m_wWeapons.size() < 1);
}

bool Player::ProcessPickUp(PickUp* pickUp)
{
	// Test if is health pickup
	HealthPickup* pHp = dynamic_cast<HealthPickup*>(pickUp);
	if(pHp !=  nullptr)
	{
		m_Health += pHp->GetHealthGain();
		return true;
	}
	
	// Test if is weapon pickup
	Weapon* pPickUp = dynamic_cast<Weapon*>(pickUp);
	if(pPickUp != nullptr && HasEmptySlot())
	{
		m_pScene->Remove(pickUp);
		m_wWeapons.push_back(pPickUp);
		pPickUp->SetOwner(this);
		pPickUp->SetInWorld(false);
		
		return true;
	}	
	return false;
}

void Player::SendMessage(MessageType message, int value)
{
	if(message == MessageType::dammage && m_Health > 0)
	{
		LDEBUG("Player took " + std::to_string(value) + " dammage...")
		
		m_Health -= value;
		m_pScene->AddBlood(m_Position, 10);
		
		HitMarker* pHm = new HitMarker(m_Position, std::to_string(value));
		m_pScene->Add(pHm);
		
		if(m_Health <= 0)
		{
			TextureObject* pT = new TextureObject(m_Position, m_Rotation, "deadPlayer");
			m_pScene->Add(pT);
			TUiManager::Get()->GetComponent<TUiDynamicLabel>("HUD.health")->SetText(std::to_string(m_Health));
			m_pScene->GetMainCamera()->SetScale(20);
		}
	}
	else if (message == MessageType::regen)
		m_Health += value;
}

void Player::Shoot(const Vector2f& direction, float dt)
{
	if(SDL_GetMouseState(nullptr, nullptr) == SDL_BUTTON_LEFT && m_wWeapons.size())
		m_wWeapons[0]->Shoot(m_Position, direction);
}

void Player::Drop()
{
	if(m_wWeapons.size() > 0)
	{
		m_wWeapons[m_wWeapons.size() - 1]->SetInWorld(true);
		m_wWeapons[m_wWeapons.size() - 1]->SetPosition(m_Position);
		m_wWeapons[m_wWeapons.size() - 1]->SetRotation(m_Rotation);
		m_pScene->Add(m_wWeapons[m_wWeapons.size() - 1]);
		m_wWeapons.pop_back();
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

