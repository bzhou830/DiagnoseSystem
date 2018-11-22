/*================================================================================*
* Copyright @ Robin 2015 - 2025
* File name:  MainFrm.cpp
* Author: Robin   
* Version: V2.0   
* Date: 2015.11.05 
*=================================================================================*/

#include "stdafx.h"
#include "RBDcm.h"
#include "Global.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame
IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_MESSAGE(UM_PROSSESS, OnProgress)
	ON_COMMAND(ID_BTN_RESET, &CMainFrame::OnBtnAIP)
	ON_COMMAND(ID_BTN_ZOOM, &CMainFrame::OnBtnZoom)
	ON_COMMAND(ID_BTN_SRCIMG, &CMainFrame::OnBtnSrcimg)
	ON_COMMAND(ID_BTN_PLAYIMGS, &CMainFrame::OnBtnPlayimgs)
	ON_COMMAND(ID_BTN_NEXTIMG, &CMainFrame::OnBtnExtract)
	ON_COMMAND(ID_BTN_LOADXML, &CMainFrame::OnBtnLoadxml)
	ON_COMMAND(ID_BTN_LASTIMG, &CMainFrame::OnBtnGMM)
	ON_COMMAND(ID_BTN_HISTIMG, &CMainFrame::OnBtnHistimg)
	ON_COMMAND(ID_BTN_CAMERA, &CMainFrame::OnBtnCamera)
	ON_COMMAND(ID_SERILE_OPEN, &CMainFrame::OnSerileOpen)
	ON_COMMAND(ID_FILE_OPEN_ONE, &CMainFrame::OnFileOpenOne)
	ON_COMMAND(ID_BTN_ENTROPY, &CMainFrame::OnBtnEntropy)
	ON_COMMAND(ID_BTN_OTSU, &CMainFrame::OnBtnOtsu)
	ON_COMMAND(ID_BTN_REALLUNG, &CMainFrame::OnBtnVTK)
	ON_COMMAND(ID_BTN_NUDOUL, &CMainFrame::OnBtnNudoul)
	ON_COMMAND(ID_BTN_MOM, &CMainFrame::OnBtnMom)
	ON_COMMAND(ID_BTN_MEAN, &CMainFrame::OnBtnMean)
	ON_COMMAND(IDM_SAVEIMG, &CMainFrame::OnSaveimg)
	ON_COMMAND(ID_BTN_TEST, &CMainFrame::OnBtnTest)
	ON_COMMAND(IDM_TRAINING, &CMainFrame::OnTraining)
	ON_COMMAND(IDM_WINDOW, &CMainFrame::OnWindow)
	ON_COMMAND(IDM_SAVESERIAL, &CMainFrame::OnSaveserial)
	ON_COMMAND(ID_TEST, &CMainFrame::OnTest)
	ON_COMMAND(IDM_DISPLAY_NOTE, &CMainFrame::OnDisplayNote)
	ON_UPDATE_COMMAND_UI(IDM_DISPLAY_NOTE, &CMainFrame::OnUpdateDisplayNote)
	ON_COMMAND(ID_BTN_CLICK, &CMainFrame::OnBtnClick)
	ON_UPDATE_COMMAND_UI(ID_BTN_CLICK, &CMainFrame::OnUpdateBtnClick)
	ON_COMMAND(ID_TOOLBAR_FILEOPEN, &CMainFrame::OnToolbarFileopen)
	ON_COMMAND(ID_TOOLBAR_FILEPRINT, &CMainFrame::OnToolbarFileprint)
	ON_COMMAND(ID_TOOLBAR_FILESAVE, &CMainFrame::OnToolbarFilesave)
	ON_COMMAND(ID_TOOLBAR_FILESAVEAS, &CMainFrame::OnToolbarFilesaveas)
//	ON_WM_SIZE()
ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

