// AIPDlg.cpp : 实现文件
#include "stdafx.h"
#include "AIPDlg.h"
#include "afxdialogex.h"


// CAIPDlg 对话框
IMPLEMENT_DYNAMIC(CAIPDlg, CDialogEx)

CAIPDlg::CAIPDlg(CWnd* pParent)
	: CDialogEx(CAIPDlg::IDD, pParent)
{
}

CAIPDlg::~CAIPDlg()
{
}

void CAIPDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAIPDlg, CDialogEx)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BTN_NEXT, &CAIPDlg::OnBnClickedBtnNext)
	ON_BN_CLICKED(IDC_BTN_LAST, &CAIPDlg::OnBnClickedBtnLast)
END_MESSAGE_MAP()

void CAIPDlg::OnPaint()
{
	CPaintDC dc(this);															// device context for painting
	CRect rect;
	GetDlgItem(IDC_PICTURE)->GetClientRect(&rect);								//图像显示区域rect
	CDC *pDC = GetDlgItem(IDC_PICTURE)->GetDC();								//图像显示区域DC
	CImage img;
	Mat dst;
	resize(*m_itcur, dst, Size(rect.Width(), rect.Height()), 0, 0, INTER_CUBIC);
	CImgFormat::Mat2CImage_u8(dst, img);
	img.Draw(pDC->m_hDC, 0, 0);
	
	CFont ft;
	CString str;
	pDC->SetTextColor(RGB(255, 128, 0));
	pDC->SetBkMode(TRANSPARENT);
	ft.CreateFont(24, 0, 0, 0, FW_MEDIUM, FALSE, FALSE, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, "courier");  //创建字体
	pDC->SelectObject(ft);
	str.Format("%d/%d",m_CurNum+1, m_count);
	pDC->DrawText(str, &rect, DT_LEFT);

	pDC->DeleteDC();
}

void CAIPDlg::SetImgSerial(vector<Mat>::iterator it, int n)
{
	m_itbeg = it;
	m_itcur = it;
	m_count = n;
	m_CurNum = 0;
	SendMessage(WM_PAINT);
}

void CAIPDlg::OnBnClickedBtnNext()
{
	m_CurNum++;
	if (m_CurNum < m_count)
		m_itcur++;
	else
		m_CurNum--;
	SendMessage(WM_PAINT);
}


void CAIPDlg::OnBnClickedBtnLast()
{
	m_CurNum--;
	if (m_CurNum >= 0)
		m_itcur--;
	else
		m_CurNum++;
	SendMessage(WM_PAINT);
}
