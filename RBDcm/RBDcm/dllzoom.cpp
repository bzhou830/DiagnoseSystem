#include "stdafx.h"
#include "dllzoom.h"

void ShowZoom()
{
	HMODULE hM = LoadLibrary("ZoomIn.dll");
	funZoomDlg Dlg;
	if(hM)
	{
		Dlg = (funZoomDlg)GetProcAddress(hM, "ShowZoomDlg");
		if (Dlg)
			Dlg();
	}
	FreeLibrary(hM);
}