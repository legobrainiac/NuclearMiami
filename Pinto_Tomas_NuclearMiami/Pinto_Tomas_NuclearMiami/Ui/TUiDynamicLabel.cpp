#include "pch.h"
#include "TUiDynamicLabel.h"

TUiDynamicLabel::TUiDynamicLabel(std::ifstream& descriptorStream, std::string descriptor)
: TUiNode(descriptorStream, descriptor)
{
	ProcessDescriptor(descriptorStream, descriptor);
	m_pTextRender = ResourceManager::Get()->GetTextRenderer("munro");
}

void TUiDynamicLabel::ProcessDescriptor(std::ifstream& descriptorStream, std::string descriptor)
{
	m_Text = " ";
	m_Prefix = utils::GetParameterValue("prefix", descriptor);
}

void TUiDynamicLabel::Draw(const Window& window)
{
	if(!m_Active) return;
	
	TextRenderConfig textConfig;
	textConfig.spacing = 3.f;
	textConfig.scale = m_Size.x;

	std::string str = m_Prefix + m_Text;
	m_pTextRender->DrawString(str, m_Pos, textConfig);
}

TUiDynamicLabel::~TUiDynamicLabel()
{
}
