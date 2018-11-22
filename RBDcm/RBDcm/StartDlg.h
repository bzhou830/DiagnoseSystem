#pragma once
#include "RBMermoryDC.h"


// CStartDlg �Ի���
class CStartDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CStartDlg)

public:
	CStartDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CStartDlg();

// �Ի�������
	enum { IDD = IDD_DLG_START };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
private:
	CRBMemoryDC m_MemDc;

public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	CPoint m_point;
	int m_nWidth, m_nHeight;
	int m_nDx, m_nDy;
	int m_nDx1, m_nDy1;
};