//状态栏指示器
static UINT indicators[] =
{
	ID_SEPARATOR,									//状态行指示器
	IDS_ALLIMGNUM,
	IDS_CURRENTIMGNUM,
	ID_INDICATOR_NUM,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

HANDLE CMainFrame::m_hMutex = NULL;
int CMainFrame::m_LoadStatus = FALSE;

/*================================================================================*
* 主框架程序的构造函数
* 对成员变量进行值初始化，特别是指针成员
*=================================================================================*/
CMainFrame::CMainFrame()
	: m_isLeftPaneMin(FALSE)
	, m_isDisplayNote(false)
	, m_bCanClick(false)
{
	m_PlayDlg = NULL;
	m_AipDlg = NULL;
}

/*================================================================================*
* 主框架程序的析构函数
* 在此函数中对动态分配的对象进行检查，防止内存泄漏
*=================================================================================*/
CMainFrame::~CMainFrame()
{
	if (m_PlayDlg != NULL)
	{
		delete m_PlayDlg;
		m_PlayDlg = NULL;
	}
	if (m_AipDlg != NULL)
	{
		delete m_AipDlg;
		m_AipDlg = NULL;
	}
}

/*================================================================================*
* Function: 程序框架创建前消息响应
* Description: 在此类中修改程序框架样式
*=================================================================================*/
BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// CREATESTRUCT cs 来修改窗口类或样式
	cs.style = WS_OVERLAPPED | WS_CAPTION | FWS_ADDTOTITLE
		 | WS_MINIMIZEBOX | WS_SYSMENU;
	cs.lpszName = "RBDcm计算机辅助诊断系统";
	cs.style &= ~FWS_ADDTOTITLE;												//去掉后面显示的文件名字

	//获取整个屏幕的大小
	cs.cx = ::GetSystemMetrics(SM_CXSCREEN);
	CRect rect;
	SystemParametersInfo(SPI_GETWORKAREA, 0, &rect, 0);
	cs.cy = rect.bottom;
	//cs.cy = ::GetSystemMetrics(SM_CYSCREEN);
	cs.y = 0;																	//程序界面调到整个屏幕大小
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

/*================================================================================*
* Function:    程序框架中创建客户区 消息响应
* Description: 在此类中分割窗口
*=================================================================================*/
BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext)
{
	if( !m_splt.CreateStatic(this, 1, 2) )										//分割为1行2列
	{
		AfxMessageBox("窗口分割失败！");
		return FALSE;
	}
	CRect rct;
	GetWindowRect(&rct);														//获取客户区矩形
	int nRcWidth = rct.Width();													//
	int nRcHeight = rct.Height();												//
	nRcHeight -= 60;															//标题，状态栏一起的大概撒高度为60

	//右侧绑定显示窗口
	if (!m_splt2.CreateStatic(&m_splt, 2, 2, WS_CHILD|WS_VISIBLE, m_splt.IdFromRowCol(0, 0)))
	{
		AfxMessageBox("窗口分割失败！");
		return FALSE;
	}
	nRcWidth = (nRcWidth - INFOWINDOWWIDTH)/2;
	nRcHeight = nRcHeight/2;
	m_splt2.CreateView(0, 0, RUNTIME_CLASS(CLUView), CSize(nRcWidth, nRcHeight), pContext);
	m_pLUView = (CLUView*)m_splt2.GetPane(0,0);
	m_splt2.CreateView(0, 1, RUNTIME_CLASS(CRUView), CSize(nRcWidth, nRcHeight), pContext);
	m_pRUView = (CRUView*)m_splt2.GetPane(0,1);
	m_splt2.CreateView(1, 0, RUNTIME_CLASS(CLDView), CSize(nRcWidth, nRcHeight), pContext);
	m_pLDView = (CLDView*)m_splt2.GetPane(1,0);
	m_splt2.CreateView(1, 1, RUNTIME_CLASS(CLDView), CSize(nRcWidth, nRcHeight), pContext);
	m_pRDView = (CLDView*)m_splt2.GetPane(1,1);
	//m_pInfoView = (CInfoView*)m_splt2.GetPane(1,1);
	
	m_splt.CreateView(0, 1, RUNTIME_CLASS(CInfoView), CSize(INFOWINDOWWIDTH, nRcHeight), pContext);
	m_pInfoView = (CInfoView*)m_splt.GetPane(0,1);

	m_splt.SetColumnInfo(0, rct.Width()-INFOWINDOWWIDTH, 500);
	m_splt.SetColumnInfo(1,  INFOWINDOWWIDTH, 100);
	return TRUE;
}


//禁用窗口大小变动
//tips: 发送给任意子窗口的消息都会在此路由，所以要判断句柄，以免对子窗口拖动造成影响.
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
Description: 在此类中向状态栏添加滑块
***************************************************************************************/
LRESULT CMainFrame::OnProgress(WPARAM, LPARAM)
{
	/*
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
	*/
	return 0;
}



