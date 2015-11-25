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


// CSetValueDlg 对话框
IMPLEMENT_DYNAMIC(CSetValueDlg, CDialog)

CSetValueDlg::CSetValueDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSetValueDlg::IDD, pParent)
	, m_value(0) , m_strWindowText("设值")
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
Function: 获取填入的数值
Input:    NULL
Output:   NULL
Description: 提供给外部调用
Return:   int 传出填入数字
Others:   NULL
***************************************************************************************/
int CSetValueDlg::GetValue(void)
{
	return this->m_value;
}

/***************************************************************************************
Function: 修改窗口标题
Input:    窗口的标题名
Output:   NULL
Description: 提供给外部调用，增加类的通用性
Return:   NULL
Others:   NULL
***************************************************************************************/
void CSetValueDlg::SetTitle(CString strText)
{
	m_strWindowText = strText;
}


/***************************************************************************************
Function: 窗口初始化函数
Input:    NULL
Output:   NULL
Description: 在初始化函数中修改窗口标题，根据设置的窗口名
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
Function: 给窗口中的edi控件中写入数据
Input:    要写入的int类型数据
Output:   NULL
Description: 提供给外部调用
Return:   NULL
Others:   NULL
***************************************************************************************/
void CSetValueDlg::SetValue(int vl)
{
	m_value = vl;
}


