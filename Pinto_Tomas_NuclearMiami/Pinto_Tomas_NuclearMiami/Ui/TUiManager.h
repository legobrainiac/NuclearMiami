// Tomas Pinto - 2019

// TODO(tomas): Convert everything from using the shitty calling system to using OpenGL matrices, this would in turn make it even easier to handle the child scaling stuff.

#ifndef TUIMANAGER_H
#define TUIMANAGER_H

#include <vector>
#include <memory>

#include <fstream>
#include <string>
#include <map>

#include <functional>

#include "TUiContainer.h"
#include "../structs.h"

typedef std::pair<std::string, std::function<TUiNode*(std::ifstream&, std::string)>> TokenPair;

class TUiManager
{
    public:
	TUiManager();
	~TUiManager();
	
	static TUiManager& Get();
	static void Reset();
    
	void LoadUiDescriptor(std::string resourceLocation);
    
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
	std::vector<TUiNode*>	m_RootNodes;
    
	std::map<std::string, std::function<TUiNode*(std::ifstream&, std::string)>> m_TokenMap;
};

#endif // !TUIMANAGER_H
