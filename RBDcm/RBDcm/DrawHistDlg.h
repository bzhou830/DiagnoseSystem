/**********************************************************************************
  Copyright @ Robin 2015-2025
  File name:  DrawHistDlg.h
  Author: Robin   Version: V1.0   Date:2015.11.05 
  Description: dialog class about Draw image Hist. 
  Others:         
***********************************************************************************/
#pragma once

#include "RBMermoryDC.h"

// CDrawHistDlh 对话框
class CDrawHistDlg : public CDialog
{
	DECLARE_DYNAMIC(CDrawHistDlg)
public:
	CDrawHistDlg(CWnd* pParent = NULL);   				// 标准构造函数
	virtual ~CDrawHistDlg();

	enum { IDD = IDD_HIST_DLG };  						// 对话框数据
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:
	void SetHistValue(cv::Mat src);	//set pixels table value

private:
	int m_Tb[256];					//pixels table
	CRect rc_Hist;					//hist rect
	int m_pixel;					//selected pixle value
	int m_num;						//selected pixle's numbers in the table
	CRBMemoryDC m_MemDC;			//memeroy dc
	BOOL isLine;					//is Line should be drawed
	int nLineX;						//position of line's x coodinate
	int m_ratio;					//ratio about pixles and real interface

public:
	virtual BOOL OnInitDialog();	
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedBtnRedraw();
};
