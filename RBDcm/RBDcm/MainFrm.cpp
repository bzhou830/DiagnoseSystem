/**********************************************************************************
  Copyright @ Robin 2015-2025
  File name:  MainFrm.cpp
  Author: Robin   Version: V1.0   Date: 2015.11.05 
  Description: application framework class 
               MFC structure
  Others:         
***********************************************************************************/
#include "stdafx.h"
#include "RBDcm.h"
#include "MainFrm.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame
IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_MESSAGE(UM_PROSSESS, OnProgress)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,				// 状态行指示器
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

// CMainFrame 构造/析构
CMainFrame::CMainFrame()
{
}

CMainFrame::~CMainFrame()
{
}

/***************************************************************************************
Function: 程序框架创建消息响应
Input:		void
Output:   
Description: 
Return:		void
Others:		NULL
***************************************************************************************/
int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (!m_wndStatusBar.Create(this))
	{
		TRACE0("未能创建状态栏\n");
		return -1;      // 未能创建
	}
	m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT));
	m_wndStatusBar.SetPaneInfo(1,ID_INDICATOR_CAPS, SBPS_NORMAL, 50);
	m_wndStatusBar.SetPaneInfo(2,ID_INDICATOR_NUM, SBPS_NORMAL, 100);
	m_wndStatusBar.SetPaneInfo(3,ID_INDICATOR_SCRL, SBPS_NORMAL, 100);
	PostMessage(UM_PROSSESS);
	return 0;
}


/***************************************************************************************
Function: 程序框架创建前消息响应
Input:		void
Output:   
Description: 在此类中修改程序框架样式
Return:		void
Others:		NULL
***************************************************************************************/
BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// CREATESTRUCT cs 来修改窗口类或样式
	cs.style = WS_OVERLAPPED | WS_CAPTION | FWS_ADDTOTITLE
		 | WS_MINIMIZEBOX | WS_SYSMENU;
	cs.lpszName = "RBDcm计算机辅助诊断系统";
	cs.style &= ~FWS_ADDTOTITLE;				//去掉后面显示的文件名字

	cs.cy = ::GetSystemMetrics(SM_CYSCREEN);	//获取整个屏幕的大小
	cs.cx = ::GetSystemMetrics(SM_CXSCREEN);
	cs.y = 0;									//程序界面调到整个屏幕大小
	cs.x = 0;
	return TRUE;
}

// CMainFrame 诊断
#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}
#endif //_DEBUG


/***************************************************************************************
Function:    程序框架中创建客户区 消息响应
Input:		 void
Output:   
Description: 在此类中分割窗口
Return:		 void
Others:		 NULL
***************************************************************************************/
BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext)
{
	if( !m_splt.CreateStatic(this, 1, 2) )		 //分割为1行2列
	{
		AfxMessageBox("窗口分割失败！");
		return FALSE;
	}
	CRect rct;
	GetWindowRect(&rct);
	//GetClientRect(&rct);
	int nRcWidth = rct.Width();
	int nRcHeight = rct.Height();
	//左侧绑定操作面板
	m_splt.CreateView(0, 0, RUNTIME_CLASS(COprtView), CSize(nRcWidth / 7, nRcHeight), pContext);
	m_pOpt = (COprtView*)m_splt.GetPane(0,0);
	//右侧绑定显示窗口
	if (!m_splt2.CreateStatic(&m_splt, 2,2,WS_CHILD|WS_VISIBLE,m_splt.IdFromRowCol(0, 1)))
	{
		AfxMessageBox("窗口分割失败！");
		return FALSE;
	}
	m_splt2.CreateView(0, 0, RUNTIME_CLASS(CRBDcmView), CSize(nRcWidth /7 * 3, nRcHeight/2), pContext);
	m_pRBView = (CRBDcmView*)m_splt2.GetPane(0,0);
	m_splt2.CreateView(0, 1, RUNTIME_CLASS(CSegView), CSize(nRcWidth /7 * 3, nRcHeight/2), pContext);
	m_pSegView = (CSegView*)m_splt2.GetPane(0,1);
	m_splt2.CreateView(1, 0, RUNTIME_CLASS(CClassifierView), CSize(nRcWidth /7 * 3, nRcHeight/2), pContext);
	m_pClassier = (CClassifierView*)m_splt2.GetPane(1,0);
	m_splt2.CreateView(1, 1, RUNTIME_CLASS(CInfoView), CSize(nRcWidth /7 * 3, nRcHeight/2), pContext);
	m_pInfoView = (CInfoView*)m_splt2.GetPane(1,1);
	return TRUE;
}


