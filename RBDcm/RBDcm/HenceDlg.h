/**********************************************************************************
  Copyright @ Robin 2015-2025
  File name:  EnhanceOperat.h
  Author: Robin   Version: V1.0   Date:2015.11.05 
  Description: this class contain all relevent Enhance algorithms in the project.  
  Others:         
***********************************************************************************/
#pragma once


class CHenceDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CHenceDlg)

public:
	CHenceDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CHenceDlg();

// �Ի�������
	enum { IDD = IDD_DLG_HENCE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
};