/*===========================================================================*
* 设置某一个视窗为当前最大视窗，其他视窗则隐藏
*============================================================================*/
void CMainFrame::SetMainFrm(int num)
{
	CRect rct;
	GetClientRect(&rct);														//获取整个客户区大小
	int nRcWidth = rct.Width();
	int nRcHeight = rct.Height();
	if (m_isLeftPaneMin)
		nRcWidth = nRcWidth;
	else
		nRcWidth = (nRcWidth-INFOWINDOWWIDTH);
	switch (num)
	{
	case 0:
		m_splt2.SetColumnInfo(0, nRcWidth, 10);									//设置第一列列宽为窗口宽度
		m_splt2.SetRowInfo(0, nRcHeight, 10);									//设置第一行行高为窗口高度
		break;
	case 1:
		m_splt2.SetColumnInfo(0,0,10);											//设置第一列列宽为0
		m_splt2.SetColumnInfo(1,nRcWidth,10);									//设置第二列列宽为窗口宽度
		m_splt2.SetRowInfo(0,nRcHeight,10);										//设置第一行行高为窗口高度
		break;
	case 2:
		m_splt2.SetColumnInfo(0,nRcWidth,10);
		m_splt2.SetRowInfo(0,0,10);
		m_splt2.SetRowInfo(1,nRcHeight,10);
		break;
	case 3:
		m_splt2.SetColumnInfo(1,nRcWidth,10);
		m_splt2.SetRowInfo(1,nRcHeight,10);
		break;
	default:
		break;
	}
	m_splt2.RecalcLayout();														//修改视窗后要刷新
	m_pLUView->Invalidate(FALSE);
	m_pRUView->Invalidate(FALSE);
	m_pLDView->Invalidate(FALSE);
}


/*===========================================================================*
* 显示视窗回到原来位置
*============================================================================*/
int CMainFrame::ResetFrm()
{
	CRect rct;
	GetClientRect(&rct);														//获取客户区的区域大小
	int nRcWidth = rct.Width();			
	int nRcHeight = rct.Height();
	nRcHeight -= 60;															//去掉标题，菜单，状态栏
	if (m_isLeftPaneMin == TRUE)												//不显示左边面板		
		nRcWidth = nRcWidth/2;													//
	else																		//显示左边面板
		nRcWidth = (nRcWidth-INFOWINDOWWIDTH)/2;								//
	m_splt2.SetColumnInfo(0, nRcWidth, 10);										//
	m_splt2.SetRowInfo(0, nRcHeight / 2, 10);									//
	m_splt2.RecalcLayout();
	m_pLUView->Invalidate(FALSE);
	m_pRUView->Invalidate(FALSE);
	m_pLDView->Invalidate(FALSE);
	return 0;
}


/*===========================================================================*
* 打开图像序列时 线程函数 界面显示线程
*============================================================================*/
DWORD WINAPI CMainFrame::FunProc(LPVOID lp)
{
	CLoadDlg* pDlg = new CLoadDlg;
	pDlg->Create(IDD_LOAD_DLG);													//创建进度条对话框
	pDlg->ShowWindow(SW_SHOW);													//显示进度条对话框
	pDlg->SetFocus();															//进度条设置显示在最前端
	pDlg->SetSum(100);															//设置满量程
	pDlg->SetText("序列加载中，稍等...");									//对话框上显示文本
	int m = 0;
	BOOL flag = TRUE;
	BOOL isChange = FALSE;
	while (flag)																//一直读取读入图像数，读完时退出
	{
		m = (m + 1) % 100;
		pDlg->SetProcess(m);													//设置进度条
		
		WaitForSingleObject(m_hMutex, INFINITE);
		if (m_LoadStatus == 1 && isChange == FALSE)
		{
			pDlg->SetText("分割序列中，即将完成...");
			isChange = TRUE;
		}
		else if (m_LoadStatus == 2)
			flag = FALSE;
		ReleaseMutex(m_hMutex);	
		Sleep(50);
	}
	delete pDlg;																//销毁对话框	
	m_LoadStatus = 0;														
	return 0;
}


