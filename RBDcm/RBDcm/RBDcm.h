
// RBDcm.h : RBDcm Ӧ�ó������ͷ�ļ�
//
#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"       // ������


// CRBDcmApp:
// �йش����ʵ�֣������ RBDcm.cpp

typedef BOOL (__stdcall *funRunHook)( HMODULE hModule, DWORD dwThreadID);
typedef BOOL (__stdcall *funStopHook)();

class CRBDcmApp : public CWinApp
{
public:
	CRBDcmApp();
	funRunHook RunHook ;
	funStopHook StopHook;
	HMODULE hMod;
// ��д
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// ʵ��
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CRBDcmApp theApp;


