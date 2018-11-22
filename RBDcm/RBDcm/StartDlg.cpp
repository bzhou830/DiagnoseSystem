// StartDlg.cpp : ʵ���ļ�
#include "stdafx.h"
#include "StartDlg.h"
#include "afxdialogex.h"

// CStartDlg �Ի���
IMPLEMENT_DYNAMIC(CStartDlg, CDialogEx)

CStartDlg::CStartDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CStartDlg::IDD, pParent)
{
}


CStartDlg::~CStartDlg()
{
}


void CStartDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CStartDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CStartDlg ��Ϣ�������
BOOL CStartDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	//��ô���Ԥ��Ĵ�С
	CRect dlgRect;
	GetWindowRect(dlgRect);
	CRect desktopRect;
	//�����ڿ�ʼ��С��Ϊ0
	GetDesktopWindow()->GetWindowRect(desktopRect);
	MoveWindow((desktopRect.Width() - dlgRect.Width())/2,
				(desktopRect.Height() - dlgRect.Height())/2,
				0,0);
	//��ʼ���仯��С
	m_nWidth = dlgRect.Width();
	m_nHeight = dlgRect.Height();
	m_nDx = 3;
	m_nDy = 2;
	m_nDx1 = 2;
	m_nDy1 = 2;
	SetTimer(1, 10, NULL);			//�趨��ʱ��1	
	return TRUE;
}


void CStartDlg::OnPaint()
{
	CPaintDC dc(this); 
	dc.BitBlt(0,0,m_MemDc.Width(),m_MemDc.Height(),&m_MemDc,0,0,SRCCOPY);
}


void CStartDlg::OnTimer(UINT_PTR nIDEvent)
{
	CRect dlgRect;
	GetWindowRect(dlgRect);
	//�������Ĵ�С
	CRect desktopRect;
	GetDesktopWindow()->GetWindowRect(desktopRect);
	//����Ǵ��ڵ������̣��������󴰿�
	if(nIDEvent == 1)
	{
		MoveWindow(
			(-m_nDx+desktopRect.Width() - dlgRect.Width()) / 2,
			(-m_nDy+desktopRect.Height() - dlgRect.Height()) / 2, 
			+m_nDx+dlgRect.Width(),
			+m_nDy+dlgRect.Height());

		//��Ҫ��������Ԥ��Ŀ��
		if(dlgRect.Width() >= m_nWidth) 
			m_nDx=0; 
		//��Ҫ��������Ԥ��ĸ߶�
		if(dlgRect.Height() >= m_nHeight)
			m_nDy=0;
		//ֹͣ�仯���رն�ʱ��1
		if((dlgRect.Width() >= m_nWidth) && (dlgRect.Height() >= m_nHeight))
			KillTimer(1); 
	}

	//ֹͣ�仯���رն�ʱ��1
	if((dlgRect.Width() >= m_nWidth) && (dlgRect.Height() >= m_nHeight))
	{
		KillTimer(1);
		SetTimer(3,1000,NULL);
	}

	//����Ǵ��ڹرչ��̣�������С����
	if(nIDEvent == 2)
	{
		MoveWindow((+m_nDx+desktopRect.Width() - dlgRect.Width()) / 2,
			(+m_nDy+desktopRect.Height() - dlgRect.Height()) / 2,
			-m_nDx1+dlgRect.Width(),
			-m_nDy1+dlgRect.Height() );
		//����ȵ�������ȾͲ��ڱ仯
		if(dlgRect.Width() <= 0) 
			m_nDx1=0;
		//���߶ȵ������߶ȾͲ��ڱ仯
		if(dlgRect.Height() <= 0 )
			m_nDy1=0; 
		//ֹͣ�仯���رն�ʱ��2�����ҹرմ���
		if((dlgRect.Width() <= 0 ) && (dlgRect.Height() <=0))
		{
			KillTimer(2);
			CDialog::OnOK();
		}
	}
	if(nIDEvent == 3)
	{
		static int n=0;
		if(n==2)
			OnOK();
		n++;
	}
	CDialogEx::OnTimer(nIDEvent);
}
