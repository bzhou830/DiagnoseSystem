// ClassifierView.cpp : ʵ���ļ�
#include "stdafx.h"
#include "RBDcm.h"
#include "ClassifierView.h"


// CClassifierView
IMPLEMENT_DYNCREATE(CLDView, CView)


/***************************************************************************************
Function: CClassifierView���캯��
Input:    NULL
Output:   BOOL
Description: ��ͼ�๹��ʱ��������ͼ��
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
Function: CClassifierView��OnDraw��Ϣ��Ӧ
Input:    NULL
Output:   pDC:����DC 
Description: �����Լ���ͼ����л��ƣ�ʹ��˫���壩
			ͼ����������ͼ�๹��ʱ��������ͼ��
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


// CClassifierView ���
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
Function: Ϊ����������ͼ������
Input:    sOneImg һ��ͼ��������Ϣ
Output:   void 
Description: ����ͼ�����ݺ�ˢ�½��棬��ʾͼ����Ϣ
Return:   NULL
Others:   NULL
***************************************************************************************/
void CLDView::SetImgData(sOneImg src)
{
	Mat drawing = Mat::zeros(src.masking.size(), CV_8UC3 );			//����һ����ԭͼ��ߴ���ͬ�Ĳ�ɫͼ��
	if (src.masking.type() == CV_8UC1)								//�Ҷ�ͼת���ɲ�ɫͼ
		cvtColor(src.masking, drawing, CV_GRAY2RGB);
	else
		drawing = src.masking.clone();

	//��ער�ұ�ע�Ľ����Ϣ����ڱ�עΪ��ɫ
	/*
	if (src.pSingleNodules != NULL)									//����Ӧ��ע��Ϣ��д����
	{
		int size = src.pSingleNodules->vcNodulePoint.size();		//������������
		list<NodulePoint>::iterator itr = src.pSingleNodules->vcNodulePoint.begin();
		list<NodulePoint>::iterator end = src.pSingleNodules->vcNodulePoint.end();
		for (; itr != end; ++itr)									//����ÿһ������
		{
			NodulePoint pt = *itr;
			drawing.at<cv::Vec3b>(itr->y, itr->x)[0] = 0;			//B
			drawing.at<cv::Vec3b>(itr->y, itr->x)[1] = 0;			//G
			drawing.at<cv::Vec3b>(itr->y, itr->x)[2] = 255;			//R
		}
	}
	*/
	m_sOneImgShow.masking = drawing.clone();							//������������
	m_sOneImgShow.info = src.info;									//����ͼ���������
	m_ls[0]->SetLayerImgData(m_sOneImgShow.masking);					//��������ͼ�������	
	m_ls[1]->SetLayerInfoData(m_sOneImgShow.info);					//��������������ݲ�
	Invalidate(FALSE);
}


/***************************************************************************************
Function: ��ڷָ����Ӧ����
Input:    sOneImg һ��ͼ��������Ϣ
Output:   void 
Description: ����ͼ�����ݺ�ˢ�½��棬��ʾͼ����Ϣ
Return:   NULL
Others:   NULL
***************************************************************************************/
void CLDView::SegNodules(sOneImg src)
{
	CSegmentOperat seg;									//����һ���ָ����
	m_sOneImgShow.info = src.info;						//��������
	cv::Mat dst = seg.MoMSeg(src.masking.clone());		//���÷ָ��ຯ����ͼ����зָ�
	m_sOneImgShow.masking = dst.clone();					//�ָ�ṹ�������
	
	m_ls[0]->SetLayerImgData(m_sOneImgShow.masking);		//����ͼ����Ϣ�����ҽ���ˢ����ʾ
	m_ls[1]->SetLayerInfoData(m_sOneImgShow.info);
	Invalidate(FALSE);
}



/***************************************************************************************
Function: ��ڷָ����Ӧ����
Input:    sOneImg һ��ͼ��������Ϣ
Output:   void 
Description: ����ͼ�����ݺ�ˢ�½��棬��ʾͼ����Ϣ
Return:   NULL
Others:   NULL
***************************************************************************************/
void CLDView::SegNodulesMean(sOneImg src)
{
	CSegmentOperat seg;									//����һ���ָ����
	m_sOneImgShow.info = src.info;						//��������
	cv::Mat dst;// = seg.IsodataSeg(src.pixle.clone());	//���÷ָ��ຯ����ͼ����зָ�
	
	sOneImg srcimg = ((CMainFrame*)AfxGetMainWnd())->m_pLUView->GetOneImg();
	
	bitwise_and(dst, srcimg.masking, dst);
	//cv::subtract(dst, srcimg.pixle, dst);
	
	m_sOneImgShow.masking = dst.clone();					//�ָ�ṹ�������
	
	m_ls[0]->SetLayerImgData(m_sOneImgShow.masking);		//����ͼ����Ϣ�����ҽ���ˢ����ʾ
	m_ls[1]->SetLayerInfoData(m_sOneImgShow.info);
	Invalidate(FALSE);
}



void CLDView::HenssisenEnhance(sOneImg src)
{
	m_sOneImgShow.info = src.info;						//��������
	CEnhanceOperat enhance;
	cv::Mat dst = enhance.HessianEnhance(m_sOneImgShow.masking.clone(),2,10);	//����һ���ָ����
	m_sOneImgShow.masking = dst.clone();					//�ָ�ṹ�������
	m_ls[0]->SetLayerImgData(m_sOneImgShow.masking);		//����ͼ����Ϣ�����ҽ���ˢ����ʾ
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
