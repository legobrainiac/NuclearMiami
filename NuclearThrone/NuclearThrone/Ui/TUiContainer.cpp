// Tomas Pinto - 2019

#include "pch.h"
#include "TUiContainer.h"

TUiContainer::TUiContainer(std::ifstream& descriptorStream, std::string descriptor)
{
	TUiNode::ProcessDescriptor(descriptorStream, descriptor);
	ProcessDescriptor(descriptorStream, descriptor);
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
    if(!m_Active) return;
	TUiNode::Update(dt, mousePos);
}

void TUiContainer::ProcessDescriptor(std::ifstream& descriptorStream, std::string descriptor)
{
	m_Title = utils::GetParameterValue("title", descriptor);
    m_DrawOutline = bool(std::stoi(utils::GetParameterValue("drawOutline", descriptor)));
    
    m_Active = !bool(std::stoi(utils::GetParameterValue("startHidden", descriptor)));
}
