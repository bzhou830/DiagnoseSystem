#pragma once
#include "afxwin.h"
#include <vector>
#include <list>
#include "afxcmn.h"
using namespace std;

// CTransVideoDlg �Ի���
class CTransVideoDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CTransVideoDlg)

public:
	CTransVideoDlg(list<sOneImg> &lt, CWnd* pParent = NULL);				// ��׼���캯��
	virtual ~CTransVideoDlg();

	enum { IDD = IDD_DIALOG_VIDEO };
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
private:
	vector<pair<CString,string> > lt;
	list<sOneImg> &m_lt;
public:
	CComboBox m_comb;
	int m_nWidth;
	int m_nHeight;
	double m_nFps;
	BOOL m_radio_gray;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedRadioVideoGray();
	afx_msg void OnBnClickedRadioVideoRgb();
	afx_msg void OnBnClickedButtonStarttrans();
	CProgressCtrl m_proctrl;
};
