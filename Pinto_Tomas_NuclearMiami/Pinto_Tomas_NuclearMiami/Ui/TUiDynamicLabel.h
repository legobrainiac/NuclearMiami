#ifndef TUIDYNAMIC_LABEL_H
#define TUIDYNAMIC_LABEL_H

#include <string>

#include "TUiNode.h"
#include "../ResourceManager.h"

class TextRenderer;

class TUiDynamicLabel : public TUiNode
{
public:
	TUiDynamicLabel(std::ifstream& descriptorStream, std::string descriptor);
	~TUiDynamicLabel();
	
    TUiDynamicLabel(const TUiDynamicLabel&) = delete;
	TUiDynamicLabel& operator=(const TUiDynamicLabel&) = delete;
	TUiDynamicLabel(TUiDynamicLabel&&) = delete;
	TUiDynamicLabel& operator=(TUiDynamicLabel&&) = delete;
	
	virtual void Draw(const Window& window) override;
	virtual void ProcessDescriptor(std::ifstream& descriptorStream, std::string descriptor) override;

	void SetText(const std::string& str) { m_Text = str; };
	
private:
	std::string m_Prefix;
	std::string m_Text;
	TextRenderer* m_pTextRender;
};

#endif // !TUIDYNAMIC_LABEL_H
