#pragma once

#include "ImgFormat.h"
// CSrcShowDlg 对话框

class CSrcShowDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSrcShowDlg)
private:
	Mat m_Img;   //要显示到窗口中的图像
public:
	CSrcShowDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSrcShowDlg();

// 对话框数据
	enum { IDD = IDD_SRC_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	void SetImgData(Mat srcImg);
	afx_msg void OnPaint();
};


