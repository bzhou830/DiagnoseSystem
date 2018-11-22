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
		TCHAR szError[512];
		CString strError;
		e->GetErrorMessage(szError, 512);
		strError.Format("数据类型转换失败,异常原因:\r\n %s",szError);
		MessageBox(NULL, strError, "RBDcm提示您", MB_OK);
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

double CType::string2Double(string str)
{
	return atof(str.c_str());
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
		TCHAR szError[512];
		CString strError;
		e->GetErrorMessage(szError, 512);
		strError.Format("数据类型转换失败,异常原因:\r\n %s",szError);
		MessageBox(NULL, strError, "RBDcm提示您", MB_OK);
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
		TCHAR szError[512];
		CString strError;
		e->GetErrorMessage(szError, 512);
		strError.Format("数据类型转换失败,异常原因:\r\n %s",szError);
		MessageBox(NULL, strError, "RBDcm提示您", MB_OK);
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
		TCHAR szError[512];
		CString strError;
		e->GetErrorMessage(szError, 512);
		strError.Format("数据类型转换失败,异常原因:\r\n %s",szError);
		MessageBox(NULL, strError, "RBDcm提示您", MB_OK);
		return 0;
	}
	return dbGetInt;
}
