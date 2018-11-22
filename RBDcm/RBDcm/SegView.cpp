// SegView.cpp : 实现文件
#include "stdafx.h"
#include "RBDcm.h"
#include "SegView.h"

// CSegView
IMPLEMENT_DYNCREATE(CRUView, CView)

CRUView::CRUView()
: m_isViewMax(FALSE)
{
	CLayer* pLayer = NULL;						//Create layers
	m_ls.RemoveAll();							//delete all layers
	pLayer = CLayer::Create(IMG_LAYER);			//create images layer
	m_ls.Add(pLayer);							//add to the layer array
	pLayer = CLayer::Create(IMG_INFO_LAYER);	//create info layer 
	m_ls.Add(pLayer);
}


CRUView::~CRUView()
{
}


BEGIN_MESSAGE_MAP(CRUView, CView)
	ON_WM_LBUTTONDBLCLK()
END_MESSAGE_MAP()


// CSegView 绘图
void CRUView::OnDraw(CDC* pDC)
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
void CRUView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void CRUView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG


// CSegView 消息处理程序
void CRUView::OnInitialUpdate()
{
	CView::OnInitialUpdate();
}


//设置树矩信息到界面层
void CRUView::SetImgData(sOneImg src)
{
	if (!m_sOneImgShow.pPixle)										//原数据不为空时删除原数据
		delete m_sOneImgShow.pPixle;

	m_sOneImgShow.pPixle = new Mat(src.masking.size(),CV_8UC3);		//创建一个彩色的图像
	if (src.masking.type() == CV_8UC1)
		cvtColor(src.masking, *(m_sOneImgShow.pPixle), CV_GRAY2RGB);		//灰度图转换成彩色图
	else
		*m_sOneImgShow.pPixle = src.masking.clone();
	
	m_sOneImgShow.info = src.info;
	m_ls[0]->SetLayerImgData(*m_sOneImgShow.pPixle);
	m_ls[1]->SetLayerInfoData(m_sOneImgShow.info);
	Invalidate(FALSE);
}


//分割肺实质
void CRUView::SegRealLung(cv::Mat src)
{
	//使用区域生长分割肺实质
	/*
	CSegmentOperat seg;
	vector<Point2i> pt;
	seg.GetObjectCenter(this->m_sOneImgShow.pixle, pt);   //获取形心
	vector<Point2i>::iterator itr = pt.begin();
	Mat tmp = Mat::zeros(src.size(), CV_8UC1);
	for (;itr != pt.end(); ++itr)
	{
		bitwise_or(tmp,seg.RegionGrow(m_sOneImgShow.pixle,*itr),tmp);
	}
	m_sOneImgShow.pixle = tmp.clone();
	m_ls[0]->SetLayerImgData(m_sOneImgShow.pixle);
	Invalidate(FALSE);
	*/
	//使用普通二值化分割肺实质
	CSegmentOperat seg;
	Mat ar = seg.GetLungArea(m_sOneImgShow.pPixle->clone()).clone();
	Mat dst;
	bitwise_and(ar, src, dst);
	*m_sOneImgShow.pPixle = dst.clone();
	m_ls[0]->SetLayerImgData(*m_sOneImgShow.pPixle);
	Invalidate(FALSE);
}



void CRUView::GetSegRealLungs(sOneImg& seg)
{
	//seg = m_sOneImgShow;
}



//鼠标双击设置响应的窗口为最大化
void CRUView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	if (m_isViewMax)
	{
		m_isViewMax = FALSE;
		((CMainFrame*)AfxGetMainWnd())->ResetFrm();
	}
	else
	{
		m_isViewMax = TRUE;
		((CMainFrame*)AfxGetMainWnd())->SetMainFrm(1);	
	}
	CView::OnLButtonDblClk(nFlags, point);
}
