#pragma once

#include "ImgFormat.h"
// CSrcShowDlg �Ի���

class CSrcShowDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSrcShowDlg)
private:
	Mat m_Img;   //Ҫ��ʾ�������е�ͼ��
public:
	CSrcShowDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CSrcShowDlg();

// �Ի�������
	enum { IDD = IDD_SRC_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	void SetImgData(Mat srcImg);
	afx_msg void OnPaint();
};
