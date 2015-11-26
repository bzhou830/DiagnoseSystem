/**********************************************************************************
  Copyright @ Robin 2015-2025
  File name:  RBDcmView.cpp
  Author: Robin   Version: V1.0   Date: 2015.11.05 
  Description: Software Frame  View Class 
               This class contain main View and image layers manage
  Others:         
***********************************************************************************/
#include "stdafx.h"

#ifndef SHARED_HANDLERS
#include "RBDcm.h"
#endif

#include "RBDcmDoc.h"
#include "RBDcmView.h"
#include "ImgFormat.h"
#include "RBMermoryDC.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CRBDcmView
IMPLEMENT_DYNCREATE(CRBDcmView, CView)

BEGIN_MESSAGE_MAP(CRBDcmView, CView)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEWHEEL()
	ON_WM_KEYDOWN()
	ON_WM_DESTROY()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_RBUTTONDOWN()
	ON_WM_CONTEXTMENU()
END_MESSAGE_MAP()


// CRBDcmView 构造/析构
CRBDcmView::CRBDcmView() : m_nType(0)
	, isMax(false)
{
	CLayer* pLayer = NULL;
	m_ls.RemoveAll();								//删除所有图层
	pLayer = CLayer::Create(IMG_LAYER);				//创建图像数据显示层
	m_ls.Add(pLayer);
	pLayer = CLayer::Create(IMG_INFO_LAYER);		//创建图像相关信息显示层
	m_ls.Add(pLayer);
}

CRBDcmView::~CRBDcmView()
{
}

BOOL CRBDcmView::PreCreateWindow(CREATESTRUCT& cs)
{
	//CREATESTRUCT cs 来修改窗口类或样式
	return CView::PreCreateWindow(cs);
}

/***************************************************************************************
Function: 界面绘制
Input:    
Output:   NULL
Description: 将图层上的信息绘制到屏幕上
             目前没有实现双缓冲，有时产生屏幕闪烁的现象
Return:   void
Others:   NULL
***************************************************************************************/
void CRBDcmView::OnDraw(CDC* pDC)
{
	CRBDcmDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	CRect rc;
	GetClientRect(&rc);

	CRBMemoryDC MemDC;
	MemDC.Create(rc.Width(), rc.Height(), pDC);
	MemDC.FillSolidRect(0, 0, rc.Width(), rc.Height(), RGB(0,0,0));
	
    //图层改在图层自己的类中进行绘制
	int i = 0;
	int nSize = pDoc->m_ls.GetSize();
	//绘制Doc类中存在的图层
	while (i < nSize)
	{
		pDoc->m_ls[i]->OnDraw(&MemDC, rc);
		i++;
	}
	//绘制View中存在的图层
	i = 0;
	nSize = m_ls.GetSize();
	while (i < nSize)
	{
		m_ls[i]->OnDraw(&MemDC, rc);
		i++;
	}
	pDC->BitBlt(0, 0, rc.Width(), rc.Height(), &MemDC, 0, 0, SRCCOPY);
}


// CRBDcmView 诊断
#ifdef _DEBUG
void CRBDcmView::AssertValid() const
{
	CView::AssertValid();
}

void CRBDcmView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CRBDcmDoc* CRBDcmView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CRBDcmDoc)));
	return (CRBDcmDoc*)m_pDocument;
}
#endif //_DEBUG


/***************************************************************************************
Function: 视图界面下按下鼠标左键消息响应
Input:    
Output:   NULL
Description: NULL
Return:   void
Others:   NULL
***************************************************************************************/
void CRBDcmView::OnLButtonDown(UINT nFlags, CPoint point)
{
	CRBDcmDoc* pDoc = GetDocument();    //文件指针
	if (pDoc->m_ls.GetSize() == 0)		//图层无数据则返回
		return ;
	CImgDataLayer* pImgLayer = (CImgDataLayer*)(pDoc->m_ls[0]);
	CSize imgsize = pImgLayer->GetImgSize();
	CRect SrcRect;
	GetClientRect(SrcRect);
	CRect ImgRect;
	ImgRect.top = (SrcRect.Height() - imgsize.cy) / 2;
	ImgRect.left = (SrcRect.Width() - imgsize.cx) / 2;
	ImgRect.right = ImgRect.left + imgsize.cx;
	ImgRect.bottom = ImgRect.top + imgsize.cy;
	CView::OnLButtonDown(nFlags, point);
}

/***************************************************************************************
Function: 视图界面下抬起鼠标左键消息响应
Input:    
Output:   NULL
Description: NULL
Return:   void
Others:   NULL
***************************************************************************************/
void CRBDcmView::OnLButtonUp(UINT nFlags, CPoint point)
{
	
	CView::OnLButtonUp(nFlags, point);
}


