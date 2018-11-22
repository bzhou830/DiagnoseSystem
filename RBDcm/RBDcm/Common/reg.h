#ifndef _RW_REG_H_
#define _RW_REG_H_

#include <windows.h>
#include <string>
#include <tchar.h>

#ifndef tstring
#ifdef _UNICODE
#define tstring std::wstring
#else
#define tstring std::string
#endif
#endif

//////////////////////////////////////////////////////////////////
// rw_reg
//////////////////////////////////////////////////////////////////
class rw_reg
{
public:
	static BOOL GetRegStr(
		tstring strKeyName, 
		tstring strValueName, 
		tstring& strValue, 
		DWORD dwMaxLength = 260, 
		HKEY hKeyParent = HKEY_CURRENT_USER);

	static BOOL GetRegDWORD(
		tstring strKeyName,
		tstring strValueName,
		DWORD& dwValue,
		HKEY hKeyParent = HKEY_CURRENT_USER);
	
	static BOOL GetRegBinary(
		tstring strKeyName, 
		tstring strValueName, 
		BYTE* pBuf, 
		DWORD& dwBytes,
		HKEY hKeyParent = HKEY_CURRENT_USER);
	
	static BOOL SetRegStr(
		tstring strKeyName,
		tstring strValueName,
		tstring strValue,
		HKEY hKeyParent = HKEY_CURRENT_USER);
	
	static BOOL SetRegDWORD(
		tstring strKeyName,
		tstring strValueName,
		DWORD dwValue,
		HKEY hKeyParent = HKEY_CURRENT_USER);
	
	static BOOL SetRegBinary(
		tstring strKeyName, 
		tstring strValueName, 
		BYTE* pBuf, 
		DWORD dwBytes,
		HKEY hKeyParent = HKEY_CURRENT_USER);
	
	static BOOL DelRegSubKey(
		tstring strKeyName,
		tstring strSubKey,
		HKEY hKeyParent = HKEY_CURRENT_USER,
		bool bRecurse = false);
	
	static BOOL DelRegValue(
		tstring strKeyName,
		tstring strValueName,
		HKEY hKeyParent = HKEY_CURRENT_USER);

private:
	rw_reg();
};

#endif