// Tomas Pinto - 2019

#include "pch.h"
#include "TUiNode.h"

TUiNode::TUiNode(std::ifstream& descriptorStream, std::string descriptor)
: m_Active(true)
{
	ProcessDescriptor(descriptorStream, descriptor);
}

TUiNode::TUiNode()
: m_Active(true)
{
}

TUiNode::~TUiNode()
{
	for (auto children : m_Children)
		delete children;
    
	m_Children.clear();
}

void TUiNode::Draw(const Window& window)
{
    if(!m_Active) return;
    
	for (auto& node : m_Children)
		node->Draw(window);
}

void TUiNode::Update(float dt, Point2f mousePos)
{
    if(!m_Active) return;
    
	for (auto& node : m_Children)
		node->Update(dt, mousePos);
}

void TUiNode::ProcessDescriptor(std::ifstream& descriptorStream, std::string descriptor)
{
	m_Id = utils::GetParameterValue("id", descriptor);
    
	m_Size.x = std::stof(utils::GetParameterValue("sizeX", descriptor));
	m_Size.y = std::stof(utils::GetParameterValue("sizeY", descriptor));
    
	m_Pos.x = std::stof(utils::GetParameterValue("posX", descriptor));
	m_Pos.y = std::stof(utils::GetParameterValue("posY", descriptor));
}

void TUiNode::SetParent(TUiNode* parent)
{
	m_Parent = parent;
}

TUiNode* TUiNode::GetParent() const
{
	return m_Parent;
}

TUiNode* TUiNode::GetComponentWithId(std::deque<std::string> idDecomp) 
{
	if (idDecomp.empty())
		return this;	
    
	for (auto& node : m_Children)
	{
		if (node->GetId() == idDecomp[0])
		{
			idDecomp.pop_front();
			return node->GetComponentWithId(idDecomp);
		}
	}
    
	return nullptr;
}

std::vector<TUiNode*>& TUiNode::GetChildren()
{
	return m_Children;
}

const Vector2f& TUiNode::GetSize() const
{
	return m_Size;
}

const Vector2f& TUiNode::GetPosition() const
{
	return m_Pos;
}

const Rectf& TUiNode::GetSSRect() const
{
	return m_ScreenSpaceRect;
}

std::string TUiNode::GetId() const
{
	return m_Id;
}

void TUiNode::SetSize(Vector2f newSize)
{
	m_Size = newSize;
}

void TUiNode::SetActive(bool val)
{
    m_Active = val;
}