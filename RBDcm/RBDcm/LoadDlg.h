#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// CLoadDlg �Ի���

class CLoadDlg : public CDialog
{
	DECLARE_DYNAMIC(CLoadDlg)

public:
	CLoadDlg(CWnd* pParent = NULL);						// ��׼���캯��
	virtual ~CLoadDlg();

// �Ի�������
	enum { IDD = IDD_LOAD_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CProgressCtrl m_Progress;
	virtual BOOL OnInitDialog();
	void SetProcess(int cur);
	void SetSum(int sum);
	CStatic m_txtSc;
	void SetText(CString str);
};