//禁用窗口大小变动
//tips: 发送给任意子窗口的消息都会在此路由，
// 所以要判断句柄，以免对子窗口拖动造成影响.
BOOL CMainFrame::PreTranslateMessage(MSG* pMsg)
{
	//截获鼠标在标题栏双击消息
	HWND hWnd = AfxGetMainWnd()->m_hWnd;
	if(pMsg->message == WM_NCLBUTTONDBLCLK && pMsg->wParam == HTCAPTION && pMsg->hwnd == hWnd)
		return TRUE;
	//截获鼠标在标题栏拖动消息
	if (pMsg->message == WM_NCLBUTTONDOWN  && pMsg->wParam == HTCAPTION && pMsg->hwnd == hWnd)
		return TRUE;
	return CFrameWnd::PreTranslateMessage(pMsg);
}


/***************************************************************************************
Function:    自定义消息处理函数
Input:		 void
Output:   
Description: 在此类中向状态栏添加滑块
Return:		 void
Others:		 NULL
***************************************************************************************/
LRESULT CMainFrame::OnProgress(WPARAM, LPARAM)
{
	CRect rect;
	m_wndStatusBar.GetItemRect(3, &rect);  //获取要显示区域窗体的大小
	//适当缩小窗体，便于边缘不被遮盖
	rect.top += 2;
	rect.bottom -= 2;
	rect.left += 2;
	rect.right -= 2;
	//向状态栏创建一个滑块控件
	if (!m_slider.Create(TBS_HORZ|WS_CHILD|WS_VISIBLE, rect, &m_wndStatusBar, 123))
	{
		AfxMessageBox("滑块创建失败");
		return -1;
	}
	return 0;
}


/***************************************************************************************
Function:    设置某一个视窗为当前最大视窗，其他视窗则隐藏
Input:		 void
Output:   
Description: 
Return:		 void
Others:		 NULL
***************************************************************************************/
void CMainFrame::SetMainFrm(int num)
{
	CRect rct;
	GetClientRect(&rct);
	int nRcWidth = rct.Width();
	int nRcHeight = rct.Height();
	switch (num)
	{
	case 0:
		m_splt2.SetColumnInfo(0,nRcWidth/7 * 6,10);
		m_splt2.SetRowInfo(0,nRcHeight,10);
		break;
	case 1:
		m_splt2.SetColumnInfo(1,nRcWidth/7 * 6,10);
		m_splt2.SetRowInfo(0,nRcHeight,10);
		break;
	case 2:
		m_splt2.SetColumnInfo(0,nRcWidth/7 * 6,10);
		m_splt2.SetRowInfo(1,nRcHeight,10);
		break;
	case 3:
		m_splt2.SetColumnInfo(0,nRcWidth/7 * 6,10);
		m_splt2.SetRowInfo(1,nRcHeight,10);
		break;
	default:
		break;
	}
	m_splt2.RecalcLayout();				//修改视窗后要刷新
}


/***************************************************************************************
Function:    显示视窗回到原来位置
Input:		 void
Output:   
Description: 在此类中向状态栏添加滑块
Return:		 void
Others:		 NULL
***************************************************************************************/
int CMainFrame::ResetFrm(void)
{
	CRect rct;
	GetClientRect(&rct);
	int nRcWidth = rct.Width();
	int nRcHeight = rct.Height();
	m_splt2.SetColumnInfo(0, nRcWidth / 7 * 3, 10);
	m_splt2.SetRowInfo(0, nRcHeight / 2, 10);
	m_splt2.RecalcLayout();
	return 0;
}
