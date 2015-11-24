// SegView.cpp : 实现文件
#include "stdafx.h"
#include "RBDcm.h"
#include "SegView.h"

// CSegView
IMPLEMENT_DYNCREATE(CSegView, CView)

CSegView::CSegView()
{
	CLayer* pLayer = NULL;						//Create layers
	m_ls.RemoveAll();							//delete all layers
	pLayer = CLayer::Create(IMG_LAYER);			//create images layer
	m_ls.Add(pLayer);							//add to the layer array
	pLayer = CLayer::Create(IMG_INFO_LAYER);	//create info layer 
	m_ls.Add(pLayer);
}


CSegView::~CSegView()
{
}


BEGIN_MESSAGE_MAP(CSegView, CView)
END_MESSAGE_MAP()


// CSegView 绘图
void CSegView::OnDraw(CDC* pDC)
{
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

// CSegView 诊断
#ifdef _DEBUG
void CSegView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void CSegView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG


// CSegView 消息处理程序
void CSegView::OnInitialUpdate()
{
	CView::OnInitialUpdate();
}


void CSegView::SetImgData(sOneImg src)
{
	m_sOneImgShow.pixle = src.pixle.clone();
	m_sOneImgShow.info = src.info;
	m_ls[0]->SetLayerImgData(m_sOneImgShow.pixle);
	m_ls[1]->SetLayerInfoData(m_sOneImgShow.info);
	Invalidate(FALSE);
}


void CSegView::SegRealLung(cv::Mat src)
{
	CSegmentOperat seg;
	Mat ar = seg.GetLungArea(m_sOneImgShow.pixle.clone()).clone();
	Mat dst;
	bitwise_and(ar, src, dst);
	m_sOneImgShow.pixle = dst.clone();
	m_ls[0]->SetLayerImgData(m_sOneImgShow.pixle);
	Invalidate(FALSE);
}



void CSegView::GetSegRealLungs(sOneImg& seg)
{
	seg = m_sOneImgShow;
}
