#pragma once
#include <list>
#include <string>
#include "Global.h"
#include "afxcmn.h"
#include "afxwin.h"
using namespace std;

class CSaveImgDlg : public CDialog
{
private:
	list<sOneImg> &m_lt;
	string m_path;
	bool m_bpathok;
public:
	enum {SRC_IMG, SEG_IMG, NOD_IMG} tp;
	enum IMG_BIT {BIT8U, BIT16U} bt;

	DECLARE_DYNAMIC(CSaveImgDlg)
public:
	CSaveImgDlg(list<sOneImg> &im, CWnd* pParent = NULL);//标准构造函数
	virtual ~CSaveImgDlg();

// 对话框数据
	enum { IDD = IDD_DLG_SAVE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    //DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButton1();
	CProgressCtrl m_proctrl;
	BOOL m_rd;
	BOOL m_rdb8;
};
