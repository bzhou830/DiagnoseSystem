#include "stdafx.h"
#include "log.h"

CLog::CLog()  //���캯����������־�ļ���Ĭ��·��
{
	::InitializeCriticalSection(&m_crit);   //��ʼ���ٽ���
}


CLog::~CLog()
{
	::DeleteCriticalSection(&m_crit);    //�ͷ����ٽ���
}


void CLog::InitLog(LPCTSTR lpszLogPath)   
{		
	//����һ��Ĭ��·��
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
/*-----------------------�����ٽ���(д�ļ�)------------------------------------*/	
	::EnterCriticalSection(&m_crit);   
	try      
	{
		va_list argptr;          //�����ַ����ĸ�ʽ
		va_start(argptr, fmt);
		_vsnprintf(m_tBuf, BUFSIZE, fmt, argptr);
		va_end(argptr);
	}
	catch (...)
	{
		m_tBuf[0] = 0;
	}
	
	FILE *fp = fopen(m_strLogPath, "a"); //����ӵķ�ʽ������ļ�
	if (fp)
	{
		fprintf(fp,"%s:  ", AfxGetApp()->m_pszExeName);  //���뵱ǰ������
		
		CTime ct ;                          //���뵱ǰʱ��
		ct = CTime::GetCurrentTime();
		fprintf(fp,"%s : ",ct.Format("%m/%d/%Y %H:%M:%S"));
		fprintf(fp, "%s\n", m_tBuf);		//д��
		fclose(fp);		
	}	
	::LeaveCriticalSection(&m_crit);  
/*-------------------�˳��ٽ���----------------------------------------*/	
}


