#include "pch.h"
#include "Player.h"
#include "Texture.h"

Player::Player(const Vector2f& position, const Vector2f& scale, float rotation)
: GameObject(position, scale, rotation)
, m_pTexture(new Texture("Resources/Images/Characters/char1.png"))
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
	Rotate(10.f * dt);
	
	//Check keyboard state
    const Uint8 *pStates = SDL_GetKeyboardState( nullptr );
    
	if(pStates[SDL_SCANCODE_W])
    {
    	Translate(Vector2f{0.f, 200.f} * dt);
    }
	
	if(pStates[SDL_SCANCODE_S])
    {
    	Translate(Vector2f{0.f, -200.f} * dt);
    }
	
	if(pStates[SDL_SCANCODE_D])
    {
    	Translate(Vector2f{200.f, 0.f} * dt);
    }
	
	if(pStates[SDL_SCANCODE_A])
    {
    	Translate(Vector2f{-200.f, 0.f} * dt);
    }
    	
	GameObject::Update(dt);
}
