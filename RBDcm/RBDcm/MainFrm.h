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
#include "InfoView.h"															//ר�ұ�ע��Ϣ����
#include "MySplitterWndEx.h"													//�ָ��
#include "SplitterWndExt.h"														//
#include "AllImgView.h"															//
#include "ImgSerial.h"															//ͼ������ת��
#include "SrcShowDlg.h"															//ԭͼ����ʾ����
#include "SegmentOperat.h"														//ͼ��ָ����
#include "EnhanceOperat.h"														//ͼ����ǿ����
#include "OneImgFeatures.h"														//������ͼ��������
#include "WndWidthPosDlg.h"														//����λ��������
#include "log/Log.h"															//д��־ͷ�ļ�
#include "SaveImgDlg.h"															//ͼ�񱣴洰��
#include "LoadDlg.h"															//����������
#include "AIPDlg.h"
#include "Dock.h"


#include <iostream>

using namespace std;

const UINT UM_PROSSESS = WM_USER + 1;		//�Զ�����Ϣ
const UINT INFOWINDOWWIDTH = 200;			//��Ϣ��ʾ����

class COprtView;							//�������࣬��֧���໥����

class CMainFrame : public CFrameWnd
{
private:
	double              fSlice;
public:
	bool				m_isDisplayNote;	//�Ƿ���ʾר�ұ�ע��Ϣ
	CImgSerial			m_ImgSerial;        //Mat-image structure
	CPlaySeriesDlg*		m_PlayDlg;			//����ͼ��Ի���
	CAIPDlg*			m_AipDlg;			//�ܶ�ͶӰ�Ի���
public:
	CMySplitterWndEx	m_splt;				//���ָ���
	CMySplitterWndEx	m_splt2;			//�Ҳ�ָ���
	CSliderCtrl			m_slider;			//״̬��������
	COprtView*			m_pOpt;				//������崰��
	CLUView*			m_pLUView;			//����ͼ����ʾ����
	CRUView*			m_pRUView;			//����ͼ����ʾ����
	CLDView*			m_pLDView;			//����ͼ����ʾ����
	CLDView*			m_pRDView;			//���·���������
	CInfoView*			m_pInfoView;		//����ͼ����ʾ����
	BOOL                m_isLeftPaneMin;    //�������Ƿ�����
	CToolBar            m_toolBarFunc;		//ͼ������ع���
	CToolBar            m_toolBarFile;		//�ļ�������ع���
	CLog                m_Log;				//д��־����
	CString             m_szAppPath;		//�������·
	//CvSVM				m_svmClassifier;	//SVM������
	static HANDLE		m_hMutex;			//������
	static int			m_LoadStatus;
	bool				m_bCanClick;		//�������ܷ�����
protected: // �������л�����
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)
// ����
public:
// ����
public:
	CStatusBar* GetStatusBar(){return &m_wndStatusBar;};

// ��д
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
// ʵ��
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:				// �ؼ���Ƕ���Ա
	CStatusBar   m_wndStatusBar;
// ���ɵ���Ϣӳ�亯��
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
Function: ���������Ƿ����ͼ������
Input:    NULL
Output:   TRUE ������û��ͼ��  FASLE ��������ͼ��
***************************************************************************************/
inline BOOL CMainFrame::IsHasImgs()
{
	if (m_ImgSerial.isEmpty())
		return FALSE;
	else
		return TRUE;
}

