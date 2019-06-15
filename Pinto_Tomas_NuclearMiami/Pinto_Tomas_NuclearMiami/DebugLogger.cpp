#include "pch.h"
#include "DebugLogger.h"

DebugLogger* DebugLogger::m_psDebugLogger = nullptr;

DebugLogger::DebugLogger()
: m_LogSpacing(30.f)
{
}

DebugLogger::~DebugLogger()
{
}

void DebugLogger::Log(const std::string& message)
{
	m_Log.push_back(message);
	if(m_Log.size() > MAX_LOG_SIZE) m_Log.pop_front();
}

void DebugLogger::Draw() const
{
	TextRenderer* pTextRenderer = ResourceManager::Get()->GetTextRenderer("munroDebug");
	
	TextRenderConfig textConfig;
	textConfig.spacing = 4.f;
	textConfig.scale = 0.7f;
	
	int i = 0;	
	int padding = 5;
	
	for(std::deque<std::string>::const_iterator it = m_Log.begin(); it != m_Log.end(); it++)
	{
		pTextRenderer->DrawString(*it, Vector2f { 0.f,padding + i * m_LogSpacing }, textConfig);
		++i;
	}
}
