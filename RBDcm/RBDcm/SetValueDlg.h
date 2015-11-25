#pragma once

// CSetValueDlg 对话框
class CSetValueDlg : public CDialog
{
	DECLARE_DYNAMIC(CSetValueDlg)

public:
	CSetValueDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSetValueDlg();

// 对话框数据
	enum { IDD = IDD_VALUE_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
private:
	int m_value;                     //窗口中的设定值
	CString m_strWindowText;         //窗口标题
public:

	int GetValue(void);              //获取窗口中的设定值
	void SetTitle(CString strText);  //设置窗口标题
	virtual BOOL OnInitDialog();     //窗口初始化
	void SetValue(int vl);			 //设置窗口中edit控件数值
};


