/**********************************************************************************
  Copyright @ Robin 2015-2025
  File name:  SetValueDlg.cpp
  Author: Robin   Version: V1.0   Date:2015.11.05 
  Description: dialog class about set value 
  Others:         
***********************************************************************************/
#include "stdafx.h"
#include "RBDcm.h"
#include "SetValueDlg.h"
#include "afxdialogex.h"


// CSetValueDlg �Ի���
IMPLEMENT_DYNAMIC(CSetValueDlg, CDialog)

CSetValueDlg::CSetValueDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSetValueDlg::IDD, pParent)
	, m_value(0) , m_strWindowText("��ֵ")
{

}


CSetValueDlg::~CSetValueDlg()
{
}


void CSetValueDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_value);
}


BEGIN_MESSAGE_MAP(CSetValueDlg, CDialog)
END_MESSAGE_MAP()

/***************************************************************************************
Function: ��ȡ�������ֵ
Input:    NULL
Output:   NULL
Description: �ṩ���ⲿ����
Return:   int ������������
Others:   NULL
***************************************************************************************/
int CSetValueDlg::GetValue(void)
{
	return this->m_value;
}

/***************************************************************************************
Function: �޸Ĵ��ڱ���
Input:    ���ڵı�����
Output:   NULL
Description: �ṩ���ⲿ���ã��������ͨ����
Return:   NULL
Others:   NULL
***************************************************************************************/
void CSetValueDlg::SetTitle(CString strText)
{
	m_strWindowText = strText;
}


/***************************************************************************************
Function: ���ڳ�ʼ������
Input:    NULL
Output:   NULL
Description: �ڳ�ʼ���������޸Ĵ��ڱ��⣬�������õĴ�����
Return:   NULL
Others:   NULL
***************************************************************************************/
BOOL CSetValueDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	SetWindowText(m_strWindowText);
	return TRUE;  
}

/***************************************************************************************
Function: �������е�edi�ؼ���д������
Input:    Ҫд���int��������
Output:   NULL
Description: �ṩ���ⲿ����
Return:   NULL
Others:   NULL
***************************************************************************************/
void CSetValueDlg::SetValue(int vl)
{
	m_value = vl;
}
