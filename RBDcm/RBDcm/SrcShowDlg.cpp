// SrcShowDlg.cpp : 实现文件
#include "stdafx.h"
#include "RBDcm.h"
#include "SrcShowDlg.h"
#include "afxdialogex.h"


// CSrcShowDlg对话框
IMPLEMENT_DYNAMIC(CSrcShowDlg, CDialogEx)

CSrcShowDlg::CSrcShowDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSrcShowDlg::IDD, pParent)
{
}

CSrcShowDlg::~CSrcShowDlg()
{
}

void CSrcShowDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CSrcShowDlg, CDialogEx)
	ON_WM_PAINT()
END_MESSAGE_MAP()


// CSrcShowDlg 消息处理程序
BOOL CSrcShowDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	if (!m_Img.empty())
	{
		this->MoveWindow(0, 0, m_Img.rows + 10, m_Img.cols + 30);
		this->SetWindowText("原图像");
	}
	CenterWindow();
	return TRUE;
}


//复制图像数据到显示窗口类成员变量
void CSrcShowDlg::SetImgData(Mat srcImg)
{
	this->m_Img = srcImg.clone();
}


void CSrcShowDlg::OnPaint()
{
	CPaintDC dc(this);
	//底色填充为纯黑色
	CRect rc;
	this->GetWindowRect(&rc);
	CBrush br(RGB(0, 0, 0));
	dc.SelectObject(br);
	dc.FillRect(rc, &br);
	//非空时绘制图像
	if (!m_Img.empty())
	{
		CImage ShowImg;
		CImgFormat::Mat2CImage(m_Img, ShowImg);
		ShowImg.Draw(dc.m_hDC, 0, 0);
	}
}


