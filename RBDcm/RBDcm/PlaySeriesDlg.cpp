/**********************************************************************************
  Copyright @ Robin 2015-2025
  File name:  PlaySeriesDlg.h
  Author: Robin   Version: V1.0   Date: 2015.11.05 
  Description:  Play Serial Dialog Class
  Others:         
***********************************************************************************/
#include "stdafx.h"
#include "RBDcm.h"
#include "PlaySeriesDlg.h"
#include "afxdialogex.h"


// CPlaySeriesDlg 对话框
IMPLEMENT_DYNAMIC(CPlaySeriesDlg, CDialogEx)

CPlaySeriesDlg::CPlaySeriesDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CPlaySeriesDlg::IDD, pParent)
{
	isPlay = FALSE;
	m_count = 0;
	m_CurNum = 0;
}


CPlaySeriesDlg::~CPlaySeriesDlg()
{
}

void CPlaySeriesDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SLIDER_SP, m_Slider);
}


BEGIN_MESSAGE_MAP(CPlaySeriesDlg, CDialogEx)
	ON_WM_TIMER()
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BTN_PLAY, &CPlaySeriesDlg::OnBnClickedBtnPlay)
	ON_BN_CLICKED(IDC_BTN_STOP, &CPlaySeriesDlg::OnBnClickedBtnStop)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CPlaySeriesDlg 消息处理程序
void CPlaySeriesDlg::PostNcDestroy()
{
	delete this;
	CDialogEx::PostNcDestroy();
}


void CPlaySeriesDlg::SetImgSerial(vector<sOneImg>::iterator itr,int count)
{
	this->itr_bg = itr;
	this->itr = itr_bg;
	m_count = count;
	m_CurNum = 0;
}



BOOL CPlaySeriesDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	m_Slider.SetRange(1,10);
	m_Slider.SetTicFreq(1);
	m_Slider.SetPos(2);
	return TRUE;  
}


void CPlaySeriesDlg::OnTimer(UINT_PTR nIDEvent)
{
	switch (nIDEvent)
	{
	case TIMER0:
		{
			KillTimer(TIMER0);
			SetTimer(TIMER0,(12 - m_Slider.GetPos())*50, NULL);
			if (isPlay == TRUE && m_CurNum < m_count-1)
			{
				m_CurNum++;
				itr++;
				SendMessage(WM_PAINT);
			}
			else if (m_CurNum >= m_count-1)
			{
				itr = itr_bg;
				m_CurNum = 0;
				isPlay = FALSE;
			}
		}
		break;
	}
	CDialogEx::OnTimer(nIDEvent);
}


void CPlaySeriesDlg::OnPaint()
{
	CPaintDC dc(this); 
	CRect rect;
	GetDlgItem(IDC_PICTURE)->GetClientRect(&rect);  //图像显示区域rect
	CDC *pDC = GetDlgItem(IDC_PICTURE)->GetDC();    //图像显示区域DC
	if(!isPlay)
		return;
	CImage img;
	Mat dst;
	resize(itr->pixle, dst, Size(rect.Width(), rect.Height()), 0, 0, INTER_CUBIC);
	CImgFormat::Mat2CImage(dst, img);
	img.Draw(pDC->m_hDC, 0, 0);

	CString str;
	pDC->SetTextColor(RGB(255, 255, 0));
	pDC->SetBkMode(TRANSPARENT);
	CFont ft;
	ft.CreateFont(24, 0, 0, 0, FW_MEDIUM, FALSE, FALSE, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, "宋体");  //创建字体
	pDC->SelectObject(ft);
	str.Format("%d/%d",m_CurNum+1, m_count);
	pDC->DrawText(str, &rect, DT_LEFT);
	pDC->DeleteDC();
}


void CPlaySeriesDlg::OnBnClickedBtnPlay()
{
	SetTimer(TIMER0,(12 - m_Slider.GetPos())*50, NULL);
	isPlay = TRUE;
}


void CPlaySeriesDlg::OnBnClickedBtnStop()
{
	KillTimer(TIMER0);
	isPlay = FALSE;
}


void CPlaySeriesDlg::OnClose()
{
	KillTimer(TIMER0);
	isPlay = FALSE;
	m_count = 0;
	m_CurNum = 0;
	CDialogEx::OnClose();
}

