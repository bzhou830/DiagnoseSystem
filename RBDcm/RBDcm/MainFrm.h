/**********************************************************************************
  Copyright @ Robin 2015-2025
  File name:  MainFrm.h
  Author: Robin   Version: V1.0   Date: 2015.11.05 
  Description: application framework class 
***********************************************************************************/
#pragma once

#include "RBDcmView.h"															//
#include "OprtView.h"															//
#include "SegView.h"															//
#include "ClassifierView.h"														//
#include "InfoView.h"															//专家标注信息窗口
#include "MySplitterWndEx.h"													//分割窗口
#include "SplitterWndExt.h"														//
#include "AllImgView.h"															//
#include "ImgSerial.h"															//图像数据转换
#include "SrcShowDlg.h"															//原图像显示窗口
#include "SegmentOperat.h"														//图像分割操作
#include "EnhanceOperat.h"														//图像增强操作
#include "OneImgFeatures.h"														//纵膈窗图像结节特征
#include "WndWidthPosDlg.h"														//窗宽窗位调整函数
#include "log/Log.h"															//写日志头文件
#include "SaveImgDlg.h"															//图像保存窗口
#include "LoadDlg.h"															//进度条窗口
#include "AIPDlg.h"
#include "Dock.h"


#include <iostream>

using namespace std;

const UINT UM_PROSSESS = WM_USER + 1;		//自定义消息
const UINT INFOWINDOWWIDTH = 200;			//信息显示框宽度

class COprtView;							//声明该类，以支持相互引用

class CMainFrame : public CFrameWnd
{
private:
	double              fSlice;
public:
	bool				m_isDisplayNote;	//是否显示专家标注信息
	CImgSerial			m_ImgSerial;        //Mat-image structure
	CPlaySeriesDlg*		m_PlayDlg;			//播放图像对话框
	CAIPDlg*			m_AipDlg;			//密度投影对话框
public:
	CMySplitterWndEx	m_splt;				//左侧分割栏
	CMySplitterWndEx	m_splt2;			//右侧分割栏
	CSliderCtrl			m_slider;			//状态栏滚动条
	COprtView*			m_pOpt;				//操作面板窗口
	CLUView*			m_pLUView;			//左上图像显示窗口
	CRUView*			m_pRUView;			//右上图像显示窗口
	CLDView*			m_pLDView;			//左下图像显示窗口
	CLDView*			m_pRDView;			//右下分类结果窗口
	CInfoView*			m_pInfoView;		//右下图像显示窗口
	BOOL                m_isLeftPaneMin;    //左边面板是否被收缩
	CToolBar            m_toolBarFunc;		//图像处理相关工具
	CToolBar            m_toolBarFile;		//文件操作相关工具
	CLog                m_Log;				//写日志对象
	CString             m_szAppPath;		//软件运行路
	//CvSVM				m_svmClassifier;	//SVM分类器
	static HANDLE		m_hMutex;			//互斥量
	static int			m_LoadStatus;
	bool				m_bCanClick;		//界面点击能否启用
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
	static DWORD WINAPI FunProc(LPVOID lp);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	void SetMainFrm(int num);
	int ResetFrm(void);
	inline BOOL IsHasImgs();
	void SetStateText(int num, CString szText);
	void ShowSagittal(int col, int line);
	void ShowCoronal(int row, int line);
	void lastimg();
	void nextimg();
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	afx_msg void OnUpdateDisplayNote(CCmdUI *pCmdUI);
	afx_msg void OnUpdateBtnClick(CCmdUI *pCmdUI);
	afx_msg void OnBtnAIP();
	afx_msg void OnBtnZoom();
	afx_msg void OnBtnSrcimg();
	afx_msg void OnBtnPlayimgs();
	afx_msg void OnBtnExtract();
	afx_msg void OnBtnLoadxml();
	afx_msg void OnBtnGMM();
	afx_msg void OnBtnHistimg();
	afx_msg void OnBtnCamera();
	afx_msg void OnSerileOpen();
	afx_msg void OnFileOpenOne();
	afx_msg void OnBtnEntropy();
	afx_msg void OnBtnOtsu();
	afx_msg void OnBtnVTK();
	afx_msg void OnBtnNudoul();
	afx_msg void OnBtnMom();
	afx_msg void OnBtnMean();
	afx_msg void OnSaveimg();
	afx_msg void OnBtnTest();
	afx_msg void OnTraining();
	afx_msg void OnWindow();
	afx_msg void OnSaveserial();
	afx_msg void OnTest();
	afx_msg void OnDisplayNote();
	afx_msg void OnBtnOpen();
	afx_msg void OnBtnSave();
	afx_msg void OnBtnSaveas();
	afx_msg void OnBtnClick();
	afx_msg void OnToolbarFileopen();
	afx_msg void OnToolbarFileprint();
	afx_msg void OnToolbarFilesave();
	afx_msg void OnToolbarFilesaveas();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	bool verifySizes(RotatedRect mr);
	bool verifySizes(Mat m);
	string file;
	int m_aipFrameNum;
};

/***************************************************************************************
Function: 数据区中是否存在图像数据
Input:    NULL
Output:   TRUE 容器中没有图像  FASLE 容器中有图像
***************************************************************************************/
inline BOOL CMainFrame::IsHasImgs()
{
	if (m_ImgSerial.isEmpty())
		return FALSE;
	else
		return TRUE;
}

