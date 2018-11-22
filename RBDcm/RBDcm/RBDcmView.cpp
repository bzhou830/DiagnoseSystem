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


// CRBDcmView ����/����
CLUView::CLUView() : m_nType(0)
	, isMax(false)
{
	CLayer* pLayer = NULL;
	m_ls.RemoveAll();								//ɾ������ͼ��
	pLayer = CLayer::Create(IMG_LAYER);				//����ͼ��������ʾ��
	m_ls.Add(pLayer);
	pLayer = CLayer::Create(IMG_INFO_LAYER);		//����ͼ�������Ϣ��ʾ��
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
	//CREATESTRUCT cs ���޸Ĵ��������ʽ
	return CView::PreCreateWindow(cs);
}

/***************************************************************************************
Function: �������
Input:    
Output:   NULL
Description: ��ͼ���ϵ���Ϣ���Ƶ���Ļ��
             Ŀǰû��ʵ��˫���壬��ʱ������Ļ��˸������
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
	
    //ͼ�����ͼ���Լ������н��л���
	int i = 0;
	int nSize = m_ls.GetSize();
	while (i < nSize)
	{
		m_ls[i]->OnDraw(&MemDC, rc);
		i++;
	}
	pDC->BitBlt(0, 0, rc.Width(), rc.Height(), &MemDC, 0, 0, SRCCOPY);
}


// CRBDcmView ���
#ifdef _DEBUG
void CLUView::AssertValid() const
{
	CView::AssertValid();
}

void CLUView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CRBDcmDoc* CLUView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CRBDcmDoc)));
	return (CRBDcmDoc*)m_pDocument;
}
#endif //_DEBUG


/***************************************************************************************
Function: ��ͼ�����°�����������Ϣ��Ӧ
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
	//����任��ͼ���ϵĵ�
	point.x = point.x - rect.Width()/2 + 256;			//ͼ���Ⱥ͸߶Ⱦ�Ϊ512
	point.y = point.y - rect.Height()/2 + 256; 
	
	if (point.x < 0 || point.y < 0)						//�㲻��ͼ����
		return;

	//��������ʸ״��͹�״������
	((CMainFrame*)AfxGetMainWnd())->ShowSagittal(point.y, point.x);
	((CMainFrame*)AfxGetMainWnd())->ShowCoronal(point.x, point.y);
	
	//�����Ҳ������Ӧ������
	//m_sOneImgShow
	/*
	CRBDcmDoc* pDoc = GetDocument();    //�ļ�ָ��
	if (pDoc->m_ls.GetSize() == 0)		//ͼ���������򷵻�
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
Function: ��ͼ������̧����������Ϣ��Ӧ
***************************************************************************************/
void CLUView::OnLButtonUp(UINT nFlags, CPoint point)
{
	CView::OnLButtonUp(nFlags, point);
}


/***************************************************************************************
Function: ��ͼ��������������Ϣ
***************************************************************************************/
BOOL CLUView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	if ( !(((CMainFrame*)AfxGetMainWnd())->IsHasImgs()) )		//������û�е���ͼ����ֱ���˳�
		return FALSE;
	if (m_ls.GetSize() == 0)									//ͼ���������򷵻�
		return FALSE;
	CImgDataLayer* pImgLayer = (CImgDataLayer*)(m_ls[0]);		//ͼ�����ݲ�
	static int count = 0;										//��¼�����������˲�
	static double action = 0.0;									
	if (zDelta > 0)												//���������� �Ŵ�ͼ��
		((CMainFrame*)AfxGetMainWnd())->lastimg();
	else														//������ڹ��� ��Сͼ��
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
Function: ��������С����Ӧ
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
Function: View���ʼ������
Input:    void
Output:   NULL
Description: �ڴ˻�ȡ����������ָ�� XX
rewriter:    ��ֱ���ڴ˻�ȡ���ָ�룬�Ӵ��Ļ�ȡ����ȫ����Frame������ͳһ���й���
Return:   void
Others:   NULL
***************************************************************************************/
void CLUView::OnInitialUpdate()
{
	CView::OnInitialUpdate();
}


/***************************************************************************************
Function: ��ͼ������ʱ��Ϣ��Ӧ
Input:    void
Output:   NULL
Description: �����ڶ��ϴ����Ķ���
             ���ҵ���Doc������ٺ�����Doc���ж��ϴ����Ķ����������
Return:   void
Others:   NULL
***************************************************************************************/
void CLUView::OnDestroy()
{
	CView::OnDestroy();
	CRBDcmDoc *pDoc = GetDocument();
}


