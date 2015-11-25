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

// CDrawHistDlh 对话框
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


// CDrawHistDlh 消息处理程序
void CDrawHistDlg::SetHistValue(cv::Mat src)
{
	int nCol = src.cols * src.channels();		//每行的像素个数
	for (int i = 0; i < src.rows; i++)
	{
		uchar* pData = src.ptr<uchar>(i);
		for (int j = 0; j < nCol; ++j)
		{
			m_Tb[src.at<uchar>(i,j)] += 1;		//计算每个像素的个数
		}
	}
}


/***************************************************************************************
Function: CDrawHistDlg窗体初始化函数
Input:    NULL
Output:   BOOL
Description: NULL
Return:   BOOL
Others:   图像的大小和Hist的横向宽度不完全一致，最好将图片的画面上的比例和程序中的整数保持
          完全一致，增加界面的美观性
***************************************************************************************/
BOOL CDrawHistDlg::OnInitDialog()
{
	CDialog::OnInitDialog();								//父类初始化函数
	HICON m_hIcon = AfxGetApp()->LoadIcon(IDI_HIST);	//设置窗口Icon
	SetIcon(m_hIcon, FALSE);

	m_MemDC.LoadBitmap("res\\hist.bmp");					//从文件中加载资源到DC中
	CStatic *pHist = (CStatic*)GetDlgItem(IDC_HIST_RECT);
	pHist->GetClientRect(rc_Hist);							//获取图像控件尺寸

	//从直方图表中查找到最大值, 根据最大值来确定画图的比例
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
Function: CDrawHistDlg窗体绘图函数
Input:    NULL
Output:   void
Description: 在直方图窗体内绘制直方图
Return:   void
Others:   NULL
***************************************************************************************/
void CDrawHistDlg::OnPaint()
{
	CPaintDC dc(this);
	
	//界面贴图,将直方图背景图绘制到窗体上
	dc.BitBlt(0, 0, m_MemDC.Width(), m_MemDC.Height(),&m_MemDC,0,0,SRCCOPY);
	//获控件的位置，在控件上将直方图绘制出来
	CStatic *pHist = (CStatic*)GetDlgItem(IDC_HIST_RECT);
	pHist->GetClientRect(rc_Hist);			//获取图像控件尺寸
	CDC *pDC = pHist->GetDC();				//获取控件dc
	int bar = rc_Hist.Width() / 256;		//每一条线之间的间距
	CPen pen(PS_SOLID, 2, RGB(0, 0, 255));	//划线Pen
	pDC->SelectObject(pen);
	
	//绘制直方图线条
	for (int i = 0; i < 256; i++)
	{
		pDC->MoveTo(rc_Hist.left + bar*i + 5, rc_Hist.bottom);  //
		pDC->LineTo(rc_Hist.left + bar*i + 5, rc_Hist.bottom - m_Tb[i]/m_ratio);
	}

	//判断界面上是否需要绘制鼠标点击时候的线条,有则绘制出来
	CPen pen2(PS_DOT, 1, RGB(255, 0, 0));		//划线Pen
	pDC->SelectObject(pen2);
	if (isLine)
	{
		pDC->MoveTo(nLineX, rc_Hist.bottom);	//
		pDC->LineTo(nLineX, rc_Hist.top);
	}
	
	ReleaseDC(pDC);
}


/***************************************************************************************
Function: 在窗体上鼠标左键按下时候的消息
Input:    NULL
Output:   void
Description: 
Return:   void
Others:   NULL
***************************************************************************************/
void CDrawHistDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	//转换到直方图控件中的坐标系
	CRect rc;
	GetDlgItem(IDC_HIST_RECT)->GetWindowRect(&rc);
	ScreenToClient(rc);
	if (!rc.PtInRect(point))
		return;

	//查找选中像素在直方图的高度数值
	m_pixel = (point.x - rc.left) / (rc.Width()/256);
	if (m_pixel > 255)
		m_pixel = 255;
	m_num = m_Tb[m_pixel];
	UpdateData(FALSE);					//数值更新到界面控件内

	isLine = TRUE;
	nLineX = point.x - rc.left + 5;
	Invalidate(FALSE);
	CDialog::OnLButtonDown(nFlags, point);
}


/***************************************************************************************
Function: 界面按键“确认”消息响应
Input:    NULL
Output:   void
Description: 
Return:   void
Others:   NULL
***************************************************************************************/
void CDrawHistDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);		//控件数值更新到程序中
	if (m_pixel > 255)
		m_pixel = 255;
	m_num = m_Tb[m_pixel];
	isLine = TRUE;
	CStatic *pHist = (CStatic*)GetDlgItem(IDC_HIST_RECT);
	pHist->GetClientRect(rc_Hist);			//获取图像控件尺寸
	int bar = rc_Hist.Width() / 256;		//每一条线之间的间距
	nLineX = m_pixel*bar + 5;				//留取5个偏移量
	Invalidate(FALSE);
	UpdateData(FALSE);						//数值更新到界面控件内
}


/***************************************************************************************
Function: 界面按键“去零重画”消息响应
Input:    NULL
Output:   void
Description: 
Return:   void
Others:   NULL
***************************************************************************************/
void CDrawHistDlg::OnBnClickedBtnRedraw()
{
	int maxValue = 0;
	m_Tb[0] = 0;     //去掉像素为零的点
	for(int i = 1; i < 256; i++)
	{
		if (m_Tb[i] > maxValue)
			maxValue = m_Tb[i];
	}
	m_ratio  = maxValue / rc_Hist.Height();
	Invalidate(FALSE);
}


