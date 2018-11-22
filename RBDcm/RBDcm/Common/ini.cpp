#include "ini.h"
#include <iostream>
#include <windows.h>
#include <tchar.h>

CIniReader::CIniReader(LPCTSTR szFileName)
{
	memset(m_szFileName, 0x00, sizeof(m_szFileName));
	memcpy(m_szFileName, szFileName, _tcslen(szFileName)*sizeof(TCHAR));
}

int CIniReader::ReadInteger(LPCTSTR szSection, LPCTSTR szKey, int iDefaultValue)
{
	int iResult = GetPrivateProfileInt(szSection, szKey, iDefaultValue, m_szFileName); 
	return iResult;
}

float CIniReader::ReadFloat(LPCTSTR szSection, LPCTSTR szKey, float fltDefaultValue)
{
	TCHAR szResult[255];
	TCHAR szDefault[255];
	float fltResult;
	_stprintf_s(szDefault, 255, TEXT("%f"),fltDefaultValue);
	GetPrivateProfileString(szSection, szKey, szDefault, szResult, 255, m_szFileName); 
	fltResult = (float)_tstof(szResult);
	return fltResult;
}

bool CIniReader::ReadBoolean(LPCTSTR szSection, LPCTSTR szKey, bool bolDefaultValue)
{
	TCHAR szResult[255];
	TCHAR szDefault[255];
	bool bolResult;
	_stprintf_s(szDefault, 255, TEXT("%s"), bolDefaultValue ? TEXT("True") : TEXT("False"));
	GetPrivateProfileString(szSection, szKey, szDefault, szResult, 255, m_szFileName); 
	bolResult = (_tcscmp(szResult, TEXT("True")) == 0 || 
	_tcscmp(szResult, TEXT("true")) == 0) ? true : false;
	return bolResult;
}

LPTSTR CIniReader::ReadString(LPCTSTR szSection, LPCTSTR szKey, LPCTSTR szDefaultValue)
{
	LPTSTR szResult = new TCHAR[255];
	memset(szResult, 0x00, sizeof(szResult));
	GetPrivateProfileString(szSection, szKey, szDefaultValue, szResult, 255, m_szFileName); 
	return szResult;
}