/*===========================================================================*
* 打开图像序列操作
*============================================================================*/
void CMainFrame::OnSerileOpen()
{
	CFileDialog dlg(TRUE);														//文件打开对话框
	CFileFind ff;																//文件查找类
	CStringArray dcmPathstr;													//文件路径+文件名列表 dcm文件
	dcmPathstr.RemoveAll();
	dlg.m_ofn.lpstrTitle = "打开图像序列";											//指定打开文件对话框名称
	dlg.m_ofn.lpstrFilter = "Dcm Files(*.dcm)\0*.dcm\0All Files(*.*)\0*.*\0\0"; //文件过滤器

	if (IDCANCEL == dlg.DoModal())												//弹出文件打开对话框，选择取消则直接返回
		return;

	CString strFileName = dlg.GetPathName();									//获取文件路径+文件名
	int end = strFileName.ReverseFind('\\');									//逆向查找到分隔符文件夹
	CString strFilePath = strFileName.Left(end);								//去掉最后的文件名得到文件目录
	if(!ff.FindFile(strFilePath + "\\*.dcm"))									//查找目录下是否存在dcm文件，不存在则退出
		return;

	while(ff.FindNextFile())													//存在dcm文件，将其文件完整路径存入dcmPathstr
		dcmPathstr.Add(strFilePath + "\\" + ff.GetFileName());

	m_ImgSerial.Clear();														//导入数据前先清除
	CString strXml = NULL;														//文件路径

	HANDLE hHANDLE = CreateThread(NULL, 0, FunProc, NULL, 0, NULL);				//创建线程用于显示读取进度
	CloseHandle(hHANDLE);														//关闭句柄
	//m_hMutex = CreateMutex(NULL, FALSE, NULL);								//创建互斥量，用于数据同步

	m_ImgSerial.LoadDcms(&dcmPathstr);											//多线程加载
	m_ImgSerial.Merge();														//合并加载内容

	/*
	一张一张加载
	for (int i=0; i<dcmPathstr.GetSize(); ++i)									//依次加载图像
	{
		strFileName = dcmPathstr.GetAt(i);										//从序列中取得文件路径
		m_ImgSerial.LoadDcm(strFileName);										//加载Dcm文件
	}
	*/

	WaitForSingleObject(m_hMutex, INFINITE);
	m_LoadStatus = 1;															//加载完成
	ReleaseMutex(m_hMutex);
	m_ImgSerial.SortImg();														//对图像排序（按照z轴顺序）
	
	fSlice = fabs(m_ImgSerial.GetEnd()->fSliceLocation 
		- m_ImgSerial.GetBegin()->fSliceLocation);								//像素位置
	fSlice = fSlice / m_ImgSerial.GetBegin()->fSliceSpacing;					//计算CT层厚
	int dealtime = m_ImgSerial.SegSeries();										//分割序列图像
	
	m_aipFrameNum = (int)(7.5 / m_ImgSerial.GetBegin()->info.slicesickness);	//AIP层数

	m_ImgSerial.ImgAIP(m_aipFrameNum);											//平均密度投影
	m_ImgSerial.mskAIP(m_aipFrameNum);

	SetStateText(3, "分割用时:" + CType::int2CString(dealtime) + "ms");			//状态栏显示分割用时

	WaitForSingleObject(m_hMutex, INFINITE);
	m_LoadStatus = 2;
	ReleaseMutex(m_hMutex);
	
	sOneImg coronal;															//重建得到矢状面图像
	coronal.masking = m_ImgSerial.GetCoronal(120, static_cast<int>(fSlice));		
	coronal.info = m_ImgSerial.GetBegin()->info;
	m_pRUView->SetImgData(coronal);												//显示矢状面图像

	sOneImg sagittal;															//重建得到冠状面图像
	sagittal.masking = m_ImgSerial.GetSagittal(256,static_cast<int>(fSlice));	
	sagittal.info = coronal.info;
	m_pLDView->SetImgData(sagittal);											//显示冠状面图像

	sOneImg info = m_ImgSerial.GetCurrentMatImg();								//取得当前图像
	m_pLUView->SetImgData(info);												//显示到原图像窗口
	CString szText;
	szText.Format("序列共有【%d】张图像", m_ImgSerial.GetImageNum());				//在状态栏显示图像总数
	SetStateText(1, szText);

	file = info.info.sId;														//保存当前序列名称
}

