// LoadDlg.cpp : 实现文件
#include "stdafx.h"
#include "LoadDlg.h"
#include "afxdialogex.h"


// CLoadDlg 对话框
IMPLEMENT_DYNAMIC(CLoadDlg, CDialog)

CLoadDlg::CLoadDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLoadDlg::IDD, pParent)
{
}

CLoadDlg::~CLoadDlg()
{
}

void CLoadDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROGRESS_BAR, m_Progress);
	DDX_Control(pDX, IDC_STATIC_TEXT, m_txtSc);
}


BEGIN_MESSAGE_MAP(CLoadDlg, CDialog)
END_MESSAGE_MAP()


// CLoadDlg 消息处理程序
BOOL CLoadDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	ModifyStyleEx(WS_EX_APPWINDOW,WS_EX_TOOLWINDOW);							//不显示在任务栏
	//m_Progress.SetRange(0,1000);
	//m_Progress.SetStep(1);
	//m_Progress.StepIt();
	m_Progress.SendMessage(PBM_SETBKCOLOR, 0, RGB(0, 0, 0));
	m_Progress.SendMessage(PBM_SETBARCOLOR, 0, RGB(166, 219, 247));

	return TRUE;  
}

void CLoadDlg::SetProcess(int cur)
{
	m_Progress.SetPos(cur);
}

void CLoadDlg::SetSum(int sum)
{
	m_Progress.SetRange(0, sum);
}

void CLoadDlg::SetText(CString str)
{
	m_txtSc.SetWindowText(str);
}
