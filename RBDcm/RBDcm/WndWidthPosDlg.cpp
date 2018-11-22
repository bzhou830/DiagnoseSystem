// WndWidthPosDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "WndWidthPosDlg.h"
#include "afxdialogex.h"


// CWndWidthPosDlg 对话框

IMPLEMENT_DYNAMIC(CWndWidthPosDlg, CDialog)

CWndWidthPosDlg::CWndWidthPosDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CWndWidthPosDlg::IDD, pParent)
{

}

CWndWidthPosDlg::~CWndWidthPosDlg()
{
}

void CWndWidthPosDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CWndWidthPosDlg, CDialog)
END_MESSAGE_MAP()


// CWndWidthPosDlg 消息处理程序
