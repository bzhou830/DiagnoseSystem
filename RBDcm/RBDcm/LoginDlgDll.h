#ifndef __LOGINDLGDLL_H__
#define __LOGINDLGDLL_H__
#include "stdafx.h"

typedef int (_stdcall *funShowDlg)(CString &szName,CString& szPass);

int ShowLoginDlg(CString &szName,CString& szPass);



#endif