// MaskSetDlg.cpp : 实现文件
#include "stdafx.h"
#include "RBDcm.h"
#include "MaskSetDlg.h"
#include "afxdialogex.h"

// CMaskSetDlg 对话框
IMPLEMENT_DYNAMIC(CMaskSetDlg, CDialog)

CMaskSetDlg::CMaskSetDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMaskSetDlg::IDD, pParent)
{
	//构造时成员变量的值全部清空
	m_mask.mk[0][0] = 0; m_mask.mk[0][1] = 0; m_mask.mk[0][2] = 0;
	m_mask.mk[1][0] = 0; m_mask.mk[1][1] = 0; m_mask.mk[1][2] = 0;
	m_mask.mk[2][0] = 0; m_mask.mk[2][1] = 0; m_mask.mk[2][2] = 0;
	m_mask.weight = 0;
}

CMaskSetDlg::~CMaskSetDlg()
{
}


//控件中的数据和变量绑定在一起
void CMaskSetDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_11, m_mask.mk[0][0]);
	DDX_Text(pDX, IDC_EDIT_12, m_mask.mk[0][1]);
	DDX_Text(pDX, IDC_EDIT_13, m_mask.mk[0][2]);
	DDX_Text(pDX, IDC_EDIT_21, m_mask.mk[1][0]);
	DDX_Text(pDX, IDC_EDIT_22, m_mask.mk[1][1]);
	DDX_Text(pDX, IDC_EDIT_23, m_mask.mk[1][2]);
	DDX_Text(pDX, IDC_EDIT_31, m_mask.mk[2][0]);
	DDX_Text(pDX, IDC_EDIT_32, m_mask.mk[2][1]);
	DDX_Text(pDX, IDC_EDIT_33, m_mask.mk[2][2]);
	DDX_Text(pDX,IDC_EDIT_Wight, m_mask.weight);
}


BEGIN_MESSAGE_MAP(CMaskSetDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CMaskSetDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BTN_LAP4, &CMaskSetDlg::OnBnClickedBtnLap4)
	ON_BN_CLICKED(IDC_BTN_LAP8, &CMaskSetDlg::OnBnClickedBtnLap8)
	ON_BN_CLICKED(IDC_BTN_ROBERTSX, &CMaskSetDlg::OnBnClickedBtnRobertsx)
	ON_BN_CLICKED(IDC_BTN_ROBERTSY, &CMaskSetDlg::OnBnClickedBtnRobertsy)
	ON_BN_CLICKED(IDC_BTN_SOBELX, &CMaskSetDlg::OnBnClickedBtnSobelx)
	ON_BN_CLICKED(IDC_BTN_SOBELY, &CMaskSetDlg::OnBnClickedBtnSobely)
	ON_BN_CLICKED(IDC_BTN_PREWITTX, &CMaskSetDlg::OnBnClickedBtnPrewittx)
	ON_BN_CLICKED(IDC_BTN_PREWITTY, &CMaskSetDlg::OnBnClickedBtnPrewitty)
	ON_BN_CLICKED(IDC_BTN_AVERANGE, &CMaskSetDlg::OnBnClickedBtnAverange)
END_MESSAGE_MAP()


// CMaskSetDlg 消息处理程序
void CMaskSetDlg::OnBnClickedOk()
{
	UpdateData(TRUE); //界面数据更新到变量
	CDialog::OnOK();
}

//4领域拉普拉斯算子
void CMaskSetDlg::OnBnClickedBtnLap4()
{
	int tb[3][3] = {0, -1, 0, -1, 4, -1, 0, -1, 0}; //变换核
	memcpy(m_mask.mk, tb, sizeof(int)*9);
	UpdateData(FALSE);//设置数据到界面
}

//8领域拉普拉斯算子
void CMaskSetDlg::OnBnClickedBtnLap8()
{
	int tb[3][3] = {-1, -1, -1, -1, 8, -1, -1, -1, -1};
	memcpy(m_mask.mk, tb, sizeof(int)*9);
	UpdateData(FALSE);//设置数据到界面
}

//x方向梯度的Roberts算子
void CMaskSetDlg::OnBnClickedBtnRobertsx()
{
	int tb[3][3] = {1, 0, 0, 0, -1, 0, 0, 0, 0};
	memcpy(m_mask.mk, tb, sizeof(int)*9);
	UpdateData(FALSE);//设置数据到界面
}

//y方向梯度的Roberts算子
void CMaskSetDlg::OnBnClickedBtnRobertsy()
{
	int tb[3][3] = {0, 1, 0, -1, 0, 0, 0, 0, 0};
	memcpy(m_mask.mk, tb, sizeof(int)*9);
	UpdateData(FALSE);//设置数据到界面
}

//x方向梯度的Sobel算子
void CMaskSetDlg::OnBnClickedBtnSobelx()
{
	int tb[3][3] = {-1, 0, 1, -2, 0, 2, -1, 0, 1};
	memcpy(m_mask.mk, tb, sizeof(int)*9);
	UpdateData(FALSE);//设置数据到界面
}

//y方向梯度的Sobel算子
void CMaskSetDlg::OnBnClickedBtnSobely()
{
	int tb[3][3] = {1, 2, 1, 0, 0, 0, -1, -2, -1};
	memcpy(m_mask.mk, tb, sizeof(int)*9);
	UpdateData(FALSE);//设置数据到界面
}

//x方向梯度的Prewitt算子
void CMaskSetDlg::OnBnClickedBtnPrewittx()
{
	int tb[3][3] = {-1, 0, 1, -1, 0, 1, -1, 0, 1};
	memcpy(m_mask.mk, tb, sizeof(int)*9);
	UpdateData(FALSE);//设置数据到界面
}

//y方向梯度的Prewitt算子
void CMaskSetDlg::OnBnClickedBtnPrewitty()
{
	int tb[3][3] = {1, 1, 1, 0, 0, 0, -1, -1, -1};
	memcpy(m_mask.mk, tb, sizeof(int)*9);
	UpdateData(FALSE);//设置数据到界面
}

//均值滤波算子
void CMaskSetDlg::OnBnClickedBtnAverange()
{
	int tb[3][3] = {1, 1, 1, 1, 1, 1, 1, 1, 1};
	memcpy(m_mask.mk, tb, sizeof(int)*9);
	m_mask.weight = 9;
	UpdateData(FALSE);//设置数据到界面
}


BOOL CMaskSetDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	//设置窗口图标
	HICON m_hIcon = AfxGetApp()->LoadIcon(IDI_MASK);
	SetIcon(m_hIcon, FALSE);

	return TRUE; 
}



