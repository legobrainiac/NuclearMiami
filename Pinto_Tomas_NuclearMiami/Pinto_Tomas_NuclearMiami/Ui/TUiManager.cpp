// Tomas Pinto - 2019

#include "pch.h"

#include "TUiManager.h"
#include "TUiNode.h"
#include "TUiContainer.h"
#include "TUiLabel.h"
#include "TUiButton.h"
#include "TUiUtils.h"

#include "..\ResourceManager.h"

#include <sstream>
#include <deque>

TUiManager* TUiManager::m_pUiManager = nullptr;

TUiManager& TUiManager::Get()
{
	if (!m_pUiManager)
		m_pUiManager = new TUiManager();
    
	return *m_pUiManager;
}

void TUiManager::Reset()
{
	DEBUG("Clearing Ui");
	for (TUiNode* node : m_pUiManager->GetRootNodes())
		delete node;
    
	m_pUiManager->GetRootNodes().clear();
}

TUiManager::TUiManager()
{
	std::cout << "UI Manager initialized..." << std::endl;
    
	m_TokenMap["TContainer"] = [](std::ifstream& descriptorStream, std::string resource)
	{
		return new TUiContainer(descriptorStream, resource);
	};
    
	m_TokenMap["TLabel"] = [](std::ifstream& descriptorStream, std::string resource)
	{
		return new TUiLabel(descriptorStream, resource);
	};
    
	m_TokenMap["TButton"] = [](std::ifstream& descriptorStream, std::string resource)
	{
		return new TUiButton(descriptorStream, resource);
	};
    
	// Not implemented, just for testing for now
	m_TokenMap["TSLider"] = [](std::ifstream& descriptorStream, std::string resource)
	{
		return new TUiLabel(descriptorStream, resource);
	};
    
	m_TokenMap["TTextBox"] = [](std::ifstream& descriptorStream, std::string resource)
	{
		return new TUiLabel(descriptorStream, resource);
	};
    
	m_TokenMap["TImage"] = [](std::ifstream& descriptorStream, std::string resource)
	{
		return new TUiLabel(descriptorStream, resource);
	};
    
	// This token exists only for query necessities, removing this would result in extra code
	m_TokenMap["TEndContainer"] = [](std::ifstream& descriptorStream, std::string resource)
	{
		return nullptr;
	};
	
	// This token exists to process Texture pre-loading
	m_TokenMap["TPreloadTexture"] = [](std::ifstream& descriptorStream, std::string resource)
	{
		std::string path = utils::GetParameterValue("path", resource);
		std::string name = utils::GetParameterValue("name", resource);
		
		ResourceManager::Get()->LoadTexture(path, name);
		return new TUiEmpty();
	};
	
	// This token exists to process SoundEffect pre-loading
	m_TokenMap["TPreloadSoundEffect"] = [](std::ifstream& descriptorStream, std::string resource)
	{
		std::string path = utils::GetParameterValue("path", resource);
		std::string name = utils::GetParameterValue("name", resource);
		
		ResourceManager::Get()->LoadSoundEffect(path, name);
		return new TUiEmpty();
	};
}

TUiManager::~TUiManager()
{
	DEBUG("Shutting down ui manager...");
	Reset();
}

void TUiManager::LoadUiDescriptor(std::string resourceLocation)
{
	std::ifstream	stream(resourceLocation);
	std::string		line;
	TUiNode*		node = nullptr;
	TUiContainer*	openContainer = nullptr;
    
	bool			isContainerOpen = false;
    
	while (!stream.eof())
	{
		std::getline(stream, line);
        
		for (const TokenPair& token : m_TokenMap)
		{
			size_t i = line.find(token.first);
            
			if (i != -1)
			{
				DEBUG("Token [ " << token.first << "(... ] found for " << resourceLocation);
				node = m_TokenMap[token.first](stream, line);
                
				if (token.first == "TContainer")
				{
					DEBUG("Container opened [ " << token.first << "(... ]" << "for " << resourceLocation);
                    
					m_RootNodes.push_back(node);
					openContainer = static_cast<TUiContainer*>(node);
                    
					isContainerOpen = true;
				}
				else if (token.first == "TEndContainer")
				{
					DEBUG("Container closed [ " << token.first << "(... ]" << " for " << resourceLocation);
					isContainerOpen = false;
				}
				else if (isContainerOpen && openContainer)
				{
					node->SetParent(static_cast<TUiNode*>(openContainer));
					openContainer->GetChildren().push_back(node);
				}
				else // If no container is open, none container objects can become root nodes
					m_RootNodes.push_back(node);
                
				break;
			}
		}
	}
    
	if (isContainerOpen)
		ERR("Container was left open, assuming end of file as container end...");
}

void TUiManager::Update(float dt, Point2f mousePos)
{
	for (TUiNode* node : m_RootNodes)
		node->Update(dt, mousePos);
}

void TUiManager::Draw(const Window& window)
{
	for (TUiNode* node : m_RootNodes)
		node->Draw(window);
}

TUiNode* TUiManager::GetComponent(std::string id) const
{
	std::deque<std::string> idDecomp;
	std::istringstream idDecompStream(id);
    
	for (std::string temp; std::getline(idDecompStream, temp, '.');)
		idDecomp.push_back(temp);
    
	for (TUiNode* node : m_RootNodes)
	{
		if (node->GetId() == idDecomp[0])
		{
			idDecomp.pop_front();
            
			if (idDecomp.size() == 0)
				return node;
            
			return node->GetComponentWithId(idDecomp);
		}
	}
    
	return nullptr;
}

std::vector<TUiNode*>& TUiManager::GetRootNodes()
{
	return m_RootNodes;
}
