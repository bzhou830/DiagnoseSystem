/**********************************************************************************
  Copyright @ Robin 2015-2025
  File name:  OprtView.h
  Author: Robin   Version: V1.0   Date:2015.11.05 
  Description:  operate form interface class
  Others:         
***********************************************************************************/
#pragma once
#include "iconbutton.h"
#include "MainFrm.h"
#include "afxwin.h"
#include "DrawHistDlg.h"
#include "MaskSetDlg.h"


class CMainFrame;

class COprtView : public CFormView
{
public:
	CImgSerial  m_ImgSerial;                //Mat-image structure
	CMainFrame* m_pMainFrm;
private:
	DECLARE_DYNCREATE(COprtView)
protected:
	COprtView();           
	virtual ~COprtView();

public:
	enum { IDD = IDC_DLG_OPT };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
	afx_msg void OnBnClickedBtnHist();
	afx_msg void OnBnClickedBtnCut();
	afx_msg void OnBnClickedBtnRst();
	afx_msg void OnBnClickedBtnMask();
	afx_msg void OnBnClickedBtnMedian();
	afx_msg void OnBnClickedBtnDrawhist();
	afx_msg void OnBnClickedBtnEntropy();
	afx_msg void OnBnClickedBtnOstu();
	afx_msg void OnBnClickedBtnHence();
	afx_msg void OnBnClickedBtnSrcimg();
	afx_msg void OnBnClickedBtnLung();
	afx_msg void OnBnClickedBtnVideo();
	afx_msg void OnBnClickedBtnPlay();
	afx_msg void OnBnClickedBtnLung3();
	afx_msg void OnBnClickedBtnLast();

	CIconButton m_reset;        //图像复原按钮
	CIconButton m_btnHist;      //绘制直方图按钮
	CIconButton m_btnEnhance;   //Hist增强按钮
	CIconButton m_btnCut;       //手动阈值分割按钮
	CIconButton m_btnMask;		//
	CIconButton m_btnMed;
	CIconButton m_btnEntropy;
	CIconButton m_btnOSTU;
	CIconButton m_btnHence;
	CIconButton m_btnSrc;
	CIconButton m_btnLung;
	CIconButton m_btnPlay;
	CIconButton m_btnLast;
	CIconButton m_btnNext;
	CIconButton m_btnCam;
	CIconButton m_btnMean;
	CIconButton m_btnMoM;
	CIconButton m_btnLoadXML;
	
	void Executive(int msg);
	int m_nSUM;
	int m_nCurrent;
	//CRBDcmView* m_pRDBView;
	//CRBDcmDoc* m_pDoc;
	void SetSum(int num);
	void SetCurrent(int num);
	afx_msg void OnBnClickedBtnNext();
	afx_msg void OnBnClickedBtnMean();
	afx_msg void OnBnClickedBtnMom();
	afx_msg void OnBnClickedBtnLoadxml();
	afx_msg void OnSerile();
	afx_msg void OnFileOpen();
	afx_msg void OnFileSave();
};