/***************************************************************************************
Function: ���˫����Ϣ��Ӧ
Input:    �������˫�������Ϣ
Output:   NULL
Description: ����ڴ�����˫���������Ҫ���ô�������Ϊ��ǰ�Ӵ��ڵ������ʾ
Return:   void
***************************************************************************************/
void CLUView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	CView::OnLButtonDblClk(nFlags, point);
}

//������ϴ�������
void CLUView::SetImgData(sOneImg src)
{
	if (!m_sOneImgShow.pPixle)												//ԭ���ݲ�Ϊ��ʱɾ��ԭ����
		delete m_sOneImgShow.pPixle;
	m_sOneImgShow.pPixle = new Mat(src.src_u16.size(), CV_16UC3);			//����һ����ɫ��ͼ��
	
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

	cvtColor(dst, *(m_sOneImgShow.pPixle), CV_GRAY2RGB);					//�Ҷ�ͼת���ɲ�ɫͼ
	if (((CMainFrame*)AfxGetMainWnd())->m_isDisplayNote)					//�Ƿ���ʾר�ұ�ע��Ϣ
	{
		if (src.pSingleNodules != NULL)										//�Ƿ����ר�ұ�ע��Ϣ
		{
			int size = src.pSingleNodules->vcNodulePoint.size();			//������������
			list<NodulePoint>::iterator itr = src.pSingleNodules->vcNodulePoint.begin();
			list<NodulePoint>::iterator end = src.pSingleNodules->vcNodulePoint.end();
			for (; itr != end; ++itr)										//����ÿһ������
			{
				NodulePoint pt = *itr;
				m_sOneImgShow.pPixle->at<cv::Vec3w>(itr->y, itr->x)[0] = 0;	//B
				m_sOneImgShow.pPixle->at<cv::Vec3w>(itr->y, itr->x)[1] = 0;	//G
				m_sOneImgShow.pPixle->at<cv::Vec3w>(itr->y, itr->x)[2] = 65535;	//R
			}
		}
	}
	
	m_sOneImgShow.info = src.info;											//������������
	m_ls[0]->SetLayerImgData(*(m_sOneImgShow.pPixle));
	m_ls[1]->SetLayerInfoData(m_sOneImgShow.info);
	Invalidate(FALSE);
}


//�Ҽ��˵�
void CLUView::OnContextMenu(CWnd* pWnd, CPoint point)
{
	CMenu menu;
	menu.LoadMenu(IDR_POPUP);
	menu.GetSubMenu(0)->TrackPopupMenu(TPM_LEFTALIGN|TPM_LEFTBUTTON|TPM_RIGHTBUTTON, point.x, point.y, this);
	if (m_sOneImgShow.pPixle == NULL)
		return;

	if (!m_sOneImgShow.pPixle->empty())		//ͼ������Ϊ��
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
		MessageBox("������û���κ�ͼ��", "RBDcm��ʾ��", MB_OK|MB_ICONWARNING);
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
		equalizeHist(dst, dst);									//opencv�⺯��
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
	cv::Mat kernal;												//�����任��
	cv::Mat dst;												//Ŀ��ͼ��
	//�ӶԻ����л�ȡ�任��
	if (dlg.m_mask.weight != 0)									//Ȩ��ϵ����Ϊ0					
	{
		double tmp = (double)dlg.m_mask.weight;
		kernal = (Mat_<double>(3, 3) <<
			dlg.m_mask.mk[0][0]/tmp, dlg.m_mask.mk[0][1]/tmp, dlg.m_mask.mk[0][2]/tmp,
			dlg.m_mask.mk[1][0]/tmp, dlg.m_mask.mk[1][1]/tmp, dlg.m_mask.mk[1][2]/tmp,
			dlg.m_mask.mk[2][0]/tmp, dlg.m_mask.mk[2][1]/tmp, dlg.m_mask.mk[2][2]/tmp);
	}
	else												//Ȩ��ϵ��Ϊ0
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
	medianBlur(*(m_sOneImgShow.pPixle), dst, 3);			//opencv�⺯��
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
	if (!isMax)				//�������ʱ�Ŵ���������ʱ��С��ԭ����С
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


