// Tomas Pinto - 2019

#ifndef TUIMANAGER_H
#define TUIMANAGER_H

#include <vector>
#include <memory>

#include <fstream>
#include <string>
#include <unordered_map>

#include <functional>

#include "TUiContainer.h"
#include "../structs.h"

typedef std::pair<std::string, std::function<TUiNode*(std::ifstream&, std::string)>> TokenPair;

class TUiManager
{
public:
	~TUiManager();
	
	static TUiManager* Get();
	static void Reset();
    
	void LoadUiDescriptor(std::string resourceLocation);
	void ExecuteOneLiner(const std::string& script);
    
	void Update(float dt, Point2f mousePos);
	void Draw(const Window& window);
    
	TUiNode* GetComponent(std::string id) const;
    template<typename T> 
        T* GetComponent(std::string id) const
    {
        return static_cast<T*>(GetComponent(id));
    }
    
	std::vector<TUiNode*>& GetRootNodes();
    
private:
	static TUiManager*		m_pUiManager;
	std::vector<TUiNode*>	m_pRootNodes;
    
	std::unordered_map<std::string, std::function<TUiNode*(std::ifstream&, std::string)>> m_TokenMap;
	
	TUiManager();
	
	TUiManager(const TUiManager& other) = delete;
	TUiManager& operator=(const TUiManager& other) = delete;
	TUiManager(TUiManager&&) = delete;
	TUiManager& operator=(TUiManager&&) = delete;
};

#endif // !TUIMANAGER_H
