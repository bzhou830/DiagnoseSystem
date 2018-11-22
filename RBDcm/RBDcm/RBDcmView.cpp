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
IMPLEMENT_DYNCREATE(CLUView, CView)

BEGIN_MESSAGE_MAP(CLUView, CView)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEWHEEL()
	ON_WM_KEYDOWN()
	ON_WM_DESTROY()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_MENU_DRAWHIST, &CLUView::OnMenuDrawhist)
	ON_COMMAND(ID_MENU_HISTALL, &CLUView::OnMenuHistall)
	ON_COMMAND(ID_MENU_MASK, &CLUView::OnMenuMask)
	ON_COMMAND(ID_MENU_FILTER, &CLUView::OnMenuFilter)
	ON_COMMAND(ID_MENU_MAXWND, &CLUView::OnMenuMaxwnd)
END_MESSAGE_MAP()


// CRBDcmView 构造/析构
CLUView::CLUView() : m_nType(0)
	, isMax(false)
{
	CLayer* pLayer = NULL;
	m_ls.RemoveAll();								//删除所有图层
	pLayer = CLayer::Create(IMG_LAYER);				//创建图像数据显示层
	m_ls.Add(pLayer);
	pLayer = CLayer::Create(IMG_INFO_LAYER);		//创建图像相关信息显示层
	m_ls.Add(pLayer);
	/*
	pLayer = CLayer::Create(IMG_TAG);
	m_ls.Add(pLayer);*/
}

CLUView::~CLUView()
{
}

BOOL CLUView::PreCreateWindow(CREATESTRUCT& cs)
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
void CLUView::OnDraw(CDC* pDC)
{
	CRect rc;
	CRBMemoryDC MemDC;
	GetClientRect(&rc);
	MemDC.Create(rc.Width(), rc.Height(), pDC);
	MemDC.FillSolidRect(0, 0, rc.Width(), rc.Height(), RGB(0, 0, 0));
	
    //图层改在图层自己的类中进行绘制
	int i = 0;
	int nSize = m_ls.GetSize();
	while (i < nSize)
	{
		m_ls[i]->OnDraw(&MemDC, rc);
		i++;
	}
	pDC->BitBlt(0, 0, rc.Width(), rc.Height(), &MemDC, 0, 0, SRCCOPY);
}


// CRBDcmView 诊断
#ifdef _DEBUG
void CLUView::AssertValid() const
{
	CView::AssertValid();
}

void CLUView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CRBDcmDoc* CLUView::GetDocument() const // 非调试版本是内联的
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
void CLUView::OnLButtonDown(UINT nFlags, CPoint point)
{
	if (!((CMainFrame*)AfxGetMainWnd())->IsHasImgs())
		return;

	if (!((CMainFrame*)AfxGetMainWnd())->m_bCanClick)
		return;

	CRect rect;
	GetClientRect(&rect);
	//坐标变换到图像上的点
	point.x = point.x - rect.Width()/2 + 256;			//图像宽度和高度均为512
	point.y = point.y - rect.Height()/2 + 256; 
	
	if (point.x < 0 || point.y < 0)						//点不在图像上
		return;

	//重新设置矢状面和冠状面数据
	((CMainFrame*)AfxGetMainWnd())->ShowSagittal(point.y, point.x);
	((CMainFrame*)AfxGetMainWnd())->ShowCoronal(point.x, point.y);
	
	//横断面也画出相应的线条
	//m_sOneImgShow
	/*
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
	*/
	CView::OnLButtonDown(nFlags, point);
}


/***************************************************************************************
Function: 视图界面下抬起鼠标左键消息响应
***************************************************************************************/
void CLUView::OnLButtonUp(UINT nFlags, CPoint point)
{
	CView::OnLButtonUp(nFlags, point);
}


