
// RBDcm.h : RBDcm 应用程序的主头文件
//
#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"       // 主符号


// CRBDcmApp:
// 有关此类的实现，请参阅 RBDcm.cpp

typedef BOOL (__stdcall *funRunHook)( HMODULE hModule, DWORD dwThreadID);
typedef BOOL (__stdcall *funStopHook)();

class CRBDcmApp : public CWinApp
{
public:
	CRBDcmApp();
	funRunHook RunHook ;
	funStopHook StopHook;
	HMODULE hMod;
// 重写
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// 实现
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CRBDcmApp theApp;


