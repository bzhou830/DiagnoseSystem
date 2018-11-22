#pragma once
#include <vector>
using namespace std;

// CAIPDlg �Ի���
class CAIPDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CAIPDlg)
public:
	CAIPDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CAIPDlg();

// �Ի�������
	enum { IDD = IDD_DLG_AIP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnPaint();
	void SetImgSerial(vector<Mat>::iterator it, int n);
	vector<Mat>::iterator m_itbeg;
	vector<Mat>::iterator m_itcur;
	int m_count;
	int m_CurNum;
	afx_msg void OnBnClickedBtnNext();
	afx_msg void OnBnClickedBtnLast();
};

