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
	ON_COMMAND(ID_BTN_RESET, &CMainFrame::OnBtnReset)
	ON_COMMAND(ID_BTN_ZOOM, &CMainFrame::OnBtnZoom)
	ON_COMMAND(ID_BTN_SRCIMG, &CMainFrame::OnBtnSrcimg)
	ON_COMMAND(ID_BTN_PLAYIMGS, &CMainFrame::OnBtnPlayimgs)
	ON_COMMAND(ID_BTN_NEXTIMG, &CMainFrame::OnBtnNextimg)
	ON_COMMAND(ID_BTN_LOADXML, &CMainFrame::OnBtnLoadxml)
	ON_COMMAND(ID_BTN_LASTIMG, &CMainFrame::OnBtnLastimg)
	ON_COMMAND(ID_BTN_HISTIMG, &CMainFrame::OnBtnHistimg)
	ON_COMMAND(ID_BTN_CAMERA, &CMainFrame::OnBtnCamera)
	ON_COMMAND(ID_SERILE_OPEN, &CMainFrame::OnSerileOpen)
	ON_COMMAND(ID_FILE_OPEN_ONE, &CMainFrame::OnFileOpenOne)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,				// 状态行指示器
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

// CMainFrame 构造/析构
CMainFrame::CMainFrame(): m_isLeftPaneMin(FALSE)
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
	//创建状态栏
	if (!m_wndStatusBar.Create(this))						
	{
		TRACE0("未能创建状态栏\n");
		return -1;      // 未能创建
	}
	m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT));
	m_wndStatusBar.SetPaneInfo(1,ID_INDICATOR_CAPS, SBPS_NORMAL, 50);
	m_wndStatusBar.SetPaneInfo(2,ID_INDICATOR_NUM, SBPS_NORMAL, 100);
	m_wndStatusBar.SetPaneInfo(3,ID_INDICATOR_SCRL, SBPS_NORMAL, 100);
	//创建工具栏
	if (!m_toolBar.CreateEx(this,TBSTYLE_FLAT, WS_CHILD|WS_VISIBLE|CBRS_LEFT
		|CBRS_TOOLTIPS|CBRS_FLYBY|CBRS_SIZE_DYNAMIC)
		|| !m_toolBar.LoadToolBar(IDR_TOOLBAR1))
	{
		return -1;
	}
	m_toolBar.EnableDocking(CBRS_ALIGN_ANY);		//工具栏可以停靠
	EnableDocking(CBRS_ALIGN_ANY);					//窗口支持停靠
	DockControlBar(&m_toolBar);						//停靠工具栏

	PostMessage(UM_PROSSESS);						//发送消息，在状态栏上创建工具栏
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
	int nRcWidth = rct.Width();
	int nRcHeight = rct.Height();
	nRcHeight -= 60;							//标题，状态栏预留

	//右侧绑定显示窗口
	if (!m_splt2.CreateStatic(&m_splt, 2, 2, WS_CHILD|WS_VISIBLE, m_splt.IdFromRowCol(0, 0)))
	{
		AfxMessageBox("窗口分割失败！");
		return FALSE;
	}
	nRcWidth = (nRcWidth-200)/2;
	nRcHeight = nRcHeight/2;
	m_splt2.CreateView(0, 0, RUNTIME_CLASS(CRBDcmView), CSize(nRcWidth, nRcHeight), pContext);
	m_pRBView = (CRBDcmView*)m_splt2.GetPane(0,0);
	m_splt2.CreateView(0, 1, RUNTIME_CLASS(CSegView), CSize(nRcWidth, nRcHeight), pContext);
	m_pSegView = (CSegView*)m_splt2.GetPane(0,1);
	m_splt2.CreateView(1, 0, RUNTIME_CLASS(CClassifierView), CSize(nRcWidth, nRcHeight), pContext);
	m_pClassier = (CClassifierView*)m_splt2.GetPane(1,0);
	m_splt2.CreateView(1, 1, RUNTIME_CLASS(CInfoView), CSize(nRcWidth, nRcHeight), pContext);
	m_pInfoView = (CInfoView*)m_splt2.GetPane(1,1);
	
	m_splt.CreateView(0, 1, RUNTIME_CLASS(COprtView), CSize(200, nRcHeight), pContext);
	m_pOpt = (COprtView*)m_splt.GetPane(0,1);
	
	m_splt.SetColumnInfo(0, rct.Width()-210, 500);
	m_splt.SetColumnInfo(1,  200, 100);
	
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
	if (m_isLeftPaneMin)
		nRcWidth = nRcWidth;
	else
		nRcWidth = (nRcWidth-200);
	switch (num)
	{
	case 0:
		m_splt2.SetColumnInfo(0,nRcWidth,10);
		m_splt2.SetRowInfo(0,nRcHeight,10);
		break;
	case 1:
		m_splt2.SetColumnInfo(1,nRcWidth,10);
		m_splt2.SetRowInfo(0,nRcHeight,10);
		break;
	case 2:
		m_splt2.SetColumnInfo(0,nRcWidth,10);
		m_splt2.SetRowInfo(1,nRcHeight,10);
		break;
	case 3:
		m_splt2.SetColumnInfo(0,nRcWidth,10);
		m_splt2.SetRowInfo(1,nRcHeight,10);
		break;
	default:
		break;
	}
	m_splt2.RecalcLayout();				//修改视窗后要刷新
	m_pRBView->Invalidate(FALSE);
	m_pSegView->Invalidate(FALSE);
	m_pClassier->Invalidate(FALSE);
	m_pInfoView->Invalidate(FALSE);
}


