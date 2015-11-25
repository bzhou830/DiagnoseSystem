// ClassifierView.cpp : 实现文件
#include "stdafx.h"
#include "RBDcm.h"
#include "ClassifierView.h"


// CClassifierView
IMPLEMENT_DYNCREATE(CClassifierView, CView)

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


// CClassifierView 绘图
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


// CClassifierView 消息处理程序
void CClassifierView::SetImgData(sOneImg src)
{
	m_sOneImgShow.pixle = src.pixle.clone();
	m_sOneImgShow.info = src.info;
	m_ls[0]->SetLayerImgData(m_sOneImgShow.pixle);
	m_ls[1]->SetLayerInfoData(m_sOneImgShow.info);
	Invalidate(FALSE);
}


void CClassifierView::SegNodules(sOneImg src)
{
	CSegmentOperat seg;
	m_sOneImgShow.info = src.info;
	cv::Mat dst = seg.MoMSeg(src.pixle.clone());
	m_sOneImgShow.pixle = dst.clone();
	m_ls[0]->SetLayerImgData(m_sOneImgShow.pixle);
	//m_ls[1]->SetLayerInfoData(m_sOneImgShow.info);
	Invalidate(FALSE);
}
