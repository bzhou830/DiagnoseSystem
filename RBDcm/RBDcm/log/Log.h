#ifndef _LOG_H__
#define _LOG_H__

class CLog
{
public:
	CLog();
	~CLog();

public:
	void	InitLog(LPCTSTR lpszLogPath);		
	void	WriteLog(const char* fmt, ...);		//������֣������͸�printfһ��
	
protected:
	enum {BUFSIZE = 3000};                      //����������
	char m_tBuf[BUFSIZE];
	
	CString	 m_strLogPath;
	CRITICAL_SECTION  m_crit;                   //����һ���ٽ���
};

#endif