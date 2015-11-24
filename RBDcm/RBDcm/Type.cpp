#include "StdAfx.h"
#include "Type.h"


CType::CType(void)
{
}


CType::~CType(void)
{
}

int CType::CString2int(CString str)
{
	int nGetInt = 0;
	try
	{
		nGetInt = _tstoi( LPCTSTR(str));
	}
	catch (CException* e)
	{
		MessageBox(NULL, "数据类型转换失败,异常原因:\r\n", "RBDcm提示您", MB_OK);
		return 0;
	}
	return nGetInt;
}


CString CType::int2CString(int num)
{
	CString str;
	str.Format("%d",num);
	return str;
}

double CType::CString2Double(CString str)
{
	double dbGetDouble = 0.0;
	try
	{
		dbGetDouble = _tstof( LPCTSTR(str));
	}
	catch (CException* e)
	{
		MessageBox(NULL, "数据类型转换失败", "RBDcm提示您", MB_OK);
		return 0;
	}
	return dbGetDouble;
}



CString CType::double2CString(double num)
{
	CString str;
	str.Format("%.3lf",num);
	return str;
}


double CType::pChar2Double(const char* pChar)
{
	double dbGetDouble = 0.0;
	try
	{
		dbGetDouble = _tstof( LPCTSTR(pChar));
	}
	catch (CException* e)
	{
		MessageBox(NULL, "数据类型转换失败", "RBDcm提示您", MB_OK);
		return 0;
	}
	return dbGetDouble;
}


int CType::pChar2int(const char* pChar)
{
	int dbGetInt = 0;
	try
	{
		dbGetInt = _tstoi( LPCTSTR(pChar));
	}
	catch (CException* e)
	{
		MessageBox(NULL, "数据类型转换失败", "RBDcm提示您", MB_OK);
		return 0;
	}
	return dbGetInt;
}
