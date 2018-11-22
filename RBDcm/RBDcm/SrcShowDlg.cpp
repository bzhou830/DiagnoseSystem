// SrcShowDlg.cpp : ʵ���ļ�
#include "stdafx.h"
#include "RBDcm.h"
#include "SrcShowDlg.h"
#include "afxdialogex.h"


// CSrcShowDlg�Ի���
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


// CSrcShowDlg ��Ϣ�������
BOOL CSrcShowDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	if (!m_Img.empty())
	{
		this->MoveWindow(0, 0, m_Img.rows + 10, m_Img.cols + 30);
		this->SetWindowText("ԭͼ��");
	}
	CenterWindow();
	return TRUE;
}


//����ͼ�����ݵ���ʾ�������Ա����
void CSrcShowDlg::SetImgData(Mat srcImg)
{
	this->m_Img = srcImg.clone();
}


void CSrcShowDlg::OnPaint()
{
	CPaintDC dc(this);
	//��ɫ���Ϊ����ɫ
	CRect rc;
	this->GetWindowRect(&rc);
	CBrush br(RGB(0, 0, 0));
	dc.SelectObject(br);
	dc.FillRect(rc, &br);
	//�ǿ�ʱ����ͼ��
	if (!m_Img.empty())
	{
		CImage ShowImg;
		CImgFormat::Mat2CImage(m_Img, ShowImg);
		ShowImg.Draw(dc.m_hDC, 0, 0);
	}
}


