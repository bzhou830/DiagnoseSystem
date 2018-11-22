#include "stdafx.h"
#include "LoginDlgDll.h"

int ShowLoginDlg(CString& szName,CString& szPass)
{
	int state = -1;
	HMODULE hM = LoadLibrary("Skindll.dll");
	funShowDlg Dlg;
	if(hM)
	{
		Dlg = (funShowDlg)GetProcAddress(hM, "ShowDlg");
		if (Dlg)
			state = Dlg(szName,szPass);
	}
	FreeLibrary(hM);
	return state;
}