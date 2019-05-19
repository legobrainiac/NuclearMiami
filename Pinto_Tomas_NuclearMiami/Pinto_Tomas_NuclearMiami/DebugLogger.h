#ifndef DEBUG_LOGGER_H
#define DEBUG_LOGGER_H

#include <deque>

class DebugLogger
{
public:
	~DebugLogger();
	
	static DebugLogger* Get()
	{
		if(!m_psDebugLogger)
			m_psDebugLogger = new DebugLogger();
		
		return m_psDebugLogger;
	}
	
	void Draw() const;
	void Log(const std::string& message);
	
private:
	DebugLogger();
	
	std::deque<std::string> m_Log;
	float m_LogSpacing;

	static DebugLogger* m_psDebugLogger;
};

#endif // !DEBUG_LOGGER_H 