/***************************************************************************************
Function:    显示视窗回到原来位置
Input:		 void
Output:   
Description: 
Return:		 void
Others:		 NULL
***************************************************************************************/
int CMainFrame::ResetFrm()
{
	CRect rct;
	GetClientRect(&rct);						//获取客户区的区域大小
	int nRcWidth = rct.Width();			
	int nRcHeight = rct.Height();
	nRcHeight -= 60;							//去掉标题，菜单，状态栏
	if (m_isLeftPaneMin == TRUE)				//不显示左边面板		
		nRcWidth = nRcWidth/2;					//
	else										//显示左边面板
		nRcWidth = (nRcWidth-200)/2;			//
	m_splt2.SetColumnInfo(0, nRcWidth, 10);		//
	m_splt2.SetRowInfo(0, nRcHeight / 2, 10);	//
	m_splt2.RecalcLayout();
	m_pRBView->Invalidate(FALSE);
	m_pSegView->Invalidate(FALSE);
	m_pClassier->Invalidate(FALSE);
	m_pInfoView->Invalidate(FALSE);
	return 0;
}


BOOL CMainFrame::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	return CFrameWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}


void CMainFrame::OnBtnReset()
{
	// TODO: 在此添加命令处理程序代码
}


void CMainFrame::OnBtnZoom()
{
	// TODO: 在此添加命令处理程序代码
}


void CMainFrame::OnBtnSrcimg()
{
	CSrcShowDlg dlg;
	dlg.SetImgData(m_ImgSerial.GetCurrentMatImg().pixle.clone());
	dlg.DoModal();
	dlg.CenterWindow();
}


void CMainFrame::OnBtnPlayimgs()
{
	if (m_PlayDlg != NULL)			//若对象已经存在内存中，则显示就可以了，避免创建多个对象
	{
		m_PlayDlg->SetImgSerial(m_ImgSerial.GetBegin(), m_ImgSerial.GetImageNum());
		m_PlayDlg->ShowWindow(SW_NORMAL);
		return;
	}
	m_PlayDlg = new CPlaySeriesDlg();
	CRBDcmView *pView = ((CMainFrame*)AfxGetMainWnd())->m_pRBView;
	m_PlayDlg->Create(MAKEINTRESOURCE(IDD_PLAY_SERI), pView);
	m_PlayDlg->SetImgSerial(m_ImgSerial.GetBegin(), m_ImgSerial.GetImageNum());
	m_PlayDlg->ShowWindow(SW_NORMAL);
}


void CMainFrame::OnBtnNextimg()
{
	sOneImg img = m_ImgSerial.NextMatImg();
	m_pRBView->SetImgData(img);
	m_pInfoView->SetImgData(img);
	//SetCurrent(m_ImgSerial.GetCurrentNum());
}

