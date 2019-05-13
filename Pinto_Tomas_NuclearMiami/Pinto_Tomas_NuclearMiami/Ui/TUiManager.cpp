// Tomas Pinto - 2019

#include "pch.h"

#include "TUiManager.h"
#include "TUiNode.h"
#include "TUiContainer.h"
#include "TUiLabel.h"
#include "TUiButton.h"
#include "TUiUtils.h"
#include "TUiDynamicLabel.h"

#include "..\ResourceManager.h"
#include "..\Scene.h"

#include "..\Player.h"
#include "..\AiAgent.h"
#include "..\Weapon.h"
#include "..\RocketLauncher.h"
#include "..\NextLevelPad.h"
#include "..\HealthPickup.h"

#include <sstream>
#include <deque>

TUiManager* TUiManager::m_pUiManager = nullptr;

TUiManager* TUiManager::Get()
{
	if (!m_pUiManager)
		m_pUiManager = new TUiManager();
    
	return m_pUiManager;
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
	
	m_TokenMap["TDynamicLabel"] = [](std::ifstream& descriptorStream, std::string resource)
	{
		return new TUiDynamicLabel(descriptorStream, resource);
	};
    
	// Not implemented, just for testing for now
	m_TokenMap["TSLider"] = [](std::ifstream& descriptorStream, std::string resource)
	{
		return new TUiEmpty();
	};
    
	m_TokenMap["TTextBox"] = [](std::ifstream& descriptorStream, std::string resource)
	{
		return new TUiEmpty();
	};
    
	m_TokenMap["TImage"] = [](std::ifstream& descriptorStream, std::string resource)
	{
		return new TUiEmpty();
	};
    
	// End token
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
	
	// This token exists to process SoundStream pre-loading
	m_TokenMap["TPreloadSoundStream"] = [](std::ifstream& descriptorStream, std::string resource)
	{
		std::string path = utils::GetParameterValue("path", resource);
		std::string name = utils::GetParameterValue("name", resource);
		
		ResourceManager::Get()->LoadSoundStream(path, name);
		return new TUiEmpty();
	};
	
	// This token exists to process SoundStream pre-loading
	m_TokenMap["TPreloadTextRenderer"] = [](std::ifstream& descriptorStream, std::string resource)
	{
		std::string path = utils::GetParameterValue("path", resource);
		std::string name = utils::GetParameterValue("name", resource);
		
		ResourceManager::Get()->LoadTextRenderer(path, name);
		return new TUiEmpty();
	};
	
	// Scene object loaders
	m_TokenMap["TPlayer"] = [](std::ifstream& descriptorStream, std::string resource)
	{
		float x = std::stof(utils::GetParameterValue("posx", resource));
		float y = std::stof(utils::GetParameterValue("posy", resource));
		float z = std::stof(utils::GetParameterValue("posz", resource));
		float r = std::stof(utils::GetParameterValue("rotation", resource));
		
		Player* pPlayer = Scene::Get()->GetPlayer();
		
		if(!pPlayer || pPlayer->IsDead())
		{
			Player* pPlayer = new Player(Vector2f { x, y }, Vector2f { 1.f, 1.f }, r);
			pPlayer->SetZLayer(z);
			Scene::Get()->Add(pPlayer);
			Scene::Get()->SetPlayer(pPlayer);
		}
		else
		{
			pPlayer->SetPosition(Vector2f {x, y});
			pPlayer->SetRotation(r);
		}
		
		return new TUiEmpty();
	};
	
	m_TokenMap["TEnemy"] = [](std::ifstream& descriptorStream, std::string resource)
	{
		float x = std::stof(utils::GetParameterValue("posx", resource));
		float y = std::stof(utils::GetParameterValue("posy", resource));
		float z = std::stof(utils::GetParameterValue("posz", resource));
		float r = std::stof(utils::GetParameterValue("rotation", resource));
	
		Player* pPlayer = Scene::Get()->GetPlayer();
		AiAgent* pAiAgent = new AiAgent(Vector2f { x, y }, Vector2f { 1.f, 1.f }, r, pPlayer);
		pAiAgent->SetZLayer(z);
		Scene::Get()->Add(pAiAgent);
		
		return new TUiEmpty();
	};
	
	m_TokenMap["TRifle"] = [](std::ifstream& descriptorStream, std::string resource)
	{
		float x = std::stof(utils::GetParameterValue("posx", resource));
		float y = std::stof(utils::GetParameterValue("posy", resource));
		float z = std::stof(utils::GetParameterValue("posz", resource));
		float r = std::stof(utils::GetParameterValue("rotation", resource));
	
		Weapon* pRifle = new Weapon(Vector2f {x, y}, Vector2f {1.f, 1.f}, r, "rifle", 200, 15);
		pRifle->SetZLayer(z);
		Scene::Get()->Add(pRifle);
		
		return new TUiEmpty();
	};
	
	m_TokenMap["TPistol"] = [](std::ifstream& descriptorStream, std::string resource)
	{
		float x = std::stof(utils::GetParameterValue("posx", resource));
		float y = std::stof(utils::GetParameterValue("posy", resource));
		float z = std::stof(utils::GetParameterValue("posz", resource));
		float r = std::stof(utils::GetParameterValue("rotation", resource));
	
		Weapon* pPistol = new Weapon(Vector2f {x, y}, Vector2f {1.f, 1.f}, r, "pistol", 50, 4);
		pPistol->SetZLayer(z);
		Scene::Get()->Add(pPistol);
		
		return new TUiEmpty();
	};
	
	m_TokenMap["TRocketLauncher"] = [](std::ifstream& descriptorStream, std::string resource)
	{
		float x = std::stof(utils::GetParameterValue("posx", resource));
		float y = std::stof(utils::GetParameterValue("posy", resource));
		float z = std::stof(utils::GetParameterValue("posz", resource));
		float r = std::stof(utils::GetParameterValue("rotation", resource));
	
		RocketLauncher* pRL = new RocketLauncher(Vector2f {x, y}, Vector2f {1.f, 1.f}, r);
		pRL->SetZLayer(z);
		Scene::Get()->Add(pRL);
		
		return new TUiEmpty();
	};
	
	m_TokenMap["TNextLevelPad"] = [](std::ifstream& descriptorStream, std::string resource)
	{
		float x = std::stof(utils::GetParameterValue("posx", resource));
		float y = std::stof(utils::GetParameterValue("posy", resource));
		float r = std::stof(utils::GetParameterValue("range", resource));
		int l = std::stoi(utils::GetParameterValue("level", resource));
		
		Scene::Get()->Add(new NextLevelPad(Vector2f {x,y}, l, r));	
		return new TUiEmpty();
	};
	
	m_TokenMap["THealthPickUp"] = [](std::ifstream& descriptorStream, std::string resource)
	{
		float x = std::stof(utils::GetParameterValue("posx", resource));
		float y = std::stof(utils::GetParameterValue("posy", resource));
		
		Scene::Get()->Add(new HealthPickup(Vector2f { x,y }, Vector2f { 1.f, 1.f }, 0, 15));	
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
	TUiNode*		pNode = nullptr;
	TUiContainer*	pOpenContainer = nullptr;
    
	bool			isContainerOpen = false;
    
	while (!stream.eof())
	{
		std::getline(stream, line);
        
		for (const TokenPair& token : m_TokenMap)
		{
			size_t i = line.find(token.first);
            
			if (i != -1)
			{
				LOG("Token [ " << token.first << "(... ] found for " << resourceLocation);
				pNode = m_TokenMap[token.first](stream, line);
                
				if (token.first == "TContainer")
				{
					LOG("Container opened [ " << token.first << "(... ]" << "for " << resourceLocation);
                    
					m_pRootNodes.push_back(pNode);
					pOpenContainer = static_cast<TUiContainer*>(pNode);
                    
					isContainerOpen = true;
				}
				else if (token.first == "TEndContainer")
				{
					LOG("Container closed [ " << token.first << "(... ]" << " for " << resourceLocation);
					isContainerOpen = false;
				}
				else if (isContainerOpen && pOpenContainer)
				{
					pNode->SetParent(static_cast<TUiNode*>(pOpenContainer));
					pOpenContainer->GetChildren().push_back(pNode);
				}
				else // If no container is open, none container objects can become root nodes
					m_pRootNodes.push_back(pNode);
				break;
			}
		}
	}
    
	if (isContainerOpen)
		ERR("Container was left open, assuming end of file as container end...");
}

void TUiManager::Update(float dt, Point2f mousePos)
{
	for (TUiNode* pNode : m_pRootNodes)
		pNode->Update(dt, mousePos);
}

void TUiManager::Draw(const Window& window)
{
	for (TUiNode* pNode : m_pRootNodes)
		pNode->Draw(window);
}

TUiNode* TUiManager::GetComponent(std::string id) const
{
	std::deque<std::string> idDecomp;
	std::istringstream idDecompStream(id);
    
	for (std::string temp; std::getline(idDecompStream, temp, '.');)
		idDecomp.push_back(temp);
    
	for (TUiNode* pNode : m_pRootNodes)
	{
		if (pNode->GetId() == idDecomp[0])
		{
			idDecomp.pop_front();
            
			if (idDecomp.size() == 0)
				return pNode;
            
			return pNode->GetComponentWithId(idDecomp);
		}
	}
    
	return nullptr;
}

std::vector<TUiNode*>& TUiManager::GetRootNodes()
{
	return m_pRootNodes;
}
