/**********************************************************************************
  Copyright @ Robin 2015-2025
  File name:  MainFrm.h
  Author: Robin   Version: V1.0   Date: 2015.11.05 
  Description: application framework class 
               MFC structure
  Others:         
***********************************************************************************/

#pragma once

#include "RBDcmView.h"
#include "OprtView.h"
#include "SegView.h"
#include "ClassifierView.h"
#include "InfoView.h"
#include "MySplitterWndEx.h"
#include "SplitterWndExt.h"
#include "AllImgView.h"
#include "MatImg.h"
#include "SrcShowDlg.h"
#include "SegmentOperat.h"
#include "EnhanceOperat.h"
#include "OneImgFeatures.h"


const UINT UM_PROSSESS = WM_USER + 1;
class COprtView;
//class CSplitterWndExt;

class CMainFrame : public CFrameWnd
{
public:
	CImgSerial  m_ImgSerial;                //Mat-image structure
	CPlaySeriesDlg* m_PlayDlg;
public:
	CMySplitterWndEx	m_splt;				//左侧分割栏
	CMySplitterWndEx	m_splt2;			//右侧分割栏
	CSliderCtrl			m_slider;			//状态栏滚动条
	COprtView*			m_pOpt;				//操作面板窗口
	CRBDcmView*			m_pRBView;			//左上图像显示窗口
	CSegView*			m_pSegView;			//右上图像显示窗口
	CClassifierView*	m_pClassier;		//左下图像显示窗口
	CInfoView*			m_pInfoView;		//右下图像显示窗口
	BOOL                m_isLeftPaneMin;    //左边面板是否被收缩
	CToolBar            m_toolBar;			//工具栏
protected: // 仅从序列化创建
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)
// 特性
public:

// 操作
public:
	CStatusBar* GetStatusBar(){return &m_wndStatusBar;};

// 重写
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
// 实现
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:				// 控件条嵌入成员
	CStatusBar   m_wndStatusBar;
// 生成的消息映射函数
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg LRESULT OnProgress(WPARAM, LPARAM);
	DECLARE_MESSAGE_MAP()

	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	void SetMainFrm(int num);
	int ResetFrm(void);
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	afx_msg void OnBtnReset();
	afx_msg void OnBtnZoom();
	afx_msg void OnBtnSrcimg();
	afx_msg void OnBtnPlayimgs();
	afx_msg void OnBtnNextimg();
	afx_msg void OnBtnLoadxml();
	afx_msg void OnBtnLastimg();
	afx_msg void OnBtnHistimg();
	afx_msg void OnBtnCamera();
	afx_msg void OnSerileOpen();
	afx_msg void OnFileOpenOne();
	afx_msg void OnBtnEntropy();
	afx_msg void OnBtnOtsu();
	afx_msg void OnBtnReallung();
//	afx_msg void OnBtnNudoul();
	afx_msg void OnBtnNudoul();
	afx_msg void OnBtnMom();
	afx_msg void OnBtnMean();
};

