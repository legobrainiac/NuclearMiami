// Tomas Pinto - 2019

#ifndef TUIBUTTON_H
#define TUIBUTTON_H

#include "TUiNode.h"

#include <functional>

class Texture;

class TUiButton :
public TUiNode
{
    public:
	TUiButton(std::ifstream& descriptorStream, std::string descriptor);
	~TUiButton();
    
	// Inherited via TUiNode
	virtual void Draw(const Window& window) override;
	virtual void Update(float dt, Point2f mousePos) override;
	virtual void ProcessDescriptor(std::ifstream& descriptorStream, std::string descriptor) override;
    
	void RegisterClickCallBack(std::function<void()> callback);
    void RegisterClickDeltaCallBack(std::function<void(float deltaPressed)> callback);
    
    private:
	std::string m_Text;
	std::string m_BackgroundTextureLocation;
    
	Texture* m_pTextTexture;
	Texture* m_pTexture;
	
	bool m_IsOverlapped;
    bool m_IsPressed;
    float m_DeltaPressed;
	Rectf m_Rect;
    
	std::vector<std::function<void()>> m_ClickCallbacks;
    std::vector<std::function<void(float deltaPressed)>> m_ClickDeltaCallbacks;
};

#endif // !TUIBUTTON_H