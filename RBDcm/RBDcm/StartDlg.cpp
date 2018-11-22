// StartDlg.cpp : 实现文件
#include "stdafx.h"
#include "StartDlg.h"
#include "afxdialogex.h"

// CStartDlg 对话框
IMPLEMENT_DYNAMIC(CStartDlg, CDialogEx)

CStartDlg::CStartDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CStartDlg::IDD, pParent)
{
}


CStartDlg::~CStartDlg()
{
}


void CStartDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CStartDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CStartDlg 消息处理程序
BOOL CStartDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	//获得窗口预设的大小
	CRect dlgRect;
	GetWindowRect(dlgRect);
	CRect desktopRect;
	//将窗口开始大小设为0
	GetDesktopWindow()->GetWindowRect(desktopRect);
	MoveWindow((desktopRect.Width() - dlgRect.Width())/2,
				(desktopRect.Height() - dlgRect.Height())/2,
				0,0);
	//初始化变化大小
	m_nWidth = dlgRect.Width();
	m_nHeight = dlgRect.Height();
	m_nDx = 3;
	m_nDy = 2;
	m_nDx1 = 2;
	m_nDy1 = 2;
	SetTimer(1, 10, NULL);			//设定定时器1	
	return TRUE;
}


void CStartDlg::OnPaint()
{
	CPaintDC dc(this); 
	dc.BitBlt(0,0,m_MemDc.Width(),m_MemDc.Height(),&m_MemDc,0,0,SRCCOPY);
}


void CStartDlg::OnTimer(UINT_PTR nIDEvent)
{
	CRect dlgRect;
	GetWindowRect(dlgRect);
	//获得桌面的大小
	CRect desktopRect;
	GetDesktopWindow()->GetWindowRect(desktopRect);
	//如果是窗口弹出过程，则逐渐增大窗口
	if(nIDEvent == 1)
	{
		MoveWindow(
			(-m_nDx+desktopRect.Width() - dlgRect.Width()) / 2,
			(-m_nDy+desktopRect.Height() - dlgRect.Height()) / 2, 
			+m_nDx+dlgRect.Width(),
			+m_nDy+dlgRect.Height());

		//不要超过窗口预设的宽度
		if(dlgRect.Width() >= m_nWidth) 
			m_nDx=0; 
		//不要超过窗口预设的高度
		if(dlgRect.Height() >= m_nHeight)
			m_nDy=0;
		//停止变化，关闭定时器1
		if((dlgRect.Width() >= m_nWidth) && (dlgRect.Height() >= m_nHeight))
			KillTimer(1); 
	}

	//停止变化，关闭定时器1
	if((dlgRect.Width() >= m_nWidth) && (dlgRect.Height() >= m_nHeight))
	{
		KillTimer(1);
		SetTimer(3,1000,NULL);
	}

	//如果是窗口关闭过程，则逐渐缩小窗口
	if(nIDEvent == 2)
	{
		MoveWindow((+m_nDx+desktopRect.Width() - dlgRect.Width()) / 2,
			(+m_nDy+desktopRect.Height() - dlgRect.Height()) / 2,
			-m_nDx1+dlgRect.Width(),
			-m_nDy1+dlgRect.Height() );
		//当宽度等于零后宽度就不在变化
		if(dlgRect.Width() <= 0) 
			m_nDx1=0;
		//当高度等于零后高度就不在变化
		if(dlgRect.Height() <= 0 )
			m_nDy1=0; 
		//停止变化，关闭定时器2，并且关闭窗口
		if((dlgRect.Width() <= 0 ) && (dlgRect.Height() <=0))
		{
			KillTimer(2);
			CDialog::OnOK();
		}
	}
	if(nIDEvent == 3)
	{
		static int n=0;
		if(n==2)
			OnOK();
		n++;
	}
	CDialogEx::OnTimer(nIDEvent);
}