void CMainFrame::OnBtnLoadxml()
{
	CFileDialog dlg(TRUE);								//文件打开对话框
	dlg.m_ofn.lpstrTitle = "打开Xml文件";				//指定打开文件对话框名称
	//文件过滤器
	dlg.m_ofn.lpstrFilter = "xml Files(*.xml)\0*.xml\0All Files(*.*)\0*.*\0\0";  
	if (IDCANCEL == dlg.DoModal())						//弹出文件打开对话框，选择取消则直接返回
		return;
	CString strFileName = dlg.GetPathName();			//获取文件路径+文件名
	m_ImgSerial.LoadXml(strFileName);					//加载xml文件
	sOneImg info = m_ImgSerial.GetCurrentMatImg();
	m_pInfoView->SetImgData(info);						//向信息显示窗口传入结节的信息
}


void CMainFrame::OnBtnLastimg()
{
	sOneImg img = m_ImgSerial.LastMatImg();
	m_pRBView->SetImgData(img);
	m_pInfoView->SetImgData(img);
	//SetCurrent(m_ImgSerial.GetCurrentNum());
}


void CMainFrame::OnBtnHistimg()
{
	// TODO: 在此添加命令处理程序代码
}


void CMainFrame::OnBtnCamera()
{
	// TODO: 在此添加命令处理程序代码
}





void CMainFrame::OnSerileOpen()
{
	AfxMessageBox("打开图像序列");
	CFileDialog dlg(TRUE);								//文件打开对话框
	CFileFind ff;										//文件查找类
	CStringArray dcmPathstr;							//文件路径+文件名列表 dcm文件
	dlg.m_ofn.lpstrTitle = "打开图像序列";				//指定打开文件对话框名称
	dlg.m_ofn.lpstrFilter = "Dcm Files(*.dcm)\0*.dcm\0All Files(*.*)\0*.*\0\0";  //文件过滤器

	if (IDCANCEL == dlg.DoModal())						//弹出文件打开对话框，选择取消则直接返回
		return;

	CString strFileName = dlg.GetPathName();			//获取文件路径+文件名
	int end = strFileName.ReverseFind('\\');			//逆向查找
	CString strFilePath = strFileName.Left(end);		//去掉最后的文件名得到文件目录

	if(!ff.FindFile(strFilePath + "\\*.dcm"))			//查找目录下是否存在dcm文件，不存在则退出
		return;

	while(ff.FindNextFile())							//存在dcm文件，将其文件完整路径存入dcmPathstr
		dcmPathstr.Add(strFilePath + "\\" + ff.GetFileName());

	m_ImgSerial.Clear();								//导入数据前先清除
	CString strXml = NULL;								//文件路径
	for (int i=0; i<dcmPathstr.GetSize(); ++i)		
	{
		strFileName = dcmPathstr.GetAt(i);				//从序列中取得文件路径
		m_ImgSerial.LoadDcm(strFileName);				//加载Dcm文件
	}
	//SetSum(m_ImgSerial.GetImageNum());					//显示当前图像总数
	//SetCurrent(m_ImgSerial.GetCurrentNum());			//显示当前图像序号
	sOneImg info = m_ImgSerial.GetCurrentMatImg();		//取得当前图像
	m_pRBView->SetImgData(info);			//将图像设置到CRBDcm类中
}


void CMainFrame::OnFileOpenOne()
{
	CFileDialog dlg(TRUE);								//文件打开对话框
	dlg.m_ofn.lpstrTitle = "打开单张图像";				//指定打开文件对话框名称
	//文件过滤器
	dlg.m_ofn.lpstrFilter = "Dcm Files(*.dcm)\0*.dcm\0All Files(*.*)\0*.*\0\0";		
	if (IDCANCEL == dlg.DoModal())						//弹出文件打开对话框，选择取消则直接返回
		return;
	CString strFileName = dlg.GetPathName();			//获取文件路径+文件名
	m_ImgSerial.Clear();								//清除原有数据
	m_ImgSerial.LoadDcm(strFileName);					//加载数据
	//SetSum(m_ImgSerial.GetImageNum());				//显示当前图像总数
	//SetCurrent(m_ImgSerial.GetCurrentNum());			//显示当前图像序号
	sOneImg info = m_ImgSerial.GetCurrentMatImg();		//序列中当前图像
	m_pRBView->SetImgData(info);			//设置当前图像
}
