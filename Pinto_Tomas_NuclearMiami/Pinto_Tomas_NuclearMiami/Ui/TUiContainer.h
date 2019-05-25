// Tomas Pinto - 2019

#ifndef TUICONTAINER_H
#define TUICONTAINER_H

#include <fstream>
#include <string>

#include "TUiNode.h"

class TUiContainer :
public TUiNode
{
public:
	TUiContainer(std::ifstream& descriptorStream, std::string descriptor);
	~TUiContainer();
	
    TUiContainer(const TUiContainer&) = delete;
	TUiContainer& operator=(const TUiContainer&) = delete;
	TUiContainer(TUiContainer&&) = delete;
	TUiContainer& operator=(TUiContainer&&) = delete;
    
	// Inherited via TUiNode
	virtual void Draw(const Window& window) override;
	virtual void Update(float dt, Point2f mousePos) override;
	virtual void ProcessDescriptor(std::ifstream& descriptorStream, std::string descriptor) override;
    
	void FadeActive(bool val) { m_Active = true; m_FadeActive = val; };
	
private:
	std::string m_Title;
	Vector2f m_BasePos;
    bool m_DrawOutline;
	bool m_FadeActive;
	bool m_TakesInput;
};

#endif // !TUICONTAINER_H
