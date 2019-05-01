// Tomas Pinto - 2019

#ifndef TUINODE_H
#define TUINODE_H 

#include <vector>
#include <deque>
#include <memory>
#include <string>
#include <fstream>

#include "../Vector2f.h"
#include "../utils.h"

class TUiNode
{
	public:
	TUiNode();
	TUiNode(std::ifstream& descriptorStream, std::string descriptor);
	virtual ~TUiNode();
    
    TUiNode(const TUiNode&) = delete;
	TUiNode& operator=(const TUiNode&) = delete;
	TUiNode(TUiNode&&) = delete;
	TUiNode& operator=(TUiNode&&) = delete;
    
	virtual void Draw(const Window& window);
	virtual void Update(float dt, Point2f mousePos);
	virtual void ProcessDescriptor(std::ifstream& descriptorStream, std::string descriptor);
    
	void SetParent(TUiNode* pParent);
	TUiNode* GetParent() const;
	TUiNode* GetComponentWithId(std::deque<std::string> idDecomp);
	std::vector<TUiNode*>& GetChildren();
    
	const Vector2f& GetSize() const;
	const Vector2f& GetPosition() const;
	const Rectf& GetSSRect() const;
	std::string GetId() const;
	bool GetActive() const;
    
	void SetSize(Vector2f newSize);
    void SetActive(bool val);
    
	protected:
	TUiNode* m_pParent;
	std::vector<TUiNode*> m_pChildren;
    
	std::string m_Id;
	Vector2f m_Size;
	Vector2f m_Pos;
	Rectf m_ScreenSpaceRect;
	bool m_Active;
};

#endif // !TUINODE_H
