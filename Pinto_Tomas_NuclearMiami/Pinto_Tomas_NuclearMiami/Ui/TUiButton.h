// Tomas Pinto - 2019

#ifndef TUIBUTTON_H
#define TUIBUTTON_H

#include "TUiNode.h"

#include <functional>

class SoundEffect;
class Texture;

typedef std::function<void()> SimpleCallback;
typedef std::function<void(float deltaPressed)> TimeCallback;

class TUiButton :
public TUiNode
{
    public:
	TUiButton(std::ifstream& descriptorStream, std::string descriptor);
	~TUiButton();
	
    TUiButton(const TUiButton&) = delete;
	TUiButton& operator=(const TUiButton&) = delete;
	TUiButton(TUiButton&&) = delete;
	TUiButton& operator=(TUiButton&&) = delete;
    
	virtual void Draw(const Window& window) override;
	virtual void Update(float dt, Point2f mousePos) override;
	virtual void ProcessDescriptor(std::ifstream& descriptorStream, std::string descriptor) override;
    
	void RegisterClickCallBack(SimpleCallback callback);
    void RegisterHoverCallBack(SimpleCallback callback);
    void RegisterClickDeltaCallBack(TimeCallback callback);
	
    private:
	std::string m_Text;
	std::string m_BackgroundTextureLocation;
	std::string m_ClickSoundLocation;
	std::string m_HoverSoundLocation;
    
	Texture* m_pTextTexture;
	Texture* m_pTexture;
	
	SoundEffect* m_pHoverSound;
	SoundEffect* m_pClickSound;
	
	bool m_IsOverlapped;
	bool m_IsOverlappedTriggered;
	bool m_IsPressed;
    float m_DeltaPressed;
	Rectf m_Rect;
    
	std::vector<SimpleCallback> m_ClickCallbacks;
	std::vector<SimpleCallback> m_HoverCallbacks;
    std::vector<TimeCallback> m_ClickDeltaCallbacks;
};

#endif // !TUIBUTTON_H