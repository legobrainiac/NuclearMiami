// Tomas Pinto - 2019

#include "pch.h"
#include "TUiContainer.h"

TUiContainer::TUiContainer(std::ifstream& descriptorStream, std::string descriptor)
: TUiNode(descriptorStream, descriptor)
, m_FadeActive(true)
, m_BasePos(m_Pos)
{
	ProcessDescriptor(descriptorStream, descriptor);

	if(!m_Active)
	{
		m_Pos = m_BasePos + Vector2f { -1.f, 0.f }; 
	}
}

TUiContainer::~TUiContainer()
{
}

void TUiContainer::Draw(const Window& window)
{
    if(!m_Active) return;
    
    m_ScreenSpaceRect = utils::GetScalar(*this, window);
    
    if(m_DrawOutline)
    {
        glColor3f(1.f, 1.f, 1.f);
        utils::DrawRectLines(m_ScreenSpaceRect, 3.f);
    }
    
	TUiNode::Draw(window);
}

void TUiContainer::Update(float dt, Point2f mousePos)
{
	if(m_FadeActive)
	{
		m_Pos = utils::Lerp(m_Pos, m_BasePos, dt * 4.f);
		m_TakesInput = true;
	}
	else
	{
		m_Pos = utils::Lerp(m_Pos, m_BasePos + Vector2f { -1.f, 0.f }, dt * 4.f);
		m_TakesInput = false;
	}
	
    if(m_Active && m_TakesInput)
		TUiNode::Update(dt, mousePos);
}

void TUiContainer::ProcessDescriptor(std::ifstream& descriptorStream, std::string descriptor)
{
	m_Title = utils::GetParameterValue("title", descriptor);
    m_DrawOutline = bool(std::stoi(utils::GetParameterValue("drawOutline", descriptor)));
    
    m_Active = !bool(std::stoi(utils::GetParameterValue("startHidden", descriptor)));
}
