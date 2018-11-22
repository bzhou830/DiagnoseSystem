/**********************************************************************************
  Copyright @ Robin 2015-2025
  File name:  DrawHistDlh.cpp
  Author: Robin   Version: V1.0   Date: 2015.11.07 
  Description: Draw Hist Class  
  Others:         
***********************************************************************************/
#include "stdafx.h"
#include "RBDcm.h"
#include "DrawHistDlg.h"
#include "afxdialogex.h"

// CDrawHistDlh �Ի���
IMPLEMENT_DYNAMIC(CDrawHistDlg, CDialog)

CDrawHistDlg::CDrawHistDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDrawHistDlg::IDD, pParent)
	, m_pixel(0)
	, m_num(0)
	, isLine(FALSE)
	, nLineX(0)
	, m_ratio(0)
{
	memset(m_Tb, 0, sizeof(m_Tb));
}

CDrawHistDlg::~CDrawHistDlg()
{
}

void CDrawHistDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_PXVALUE, m_pixel);
	DDX_Text(pDX, IDC_PXNUM, m_num);
}


BEGIN_MESSAGE_MAP(CDrawHistDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_BN_CLICKED(IDOK, &CDrawHistDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BTN_REDRAW, &CDrawHistDlg::OnBnClickedBtnRedraw)
END_MESSAGE_MAP()


// CDrawHistDlh ��Ϣ�������
void CDrawHistDlg::SetHistValue(cv::Mat src)
{
	int nCol = src.cols * src.channels();		//ÿ�е����ظ���
	for (int i = 0; i < src.rows; i++)
	{
		uchar* pData = src.ptr<uchar>(i);
		for (int j = 0; j < nCol; ++j)
		{
			m_Tb[src.at<uchar>(i,j)] += 1;		//����ÿ�����صĸ���
		}
	}
}


/***************************************************************************************
Function: CDrawHistDlg�����ʼ������
Input:    NULL
Output:   BOOL
Description: NULL
Return:   BOOL
Others:   ͼ��Ĵ�С��Hist�ĺ����Ȳ���ȫһ�£���ý�ͼƬ�Ļ����ϵı����ͳ����е���������
          ��ȫһ�£����ӽ����������
***************************************************************************************/
BOOL CDrawHistDlg::OnInitDialog()
{
	CDialog::OnInitDialog();								//�����ʼ������
	HICON m_hIcon = AfxGetApp()->LoadIcon(IDI_HIST);	//���ô���Icon
	SetIcon(m_hIcon, FALSE);

	m_MemDC.LoadBitmap("res\\hist.bmp");					//���ļ��м�����Դ��DC��
	CStatic *pHist = (CStatic*)GetDlgItem(IDC_HIST_RECT);
	pHist->GetClientRect(rc_Hist);							//��ȡͼ��ؼ��ߴ�

	//��ֱ��ͼ���в��ҵ����ֵ, �������ֵ��ȷ����ͼ�ı���
	int maxValue = 0;
	for(int i = 0; i < 256; i++)
	{
		if (m_Tb[i] > maxValue)
			maxValue = m_Tb[i];
	}
	m_ratio  = maxValue / rc_Hist.Height();   
	return TRUE;
}


/***************************************************************************************
Function: CDrawHistDlg�����ͼ����
Input:    NULL
Output:   void
Description: ��ֱ��ͼ�����ڻ���ֱ��ͼ
Return:   void
Others:   NULL
***************************************************************************************/
void CDrawHistDlg::OnPaint()
{
	CPaintDC dc(this);
	
	//������ͼ,��ֱ��ͼ����ͼ���Ƶ�������
	dc.BitBlt(0, 0, m_MemDC.Width(), m_MemDC.Height(),&m_MemDC,0,0,SRCCOPY);
	//��ؼ���λ�ã��ڿؼ��Ͻ�ֱ��ͼ���Ƴ���
	CStatic *pHist = (CStatic*)GetDlgItem(IDC_HIST_RECT);
	pHist->GetClientRect(rc_Hist);			//��ȡͼ��ؼ��ߴ�
	CDC *pDC = pHist->GetDC();				//��ȡ�ؼ�dc
	int bar = rc_Hist.Width() / 256;		//ÿһ����֮��ļ��
	CPen pen(PS_SOLID, 2, RGB(0, 0, 255));	//����Pen
	pDC->SelectObject(pen);
	
	//����ֱ��ͼ����
	for (int i = 0; i < 256; i++)
	{
		pDC->MoveTo(rc_Hist.left + bar*i + 5, rc_Hist.bottom);  //
		pDC->LineTo(rc_Hist.left + bar*i + 5, rc_Hist.bottom - m_Tb[i]/m_ratio);
	}

	//�жϽ������Ƿ���Ҫ���������ʱ�������,������Ƴ���
	CPen pen2(PS_DOT, 1, RGB(255, 0, 0));		//����Pen
	pDC->SelectObject(pen2);
	if (isLine)
	{
		pDC->MoveTo(nLineX, rc_Hist.bottom);	//
		pDC->LineTo(nLineX, rc_Hist.top);
	}
	
	ReleaseDC(pDC);
}


/***************************************************************************************
Function: �ڴ���������������ʱ�����Ϣ
Input:    NULL
Output:   void
Description: 
Return:   void
Others:   NULL
***************************************************************************************/
void CDrawHistDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	//ת����ֱ��ͼ�ؼ��е�����ϵ
	CRect rc;
	GetDlgItem(IDC_HIST_RECT)->GetWindowRect(&rc);
	ScreenToClient(rc);
	if (!rc.PtInRect(point))
		return;

	//����ѡ��������ֱ��ͼ�ĸ߶���ֵ
	m_pixel = (point.x - rc.left) / (rc.Width()/256);
	if (m_pixel > 255)
		m_pixel = 255;
	m_num = m_Tb[m_pixel];
	UpdateData(FALSE);					//��ֵ���µ�����ؼ���

	isLine = TRUE;
	nLineX = point.x - rc.left + 5;
	Invalidate(FALSE);
	CDialog::OnLButtonDown(nFlags, point);
}


/***************************************************************************************
Function: ���水����ȷ�ϡ���Ϣ��Ӧ
Input:    NULL
Output:   void
Description: 
Return:   void
Others:   NULL
***************************************************************************************/
void CDrawHistDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);		//�ؼ���ֵ���µ�������
	if (m_pixel > 255)
		m_pixel = 255;
	m_num = m_Tb[m_pixel];
	isLine = TRUE;
	CStatic *pHist = (CStatic*)GetDlgItem(IDC_HIST_RECT);
	pHist->GetClientRect(rc_Hist);			//��ȡͼ��ؼ��ߴ�
	int bar = rc_Hist.Width() / 256;		//ÿһ����֮��ļ��
	nLineX = m_pixel*bar + 5;				//��ȡ5��ƫ����
	Invalidate(FALSE);
	UpdateData(FALSE);						//��ֵ���µ�����ؼ���
}


/***************************************************************************************
Function: ���水����ȥ���ػ�����Ϣ��Ӧ
Input:    NULL
Output:   void
Description: 
Return:   void
Others:   NULL
***************************************************************************************/
void CDrawHistDlg::OnBnClickedBtnRedraw()
{
	int maxValue = 0;
	m_Tb[0] = 0;     //ȥ������Ϊ��ĵ�
	for(int i = 1; i < 256; i++)
	{
		if (m_Tb[i] > maxValue)
			maxValue = m_Tb[i];
	}
	m_ratio  = maxValue / rc_Hist.Height();
	Invalidate(FALSE);
}


