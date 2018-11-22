// ClassifierView.cpp : 实现文件
#include "stdafx.h"
#include "RBDcm.h"
#include "ClassifierView.h"


// CClassifierView
IMPLEMENT_DYNCREATE(CLDView, CView)


/***************************************************************************************
Function: CClassifierView构造函数
Input:    NULL
Output:   BOOL
Description: 视图类构造时创建两个图层
Return:   NULL
Others:   NULL
***************************************************************************************/
CLDView::CLDView()
: m_isViewMax(FALSE)
{
	CLayer* pLayer = NULL;						//Create layers
	m_ls.RemoveAll();							//delete all layers
	pLayer = CLayer::Create(IMG_LAYER);			//create images layer
	m_ls.Add(pLayer);							//add to the layer array
	pLayer = CLayer::Create(IMG_INFO_LAYER);	//create info layer 
	m_ls.Add(pLayer);
}


CLDView::~CLDView()
{
}


BEGIN_MESSAGE_MAP(CLDView, CView)
	ON_WM_LBUTTONDBLCLK()
END_MESSAGE_MAP()


/***************************************************************************************
Function: CClassifierView类OnDraw消息响应
Input:    NULL
Output:   pDC:窗口DC 
Description: 对类自己的图层进行绘制（使用双缓冲）
			图层来自于视图类构造时创建两个图层
Return:   NULL
Others:   NULL
***************************************************************************************/
void CLDView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	CRect rc;
	GetClientRect(&rc);
	CRBMemoryDC MemDC;
	MemDC.Create(rc.Width(), rc.Height(), pDC);
	MemDC.FillSolidRect(0, 0, rc.Width(), rc.Height(), RGB(0,0,0));
	int i = 0;
	int nSize = m_ls.GetSize();
	while (i < nSize)
	{
		m_ls[i]->OnDraw(&MemDC, rc);
		i++;
	}
	pDC->BitBlt(0, 0, rc.Width(), rc.Height(), &MemDC, 0, 0, SRCCOPY);
}


// CClassifierView 诊断
#ifdef _DEBUG
void CLDView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void CLDView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG


/***************************************************************************************
Function: 为此类对象添加图层数据
Input:    sOneImg 一张图像的相关信息
Output:   void 
Description: 传入图像数据后刷新界面，显示图层信息
Return:   NULL
Others:   NULL
***************************************************************************************/
void CLDView::SetImgData(sOneImg src)
{
	Mat drawing = Mat::zeros(src.masking.size(), CV_8UC3 );			//创建一个与原图像尺寸相同的彩色图像
	if (src.masking.type() == CV_8UC1)								//灰度图转化成彩色图
		cvtColor(src.masking, drawing, CV_GRAY2RGB);
	else
		drawing = src.masking.clone();

	//标注专家标注的结节信息，结节标注为红色
	/*
	if (src.pSingleNodules != NULL)									//有响应标注信息则写数据
	{
		int size = src.pSingleNodules->vcNodulePoint.size();		//共有数据条数
		list<NodulePoint>::iterator itr = src.pSingleNodules->vcNodulePoint.begin();
		list<NodulePoint>::iterator end = src.pSingleNodules->vcNodulePoint.end();
		for (; itr != end; ++itr)									//操作每一条数据
		{
			NodulePoint pt = *itr;
			drawing.at<cv::Vec3b>(itr->y, itr->x)[0] = 0;			//B
			drawing.at<cv::Vec3b>(itr->y, itr->x)[1] = 0;			//G
			drawing.at<cv::Vec3b>(itr->y, itr->x)[2] = 255;			//R
		}
	}
	*/
	m_sOneImgShow.masking = drawing.clone();							//拷贝像素数据
	m_sOneImgShow.info = src.info;									//拷贝图像相关数据
	m_ls[0]->SetLayerImgData(m_sOneImgShow.masking);					//设置像素图像层数据	
	m_ls[1]->SetLayerInfoData(m_sOneImgShow.info);					//设置像素相关数据层
	Invalidate(FALSE);
}


/***************************************************************************************
Function: 结节分割函数响应函数
Input:    sOneImg 一张图像的相关信息
Output:   void 
Description: 传入图像数据后刷新界面，显示图层信息
Return:   NULL
Others:   NULL
***************************************************************************************/
void CLDView::SegNodules(sOneImg src)
{
	CSegmentOperat seg;									//创建一个分割对象
	m_sOneImgShow.info = src.info;						//传入数据
	cv::Mat dst = seg.MoMSeg(src.masking.clone());		//调用分割类函数对图像进行分割
	m_sOneImgShow.masking = dst.clone();					//分割结构输出保存
	
	m_ls[0]->SetLayerImgData(m_sOneImgShow.masking);		//设置图层信息，并且进行刷新显示
	m_ls[1]->SetLayerInfoData(m_sOneImgShow.info);
	Invalidate(FALSE);
}



/***************************************************************************************
Function: 结节分割函数响应函数
Input:    sOneImg 一张图像的相关信息
Output:   void 
Description: 传入图像数据后刷新界面，显示图层信息
Return:   NULL
Others:   NULL
***************************************************************************************/
void CLDView::SegNodulesMean(sOneImg src)
{
	CSegmentOperat seg;									//创建一个分割对象
	m_sOneImgShow.info = src.info;						//传入数据
	cv::Mat dst;// = seg.IsodataSeg(src.pixle.clone());	//调用分割类函数对图像进行分割
	
	sOneImg srcimg = ((CMainFrame*)AfxGetMainWnd())->m_pLUView->GetOneImg();
	
	bitwise_and(dst, srcimg.masking, dst);
	//cv::subtract(dst, srcimg.pixle, dst);
	
	m_sOneImgShow.masking = dst.clone();					//分割结构输出保存
	
	m_ls[0]->SetLayerImgData(m_sOneImgShow.masking);		//设置图层信息，并且进行刷新显示
	m_ls[1]->SetLayerInfoData(m_sOneImgShow.info);
	Invalidate(FALSE);
}



void CLDView::HenssisenEnhance(sOneImg src)
{
	m_sOneImgShow.info = src.info;						//传入数据
	CEnhanceOperat enhance;
	cv::Mat dst = enhance.HessianEnhance(m_sOneImgShow.masking.clone(),2,10);	//创建一个分割对象
	m_sOneImgShow.masking = dst.clone();					//分割结构输出保存
	m_ls[0]->SetLayerImgData(m_sOneImgShow.masking);		//设置图层信息，并且进行刷新显示
	m_ls[1]->SetLayerInfoData(m_sOneImgShow.info);
	Invalidate(FALSE);
}

sOneImg CLDView::GetOneImg()
{
	return m_sOneImgShow;
}



void CLDView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	if (m_isViewMax)
	{
		m_isViewMax = FALSE;
		((CMainFrame*)AfxGetMainWnd())->ResetFrm();
	}
	else
	{
		m_isViewMax = TRUE;
		((CMainFrame*)AfxGetMainWnd())->SetMainFrm(2);
	}
	CView::OnLButtonDblClk(nFlags, point);
}
