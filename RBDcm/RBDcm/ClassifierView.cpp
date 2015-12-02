// ClassifierView.cpp : 实现文件
#include "stdafx.h"
#include "RBDcm.h"
#include "ClassifierView.h"


// CClassifierView
IMPLEMENT_DYNCREATE(CClassifierView, CView)


/***************************************************************************************
Function: CClassifierView构造函数
Input:    NULL
Output:   BOOL
Description: 视图类构造时创建两个图层
Return:   NULL
Others:   NULL
***************************************************************************************/
CClassifierView::CClassifierView()
{
	CLayer* pLayer = NULL;						//Create layers
	m_ls.RemoveAll();							//delete all layers
	pLayer = CLayer::Create(IMG_LAYER);			//create images layer
	m_ls.Add(pLayer);							//add to the layer array
	pLayer = CLayer::Create(IMG_INFO_LAYER);	//create info layer 
	m_ls.Add(pLayer);
}


CClassifierView::~CClassifierView()
{
}


BEGIN_MESSAGE_MAP(CClassifierView, CView)
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
void CClassifierView::OnDraw(CDC* pDC)
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
void CClassifierView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void CClassifierView::Dump(CDumpContext& dc) const
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
void CClassifierView::SetImgData(sOneImg src)
{
	m_sOneImgShow.pixle = src.pixle.clone();
	m_sOneImgShow.info = src.info;
	m_ls[0]->SetLayerImgData(m_sOneImgShow.pixle);
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
void CClassifierView::SegNodules(sOneImg src)
{
	CSegmentOperat seg;									//创建一个分割对象
	m_sOneImgShow.info = src.info;						//传入数据
	cv::Mat dst = seg.MoMSeg(src.pixle.clone());		//调用分割类函数对图像进行分割
	m_sOneImgShow.pixle = dst.clone();					//分割结构输出保存
	
	m_ls[0]->SetLayerImgData(m_sOneImgShow.pixle);		//设置图层信息，并且进行刷新显示
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
void CClassifierView::SegNodulesMean(sOneImg src)
{
	CSegmentOperat seg;									//创建一个分割对象
	m_sOneImgShow.info = src.info;						//传入数据
	cv::Mat dst = seg.IsodataSeg(src.pixle.clone());	//调用分割类函数对图像进行分割
	
	sOneImg srcimg = ((CMainFrame*)AfxGetMainWnd())->m_pRBView->GetOneImg();
	
	bitwise_and(dst, srcimg.pixle, dst);
	//cv::subtract(dst, srcimg.pixle, dst);
	
	m_sOneImgShow.pixle = dst.clone();					//分割结构输出保存
	
	m_ls[0]->SetLayerImgData(m_sOneImgShow.pixle);		//设置图层信息，并且进行刷新显示
	m_ls[1]->SetLayerInfoData(m_sOneImgShow.info);
	Invalidate(FALSE);
}


void CClassifierView::HenssisenEnhance(sOneImg src)
{
	m_sOneImgShow.info = src.info;						//传入数据
	CEnhanceOperat enhance;
	cv::Mat dst = enhance.HessianEnhance(m_sOneImgShow.pixle.clone(),2,10);	//创建一个分割对象
	m_sOneImgShow.pixle = dst.clone();					//分割结构输出保存
	m_ls[0]->SetLayerImgData(m_sOneImgShow.pixle);		//设置图层信息，并且进行刷新显示
	m_ls[1]->SetLayerInfoData(m_sOneImgShow.info);
	Invalidate(FALSE);
}

sOneImg CClassifierView::GetOneImg()
{
	return m_sOneImgShow;
}

