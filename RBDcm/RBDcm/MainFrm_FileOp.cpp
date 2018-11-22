#include "stdafx.h"
#include "MainFrm.h"

void CMainFrame::OnToolbarFileopen()
{
	OnSerileOpen();
}

void CMainFrame::OnToolbarFileprint()
{
}

void CMainFrame::OnToolbarFilesave()
{
	CSaveImgDlg dlg(m_ImgSerial.m_vImgSerial);
	if (IDCANCEL == dlg.DoModal())
		return;
}

void CMainFrame::OnToolbarFilesaveas()
{
	CSaveImgDlg dlg(m_ImgSerial.m_vImgSerial);
	if (IDCANCEL == dlg.DoModal())
		return;
}

