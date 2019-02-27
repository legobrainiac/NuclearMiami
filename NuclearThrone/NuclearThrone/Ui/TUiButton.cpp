#include "pch.h"

#include "../Texture.h"
#include "TUiButton.h"

TUiButton::TUiButton(std::ifstream& descriptorStream, std::string descriptor)
{
	TUiNode::ProcessDescriptor(descriptorStream, descriptor);
	ProcessDescriptor(descriptorStream, descriptor);
    
	m_pTextTexture = new Texture(m_Text, "Resources/Fonts/DIN-Light.otf", 100, Color4f{ 1.f, 1.f,1.f, 1.f });
	m_pTexture = new Texture(m_BackgroundTextureLocation);
}

TUiButton::~TUiButton()
{
	delete m_pTexture;
	delete m_pTextTexture;
}

void TUiButton::Draw(const Window& window)
{
	m_Rect = utils::GetScalar(*this, window);
    
	glColor3f(0.5f, 1.0f, 0.5f);
    
	m_pTexture->Draw(m_Rect);
	m_pTextTexture->Draw(m_Rect);
    
	if (m_IsOverlapped)
		utils::DrawRectLines(m_Rect, 3.f);
}

void TUiButton::Update(float dt, Point2f mousePos)
{
	m_IsOverlapped = m_Rect.ContainsPoint(mousePos);
    
    // Handle callbacks for the button
    if(SDL_GetMouseState(nullptr, nullptr) == SDL_BUTTON_LEFT && m_IsOverlapped)
    {
        if(!m_IsPressed)
        {
            m_IsPressed = true;
            
            for(auto& callback : m_ClickCallbacks)
                callback();
        }
        else
        {
            m_DeltaPressed += dt;
            
            for(auto& callback : m_ClickDeltaCallbacks)
                callback(m_DeltaPressed);
        }
    }
    else
    {
        m_DeltaPressed = 0.f;
        m_IsPressed = false;
    }
}

void TUiButton::ProcessDescriptor(std::ifstream & descriptorStream, std::string descriptor)
{
	m_Text = utils::GetParameterValue("text", descriptor);
	m_BackgroundTextureLocation = utils::GetParameterValue("texture", descriptor);
}

void TUiButton::RegisterClickCallBack(std::function<void()> callback)
{
	m_ClickCallbacks.push_back(callback);
    std::cout << "Click callback registered for " << m_Id << std::endl;
}

void TUiButton::RegisterClickDeltaCallBack(std::function<void(float deltaPressed)> callback)
{
    m_ClickDeltaCallbacks.push_back(callback);
    std::cout << "Delta Click callback registered for " << m_Id << std::endl;
}


