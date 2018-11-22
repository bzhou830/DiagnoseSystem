#pragma once


// CWndWidthPosDlg 对话框

class CWndWidthPosDlg : public CDialog
{
	DECLARE_DYNAMIC(CWndWidthPosDlg)

public:
	CWndWidthPosDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CWndWidthPosDlg();

// 对话框数据
	enum { IDD = IDD_DLG_WINDOWWIDTH };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
