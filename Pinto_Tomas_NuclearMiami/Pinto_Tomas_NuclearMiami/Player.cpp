#include "pch.h"
#include "Player.h"
#include "Texture.h"
#include "Scene.h"
#include "Camera.h"
#include "Projectile.h"

Player::Player(const Vector2f& position, const Vector2f& scale, float rotation, Scene* scene, Camera* camera)
: GameObject(position, scale, rotation)
, m_pTexture(new Texture("Resources/Images/Characters/char1.png"))
, m_pCamera(camera)
, m_pScene(scene)
, m_Timer(0.f)
{
}

Player::~Player()
{
	delete m_pTexture;
}

void Player::Draw() const 
{
	glPushMatrix();
	
	glTranslatef(m_Position.x, m_Position.y, m_ZLayer);
	glRotatef(m_Rotation, 0.f, 0.f, 1.f);
	glScalef(m_Scale.x, m_Scale.y, 0.f);
	
	m_pTexture->Draw(Point2f{-(m_pTexture->GetWidth() / 2), -(m_pTexture->GetHeight() / 2)});
		
	GameObject::Draw();
	
	glPopMatrix();
}

void Player::Update(float dt)
{
	m_Timer += dt;
	
	// Get direction from player to cursor
	Point2f mousePosWS = m_pCamera->GetMouseWS(m_Position);
	Vector2f dir {Point2f{m_Position.x, m_Position.y}, mousePosWS };	
	
	// Look at cursor
	m_Rotation = -(std::atan2(dir.x, dir.y) * 180 / PI);
	
	// Test if it should be shooting
	Shoot(dir, dt);
	
	//Check keyboard state
    const Uint8 *pStates = SDL_GetKeyboardState( nullptr );
    Move(pStates, dt);
	
	// Base Update
	GameObject::Update(dt);
}

void Player::Shoot(const Vector2f& direction, float dt)
{
	if(SDL_GetMouseState(nullptr, nullptr) == SDL_BUTTON_LEFT && m_Timer > 0.1f)
    {
		m_Timer = 0.f;
		Projectile* projectile = new Projectile(m_Position, Vector2f {0.f, 0.f}, 0.f, direction.Normalized());
		m_pScene->Add(projectile);
		
		Vector2f kickBack = Vector2f{direction.Normalized().ToPoint2f(), Point2f{ 0.f, 0.f }};
		
		m_Accelleration += kickBack;
	}
}

void Player::Move(const Uint8* keyStates, float dt)
{	
	if(keyStates[SDL_SCANCODE_W])
    	Translate(Vector2f{0.f, 200.f} * dt);
	
	if(keyStates[SDL_SCANCODE_S])
    	Translate(Vector2f{0.f, -200.f} * dt);
	
	if(keyStates[SDL_SCANCODE_D])
    	Translate(Vector2f{200.f, 0.f} * dt);
	
	if(keyStates[SDL_SCANCODE_A])
    	Translate(Vector2f{-200.f, 0.f} * dt);
}
