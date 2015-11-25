/**********************************************************************************
  Copyright @ Robin 2015-2025
  File name:  PlaySeriesDlg.h
  Author: Robin   Version: V1.0   Date: 2015.11.05 
  Description:  Play Serial Dialog Class
  Others:         
***********************************************************************************/
#include "MatImg.h"
#include "ImgFormat.h"

#pragma once

const UINT TIMER0 = 1;

// CPlaySeriesDlg 对话框
class CPlaySeriesDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CPlaySeriesDlg)

public:
	CPlaySeriesDlg(CWnd* pParent = NULL);			// 标准构造函数
	virtual ~CPlaySeriesDlg();

// 对话框数据
	enum { IDD = IDD_PLAY_SERI };
private:
	list<sOneImg>::iterator itr_bg;					//传入的序列迭代器
	list<sOneImg>::iterator itr;						//该类中使用可移动的迭代器
	int m_count;										//序列总图像数
	int m_CurNum;
public:
	BOOL isPlay;										//是否播放序列标志
	void SetImgSerial(list<sOneImg>::iterator itr,
		              int count=10);					//设置图像序列迭代器和序列的总张数

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	DECLARE_MESSAGE_MAP()
	virtual void PostNcDestroy();						//销毁窗口时销毁堆中对象
public:
	virtual BOOL OnInitDialog();						//初始化对话框
	afx_msg void OnTimer(UINT_PTR nIDEvent);			//定时器响应
	afx_msg void OnPaint();								
	afx_msg void OnBnClickedBtnPlay();					//播放按钮消息响应
	afx_msg void OnBnClickedBtnStop();					//停止播放按钮消息响应
	afx_msg void OnClose();
	CSliderCtrl m_Slider;
};