/*===========================================================================*
* 打开单张图像
*============================================================================*/
void CMainFrame::OnFileOpenOne()
{
	CFileDialog dlg(TRUE);														//文件打开对话框
	dlg.m_ofn.lpstrTitle = "打开单张图像";											//指定打开文件对话框名称
	//文件过滤器
	dlg.m_ofn.lpstrFilter = "Dcm Files(*.dcm)\0*.dcm\0All Files(*.*)\0*.*\0\0";		
	if (IDCANCEL == dlg.DoModal())												//弹出文件打开对话框，选择取消则直接返回
		return;
	CString strFileName = dlg.GetPathName();									//获取文件路径+文件名
	m_ImgSerial.Clear();														//清除原有数据
	m_ImgSerial.LoadDcm(strFileName);											//加载数据
	//SetSum(m_ImgSerial.GetImageNum());										//显示当前图像总数
	//SetCurrent(m_ImgSerial.GetCurrentNum());									//显示当前图像序号
	sOneImg info = m_ImgSerial.GetCurrentMatImg();								//序列中当前图像
	m_pLUView->SetImgData(info);												//设置当前图像
}

void CMainFrame::SetStateText(int num, CString szText)
{
	GetStatusBar()->SetPaneText(num, szText);
}

//调用窗宽窗位调整函数
void CMainFrame::OnWindow()
{
	CWndWidthPosDlg dlg;
	dlg.DoModal();
}

//保存序列图像
void CMainFrame::OnSaveserial()
{
	CSaveImgDlg dlg(m_ImgSerial.m_vImgSerial);
	if (IDCANCEL == dlg.DoModal())
		return;
}

//显示矢状面
void CMainFrame::ShowSagittal(int col, int line)
{
	Mat src = m_ImgSerial.GetSagittal(col, fSlice);
	Mat tmp(src.size(),CV_8UC3);
	cvtColor(src, tmp, CV_GRAY2RGB);
	//画竖线，冠状面对应位置
	for (int i=0;i<tmp.rows;++i)
	{
		//tmp.at<cv::Vec3b>(i,line) = cv::Vec3b
		tmp.at<cv::Vec3w>(i,line)[0] = 65535;
		tmp.at<cv::Vec3w>(i,line)[1] = 0;
		tmp.at<cv::Vec3w>(i,line)[2] = 0;
	}
	//划横线，横断面对应位置
	int num = m_ImgSerial.GetCurrentNum();
	num = num * fSlice/m_ImgSerial.GetImageNum();
	for (int i=0;i<tmp.cols;++i)
	{
		tmp.at<cv::Vec3w>(num,i)[0] = 0;
		tmp.at<cv::Vec3w>(num,i)[1] = 0;
		tmp.at<cv::Vec3w>(num,i)[2] = 65535;
	}
	sOneImg img;
	img.info = m_ImgSerial.GetBegin()->info;
	img.masking = tmp.clone();
	m_pLDView->SetImgData(img);
}

//显示冠状面
void CMainFrame::ShowCoronal(int row, int line)
{
	Mat src = m_ImgSerial.GetCoronal(row,fSlice);
	Mat tmp(src.size(),CV_8UC3);
	cvtColor(src, tmp, CV_GRAY2RGB);
	for (int i=0;i<tmp.rows;++i)
	{
		tmp.at<cv::Vec3w>(i,line)[0] = 65535;
		tmp.at<cv::Vec3w>(i,line)[1] = 0;
		tmp.at<cv::Vec3w>(i,line)[2] = 0;
	}
	//划横线，横断面对应位置
	int num = m_ImgSerial.GetCurrentNum();
	num = num*fSlice/m_ImgSerial.GetImageNum();
	for (int i=0;i<tmp.cols;++i)
	{
		tmp.at<cv::Vec3w>(num, i)[0] = 0;
		tmp.at<cv::Vec3w>(num, i)[1] = 0;
		tmp.at<cv::Vec3w>(num, i)[2] = 65535;
	}
	sOneImg img;
	img.info = m_ImgSerial.GetBegin()->info;
	img.masking = tmp.clone();
	m_pRUView->SetImgData(img);
}

//显示专家标记数据菜单响应
void CMainFrame::OnDisplayNote()
{
	m_isDisplayNote = (m_isDisplayNote == FALSE);
}

//显示专家标记数据菜单响应，菜单状态
void CMainFrame::OnUpdateDisplayNote(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_isDisplayNote == TRUE);
}

