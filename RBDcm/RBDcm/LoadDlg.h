#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// CLoadDlg 对话框

class CLoadDlg : public CDialog
{
	DECLARE_DYNAMIC(CLoadDlg)

public:
	CLoadDlg(CWnd* pParent = NULL);						// 标准构造函数
	virtual ~CLoadDlg();

// 对话框数据
	enum { IDD = IDD_LOAD_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CProgressCtrl m_Progress;
	virtual BOOL OnInitDialog();
	void SetProcess(int cur);
	void SetSum(int sum);
	CStatic m_txtSc;
	void SetText(CString str);
};