/***************************************************************************************
Function: 视图界面下鼠标滚动消息
Input:    
Output:   NULL
Description: NULL
Return:   void
Others:   NULL
***************************************************************************************/
BOOL CRBDcmView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	if (m_ls.GetSize() == 0)								//图层无数据则返回
		return FALSE;
	CImgDataLayer* pImgLayer = (CImgDataLayer*)(m_ls[0]);	//图像数据层
	static int count = 0;									//记录滚动次数，滤波
	static double action = 0.0;								//
	if (zDelta > 0)											//鼠标向外滚动 放大图像
	{
		action += 1.1;										//
		//设置图像放大时图标
		HCURSOR hCursor =AfxGetApp()->LoadCursor(IDC_CURSOR_ZOOMIN);
		::SetCursor(hCursor);
	}
	else                                                    //鼠标向内滚动 缩小图像
	{
		action += 0.9;
		//设置图像缩小时图标
		HCURSOR hCursor =AfxGetApp()->LoadCursor(IDC_CURSOR_ZOOMOUT);
		::SetCursor(hCursor);
	}
	count++;
	if (count >= 3)											//滤波使用
	{
		pImgLayer->SetTimes(action / count);
		count = 0;
		action = 0;
		Invalidate(FALSE);
	}
	return CView::OnMouseWheel(nFlags, zDelta, pt);
}


/***************************************************************************************
Function: 重新绘制界面
Input:    
Output:   NULL
Description: NULL
Return:   void
Others:   NULL
***************************************************************************************/
void CRBDcmView::ReDraw(void)
{
	CMainFrame *pMain = (CMainFrame *)AfxGetApp()->m_pMainWnd;
	CRBDcmView *pRDBView = (CRBDcmView *)pMain->GetActiveView();
	CDC *pDC = pRDBView->GetDC();
	pDC->MoveTo(0, 0);
	pDC->LineTo(100, 100);
	ReleaseDC(pDC);
}


/***************************************************************************************
Function: 按键按下小心响应
Input:    
Output:   NULL
Description: NULL
Return:   void
Others:   NULL
***************************************************************************************/
void CRBDcmView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	switch (nChar)
	{
	case VK_UP:
		((CMainFrame*)AfxGetMainWnd())->m_pOpt->OnBnClickedBtnLast();
		break;
	case VK_DOWN:
		((CMainFrame*)AfxGetMainWnd())->m_pOpt->OnBnClickedBtnNext();
		break;
	}
	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}


/***************************************************************************************
Function: View类初始化函数
Input:    void
Output:   NULL
Description: 在此获取控制面板的类指针 XX
rewriter:    不直接在此获取面板指针，视窗的获取工作全部由Frame类中来统一进行管理
Return:   void
Others:   NULL
***************************************************************************************/
void CRBDcmView::OnInitialUpdate()
{
	CView::OnInitialUpdate();
}


/***************************************************************************************
Function: 视图类销毁时消息响应
Input:    void
Output:   NULL
Description: 销毁在堆上创建的对象
             并且调用Doc类的销毁函数对Doc类中堆上创建的对象进行销毁
Return:   void
Others:   NULL
***************************************************************************************/
void CRBDcmView::OnDestroy()
{
	CView::OnDestroy();
	CRBDcmDoc *pDoc = GetDocument();
}


/***************************************************************************************
Function: 鼠标双击消息响应
Input:    传入鼠标双击相关信息
Output:   NULL
Description: 鼠标在窗口中双击，则代表要将该窗口设置为当前视窗内的最大显示
Return:   void
Others:   NULL
***************************************************************************************/
void CRBDcmView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	if (!isMax)				//不是最大时放大到最大，是最大时缩小到原来大小
	{
		((CMainFrame*)AfxGetMainWnd())->SetMainFrm(0);
		isMax = true;
	}
	else
	{
		((CMainFrame*)AfxGetMainWnd())->ResetFrm();
		isMax = false;
	}
	CView::OnLButtonDblClk(nFlags, point);
}


void CRBDcmView::SetImgData(sOneImg src)
{
	m_sOneImgShow.pixle = src.pixle.clone();
	m_sOneImgShow.info = src.info;
	m_ls[0]->SetLayerImgData(m_sOneImgShow.pixle);
	m_ls[1]->SetLayerInfoData(m_sOneImgShow.info);
	Invalidate(FALSE);
}


void CRBDcmView::OnRButtonDown(UINT nFlags, CPoint point)
{
	/*CMenu menu;
	menu.LoadMenu(IDR_POPUP);
	CMenu* pMenu = menu.GetSubMenu(0);
	ClientToScreen(&point);
	pMenu->TrackPopupMenu(TPM_LEFTALIGN, point.x, point.y, this);
	CView::OnRButtonDown(nFlags, point);*/
}

//右键菜单
void CRBDcmView::OnContextMenu(CWnd* pWnd, CPoint point)
{
	CMenu menu;
	menu.LoadMenu(IDR_POPUP);
	menu.GetSubMenu(0)->TrackPopupMenu(TPM_LEFTALIGN|TPM_LEFTBUTTON|TPM_RIGHTBUTTON, point.x, point.y, this);
}