void CMainFrame::OnBtnClick()
{
	m_bCanClick = (m_bCanClick == false);
}

void CMainFrame::OnUpdateBtnClick(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_bCanClick == TRUE);
}


BOOL CMainFrame::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (m_pLUView)
		m_pLUView->Invalidate(FALSE);
	if (m_pRUView)
		m_pRUView->Invalidate(FALSE);
	if (m_pLDView)
		m_pLDView->Invalidate(FALSE);
	if (m_pRDView)
		m_pRDView->Invalidate(FALSE);

	return TRUE;
	//return CFrameWnd::OnEraseBkgnd(pDC);
}


/*===========================================================================*
* 主框架消息路由
*============================================================================*/
BOOL CMainFrame::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	return CFrameWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

/*===========================================================================*
* 主框架的创建
*============================================================================*/
int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (!m_wndStatusBar.Create(this))											//创建状态栏					
		return -1;										
	m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT));	//状态栏窗格设置
	m_wndStatusBar.SetPaneInfo(0, ID_SEPARATOR, SBPS_NORMAL, 300);				//状态栏显示内容设置和长度大小设置
	m_wndStatusBar.SetPaneInfo(1, IDS_ALLIMGNUM, SBPS_NORMAL, 150);
	m_wndStatusBar.SetPaneInfo(2, IDS_CURRENTIMGNUM, SBPS_NORMAL, 150);
	m_wndStatusBar.SetPaneInfo(3, IDS_CURRENTIMGNUM, SBPS_NORMAL, 120);

	if (!m_toolBarFunc.CreateEx(this,TBSTYLE_FLAT, WS_CHILD|WS_VISIBLE|CBRS_LEFT
		|CBRS_TOOLTIPS|CBRS_FLYBY|CBRS_SIZE_DYNAMIC)							//创建左侧工具栏
		|| !m_toolBarFunc.LoadToolBar(IDR_TOOLBAR1))return -1;

	m_toolBarFunc.EnableDocking(CBRS_ALIGN_ANY);								//工具栏可以停靠
	EnableDocking(CBRS_ALIGN_ANY);												//窗口支持停靠
	DockControlBar(&m_toolBarFunc);												//停靠工具栏

	if (!m_toolBarFile.CreateEx(this,TBSTYLE_FLAT, WS_CHILD|WS_VISIBLE|CBRS_TOP	//创建上方工具栏
		|CBRS_TOOLTIPS|CBRS_FLYBY|CBRS_SIZE_DYNAMIC)
		|| !m_toolBarFile.LoadToolBar(IDR_TOOLBAR2))return -1;

	m_toolBarFile.EnableDocking(CBRS_ALIGN_ANY);								//工具栏可以停靠
	EnableDocking(CBRS_ALIGN_ANY);												//窗口支持停靠
	DockControlBar(&m_toolBarFile);												//停靠工具栏

	PostMessage(UM_PROSSESS);													//发送消息，在状态栏上创建工具栏
				
	TCHAR szCurDir[512];						   								//获取EXE目录
	memset(szCurDir, 0, sizeof(szCurDir));
	int length = 0;
	GetModuleFileName(NULL, szCurDir, length);									//获取应用程序当前完整路径
	m_szAppPath.Format("%s",szCurDir);
	int end = m_szAppPath.ReverseFind('\\');									//逆向查找'\'去掉应用程序名
	m_szAppPath = m_szAppPath.Left(end);			
	
	m_Log.InitLog(m_szAppPath);													//初始化日志对象
	m_Log.WriteLog("%s","应用程序打开成功");										//写日志

	Global::InitStatics();														//初始化全局变量
	
	m_toolBarFile.SetButtonText(0, "打开");										//工具栏文字提示
	m_toolBarFile.SetButtonText(1, "保存");
	m_toolBarFile.SetButtonText(2, "另存");
	m_toolBarFile.SetButtonText(3, "打印");
	CRect rect;
	m_toolBarFile.GetItemRect(0, &rect);
	m_toolBarFile.SetSizes(CSize(rect.Width(),rect.Height()),CSize(16,16));		//调整工具栏大小
	m_toolBarFile.SetWindowText("文件");
	FloatControlBar(&m_toolBarFile, CPoint(1220,80));							//浮动控制栏
	m_hMutex = CreateMutex(NULL, FALSE, NULL);									//创建互斥量，用于数据同步
	return 0;
}
