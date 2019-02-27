// Tomas Pinto - 2019

#include "pch.h"

#include "../Texture.h"
#include "TUiLabel.h"

TUiLabel::TUiLabel(std::ifstream& descriptorStream, std::string descriptor)
{
	TUiNode::ProcessDescriptor(descriptorStream, descriptor);
	ProcessDescriptor(descriptorStream, descriptor);
    
	m_pTexture = new Texture(m_Text, "Resources/Fonts/DIN-Light.otf", (int)m_PtSize, Color4f {1.f, 1.f,1.f, 1.f});
}

TUiLabel::~TUiLabel()
{
	delete m_pTexture;
}

void TUiLabel::Draw(const Window& window)
{
	Rectf rect = utils::GetScalar(*this, window);
	m_pTexture->Draw(rect);
}

void TUiLabel::Update(float dt, Point2f mousePos)
{
}

void TUiLabel::ProcessDescriptor(std::ifstream& descriptorStream, std::string descriptor)
{
	m_Text = utils::GetParameterValue("text", descriptor);
	m_PtSize = std::stof(utils::GetParameterValue("ptSize", descriptor));
}
