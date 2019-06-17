// Tomas Pinto - 2019

#ifndef TUILABEL_H
#define TUILABEL_H

#include "TUiNode.h"

class Texture;

class TUiLabel :
public TUiNode
{
    public:
	TUiLabel(std::ifstream& descriptorStream, std::string descriptor);
	~TUiLabel();
	
    TUiLabel(const TUiLabel&) = delete;
	TUiLabel& operator=(const TUiLabel&) = delete;
	TUiLabel(TUiLabel&&) = delete;
	TUiLabel& operator=(TUiLabel&&) = delete;
    
	// Inherited via TUiNode
	virtual void Draw(const Window& window) override;
	virtual void Update(float dt, Point2f mousePos) override;
	virtual void ProcessDescriptor(std::ifstream& descriptorStream, std::string descriptor) override;
    
    private:
	std::string m_Text;
	Texture* m_pTexture;
    
	float m_PtSize;
};

#endif // !TUILABEL_H