#ifndef _LOG_H__
#define _LOG_H__

class CLog
{
public:
	CLog();
	~CLog();

public:
	void	InitLog(LPCTSTR lpszLogPath);		
	void	WriteLog(const char* fmt, ...);		//输出文字，参数就跟printf一样
	
protected:
	enum {BUFSIZE = 3000};                      //工作缓冲区
	char m_tBuf[BUFSIZE];
	
	CString	 m_strLogPath;
	CRITICAL_SECTION  m_crit;                   //设置一个临界区
};

#endif