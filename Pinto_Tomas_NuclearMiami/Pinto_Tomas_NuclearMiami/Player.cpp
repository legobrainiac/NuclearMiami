#include "pch.h"
#include "Player.h"
#include "Texture.h"
#include "Scene.h"
#include "Camera.h"

Player::Player(const Vector2f& position, const Vector2f& scale, float rotation, const Point2f& mousePos, Scene* scene, Camera* camera)
: GameObject(position, scale, rotation)
, m_pTexture(new Texture("Resources/Images/Characters/char1.png"))
, m_MousePos(mousePos)
, m_pCamera(camera)
, m_pScene(scene)
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

void Player::Shoot()
{
	if(SDL_GetMouseState(nullptr, nullptr) == SDL_BUTTON_LEFT)
    {
		Point2f mousePosWS = Point2f{m_MousePos.x, m_MousePos.y} - Point2f{1280.f / 2.f, 720 / 2.f};
		Vector2f dir {Point2f{m_Position.x, m_Position.y}, mousePosWS };	
		
		//m_pScene->Add(new Projectile(dir.Normalized()));
	}
	
}

void Player::DrawShootRay() const
{
	Point2f mousePosWS = Point2f{m_MousePos.x, m_MousePos.y} - Point2f{1280.f / 2.f, 720 / 2.f};
	
	glPushMatrix();
	glTranslatef(m_Position.x, m_Position.y, 0.f);
	utils::DrawLine(Point2f{0.f, 0.f}, mousePosWS, 5.f);
	glPopMatrix();
}