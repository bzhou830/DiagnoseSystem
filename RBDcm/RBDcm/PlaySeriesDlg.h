/**********************************************************************************
  Copyright @ Robin 2015-2025
  File name:  PlaySeriesDlg.h
  Author: Robin   Version: V1.0   Date: 2015.11.05 
  Description:  Play Serial Dialog Class
  Others:         
***********************************************************************************/
#include "ImgSerial.h"
#include "ImgFormat.h"

#pragma once

const UINT TIMER0 = 1;

// CPlaySeriesDlg �Ի���
class CPlaySeriesDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CPlaySeriesDlg)

public:
	CPlaySeriesDlg(CWnd* pParent = NULL);			// ��׼���캯��
	virtual ~CPlaySeriesDlg();

// �Ի�������
	enum { IDD = IDD_PLAY_SERI };
private:
	list<sOneImg>::iterator itr_bg;					//��������е�����
	list<sOneImg>::iterator itr;						//������ʹ�ÿ��ƶ��ĵ�����
	int m_count;										//������ͼ����
	int m_CurNum;
public:
	BOOL isPlay;										//�Ƿ񲥷����б�־
	void SetImgSerial(list<sOneImg>::iterator itr,
		              int count=10);					//����ͼ�����е����������е�������

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	DECLARE_MESSAGE_MAP()
	virtual void PostNcDestroy();						//���ٴ���ʱ���ٶ��ж���
public:
	virtual BOOL OnInitDialog();						//��ʼ���Ի���
	afx_msg void OnTimer(UINT_PTR nIDEvent);			//��ʱ����Ӧ
	afx_msg void OnPaint();								
	afx_msg void OnBnClickedBtnPlay();					//���Ű�ť��Ϣ��Ӧ
	afx_msg void OnBnClickedBtnStop();					//ֹͣ���Ű�ť��Ϣ��Ӧ
	afx_msg void OnClose();
	CSliderCtrl m_Slider;
	afx_msg LRESULT OnNcHitTest(CPoint point);
};