/***************************************************************************************
Function: 视图界面下鼠标滚动消息
***************************************************************************************/
BOOL CLUView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	if ( !(((CMainFrame*)AfxGetMainWnd())->IsHasImgs()) )		//程序中没有导入图像则直接退出
		return FALSE;
	if (m_ls.GetSize() == 0)									//图层无数据则返回
		return FALSE;
	CImgDataLayer* pImgLayer = (CImgDataLayer*)(m_ls[0]);		//图像数据层
	static int count = 0;										//记录滚动次数，滤波
	static double action = 0.0;									
	if (zDelta > 0)												//鼠标向外滚动 放大图像
		((CMainFrame*)AfxGetMainWnd())->lastimg();
	else														//鼠标向内滚动 缩小图像
		((CMainFrame*)AfxGetMainWnd())->nextimg();
	return CView::OnMouseWheel(nFlags, zDelta, pt);
}

void CLUView::ReDraw(void)
{
	CMainFrame *pMain = (CMainFrame *)AfxGetApp()->m_pMainWnd;
	CLUView *pRDBView = (CLUView *)pMain->GetActiveView();
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
void CLUView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	switch (nChar)
	{
	case VK_UP:
		((CMainFrame*)AfxGetMainWnd())->lastimg();
		break;
	case VK_DOWN:
		((CMainFrame*)AfxGetMainWnd())->nextimg();
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
void CLUView::OnInitialUpdate()
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
void CLUView::OnDestroy()
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
***************************************************************************************/
void CLUView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	CView::OnLButtonDblClk(nFlags, point);
}

//向界面上传入数据
void CLUView::SetImgData(sOneImg src)
{
	if (!m_sOneImgShow.pPixle)												//原数据不为空时删除原数据
		delete m_sOneImgShow.pPixle;
	m_sOneImgShow.pPixle = new Mat(src.src_u16.size(), CV_16UC3);			//创建一个彩色的图像
	
	Mat dst = Mat::zeros(src.src_u16.size(),CV_16UC1);
	//Mat watch = src.masking;
	for (int i=0;i<src.masking.rows;i++)
	{
		for (int j=0;j<src.masking.cols;j++)
		{
			if(src.masking.at<uchar>(i,j) != 0)
				dst.at<short>(i, j) = src.src_u16.at<short>(i,j);
		}
	}
	Mat tmp = src.src_u16;

	cvtColor(dst, *(m_sOneImgShow.pPixle), CV_GRAY2RGB);					//灰度图转换成彩色图
	if (((CMainFrame*)AfxGetMainWnd())->m_isDisplayNote)					//是否显示专家标注信息
	{
		if (src.pSingleNodules != NULL)										//是否存在专家标注信息
		{
			int size = src.pSingleNodules->vcNodulePoint.size();			//共有数据条数
			list<NodulePoint>::iterator itr = src.pSingleNodules->vcNodulePoint.begin();
			list<NodulePoint>::iterator end = src.pSingleNodules->vcNodulePoint.end();
			for (; itr != end; ++itr)										//操作每一条数据
			{
				NodulePoint pt = *itr;
				m_sOneImgShow.pPixle->at<cv::Vec3w>(itr->y, itr->x)[0] = 0;	//B
				m_sOneImgShow.pPixle->at<cv::Vec3w>(itr->y, itr->x)[1] = 0;	//G
				m_sOneImgShow.pPixle->at<cv::Vec3w>(itr->y, itr->x)[2] = 65535;	//R
			}
		}
	}
	
	m_sOneImgShow.info = src.info;											//拷贝附加数据
	m_ls[0]->SetLayerImgData(*(m_sOneImgShow.pPixle));
	m_ls[1]->SetLayerInfoData(m_sOneImgShow.info);
	Invalidate(FALSE);
}


//右键菜单
void CLUView::OnContextMenu(CWnd* pWnd, CPoint point)
{
	CMenu menu;
	menu.LoadMenu(IDR_POPUP);
	menu.GetSubMenu(0)->TrackPopupMenu(TPM_LEFTALIGN|TPM_LEFTBUTTON|TPM_RIGHTBUTTON, point.x, point.y, this);
	if (m_sOneImgShow.pPixle == NULL)
		return;

	if (!m_sOneImgShow.pPixle->empty())		//图像数据为空
	{
		menu.EnableMenuItem(ID_MENU_DRAWHIST, MF_GRAYED);
	}
}


void CLUView::OnMenuDrawhist()
{
	if (m_sOneImgShow.pPixle == NULL)
		return;
	if (m_sOneImgShow.pPixle->empty())
	{
		MessageBox("窗体内没有任何图像", "RBDcm提示您", MB_OK|MB_ICONWARNING);
		return;
	}
	CDrawHistDlg dlg;
	dlg.SetHistValue(*(m_sOneImgShow.pPixle));
	dlg.DoModal();
}


void CLUView::OnMenuHistall()
{
	if (m_sOneImgShow.pPixle == NULL)
		return;
	if (!m_sOneImgShow.pPixle->empty())
	{
		Mat dst = m_sOneImgShow.pPixle->clone();
		equalizeHist(dst, dst);									//opencv库函数
		*(m_sOneImgShow.pPixle) = dst.clone();					//
		m_ls[0]->SetLayerImgData(*(m_sOneImgShow.pPixle));		//
		Invalidate(FALSE);
	}
}


void CLUView::OnMenuMask()
{
	if (m_sOneImgShow.pPixle == NULL || m_sOneImgShow.pPixle->empty())
		return;

	CMaskSetDlg dlg;
	if (IDCANCEL == dlg.DoModal())
		return;
	cv::Mat kernal;												//构建变换核
	cv::Mat dst;												//目标图像
	//从对话框中获取变换核
	if (dlg.m_mask.weight != 0)									//权重系数不为0					
	{
		double tmp = (double)dlg.m_mask.weight;
		kernal = (Mat_<double>(3, 3) <<
			dlg.m_mask.mk[0][0]/tmp, dlg.m_mask.mk[0][1]/tmp, dlg.m_mask.mk[0][2]/tmp,
			dlg.m_mask.mk[1][0]/tmp, dlg.m_mask.mk[1][1]/tmp, dlg.m_mask.mk[1][2]/tmp,
			dlg.m_mask.mk[2][0]/tmp, dlg.m_mask.mk[2][1]/tmp, dlg.m_mask.mk[2][2]/tmp);
	}
	else												//权重系数为0
	{
		kernal = (Mat_<char>(3, 3) <<
			dlg.m_mask.mk[0][0], dlg.m_mask.mk[0][1], dlg.m_mask.mk[0][2],
			dlg.m_mask.mk[1][0], dlg.m_mask.mk[1][1], dlg.m_mask.mk[1][2],
			dlg.m_mask.mk[2][0], dlg.m_mask.mk[2][1], dlg.m_mask.mk[2][2]);
	}
	filter2D(m_sOneImgShow.pPixle->clone(), dst, 8, kernal);
	m_ls[0]->SetLayerImgData(dst);
	Invalidate(FALSE);
}


void CLUView::OnMenuFilter()
{
	if (m_sOneImgShow.pPixle == NULL || m_sOneImgShow.pPixle->empty())
		return;
	Mat dst;
	medianBlur(*(m_sOneImgShow.pPixle), dst, 3);			//opencv库函数
	*m_sOneImgShow.pPixle = dst.clone();
	m_ls[0]->SetLayerImgData(*(m_sOneImgShow.pPixle));
	Invalidate(FALSE);
}


sOneImg CLUView::GetOneImg()
{
	sOneImg tmp;
	cvtColor(*m_sOneImgShow.pPixle,tmp.masking,COLOR_BGR2GRAY);
	return tmp;
}


void CLUView::OnMenuMaxwnd()
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
}


