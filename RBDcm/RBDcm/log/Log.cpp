#include "stdafx.h"
#include "log.h"

CLog::CLog()  //构造函数，设置日志文件的默认路径
{
	::InitializeCriticalSection(&m_crit);   //初始化临界区
}


CLog::~CLog()
{
	::DeleteCriticalSection(&m_crit);    //释放里临界区
}


void CLog::InitLog(LPCTSTR lpszLogPath)   
{		
	//设置一个默认路径
	m_strLogPath=lpszLogPath;
}

void CLog::WriteLog(const char* fmt, ...)
{
	if (m_strLogPath.IsEmpty())
	{	
		::GetCurrentDirectory(255, m_strLogPath.GetBuffer(255));
		m_strLogPath.ReleaseBuffer();
		m_strLogPath = m_strLogPath+"\\"+AfxGetApp()->m_pszExeName+".txt";
	}
	
	if (!AfxIsValidString(fmt, -1))
		return ;
/*-----------------------进入临界区(写文件)------------------------------------*/	
	::EnterCriticalSection(&m_crit);   
	try      
	{
		va_list argptr;          //分析字符串的格式
		va_start(argptr, fmt);
		_vsnprintf(m_tBuf, BUFSIZE, fmt, argptr);
		va_end(argptr);
	}
	catch (...)
	{
		m_tBuf[0] = 0;
	}
	
	FILE *fp = fopen(m_strLogPath, "a"); //以添加的方式输出到文件
	if (fp)
	{
		fprintf(fp,"%s:  ", AfxGetApp()->m_pszExeName);  //加入当前程序名
		
		CTime ct ;                          //加入当前时间
		ct = CTime::GetCurrentTime();
		fprintf(fp,"%s : ",ct.Format("%m/%d/%Y %H:%M:%S"));
		fprintf(fp, "%s\n", m_tBuf);		//写入
		fclose(fp);		
	}	
	::LeaveCriticalSection(&m_crit);  
/*-------------------退出临界区----------------------------------------*/	
}


