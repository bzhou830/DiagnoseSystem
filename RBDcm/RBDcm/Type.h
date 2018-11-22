#pragma once
class CType
{
public:
	CType(void);
	~CType(void);
public:
	static int CString2int(CString str);
	static CString int2CString(int num);
	static double CString2Double(CString str);
	static CString double2CString(double num);
	static double pChar2Double(const char*);
	static int pChar2int(const char*);
	static double string2Double(string str);
};



