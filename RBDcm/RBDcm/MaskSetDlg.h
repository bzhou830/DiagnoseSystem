#pragma once


//�任�˽ṹ��
struct Maskmatrix
{
	int mk[3][3];
	int weight;
};


// CMaskSetDlg �Ի���
class CMaskSetDlg : public CDialog
{
	DECLARE_DYNAMIC(CMaskSetDlg)

public:
	CMaskSetDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CMaskSetDlg();

// �Ի�������
	enum { IDD = IDD_DLG_MASK };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	Maskmatrix m_mask;
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedBtnLap4();
	afx_msg void OnBnClickedBtnLap8();
	afx_msg void OnBnClickedBtnRobertsx();
	afx_msg void OnBnClickedBtnRobertsy();
	afx_msg void OnBnClickedBtnSobelx();
	afx_msg void OnBnClickedBtnSobely();
	afx_msg void OnBnClickedBtnPrewittx();
	afx_msg void OnBnClickedBtnPrewitty();
	afx_msg void OnBnClickedBtnAverange();
	virtual BOOL OnInitDialog();
};
