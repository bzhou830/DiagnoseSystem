#pragma once


// CWndWidthPosDlg �Ի���

class CWndWidthPosDlg : public CDialog
{
	DECLARE_DYNAMIC(CWndWidthPosDlg)

public:
	CWndWidthPosDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CWndWidthPosDlg();

// �Ի�������
	enum { IDD = IDD_DLG_WINDOWWIDTH };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
};
