#pragma once

// CSetValueDlg �Ի���
class CSetValueDlg : public CDialog
{
	DECLARE_DYNAMIC(CSetValueDlg)

public:
	CSetValueDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CSetValueDlg();

// �Ի�������
	enum { IDD = IDD_VALUE_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
private:
	int m_value;                     //�����е��趨ֵ
	CString m_strWindowText;         //���ڱ���
public:

	int GetValue(void);              //��ȡ�����е��趨ֵ
	void SetTitle(CString strText);  //���ô��ڱ���
	virtual BOOL OnInitDialog();     //���ڳ�ʼ��
	void SetValue(int vl);			 //���ô�����edit�ؼ���ֵ
};
