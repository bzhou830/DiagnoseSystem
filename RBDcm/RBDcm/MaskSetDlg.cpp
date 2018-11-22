// MaskSetDlg.cpp : ʵ���ļ�
#include "stdafx.h"
#include "RBDcm.h"
#include "MaskSetDlg.h"
#include "afxdialogex.h"


// CMaskSetDlg �Ի���
IMPLEMENT_DYNAMIC(CMaskSetDlg, CDialog)

CMaskSetDlg::CMaskSetDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMaskSetDlg::IDD, pParent)
{
	//����ʱ��Ա������ֵȫ�����
	m_mask.mk[0][0] = 0; m_mask.mk[0][1] = 0; m_mask.mk[0][2] = 0;
	m_mask.mk[1][0] = 0; m_mask.mk[1][1] = 0; m_mask.mk[1][2] = 0;
	m_mask.mk[2][0] = 0; m_mask.mk[2][1] = 0; m_mask.mk[2][2] = 0;
	m_mask.weight = 0;
}

CMaskSetDlg::~CMaskSetDlg()
{
}


//�ؼ��е����ݺͱ�������һ��
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


// CMaskSetDlg ��Ϣ�������
void CMaskSetDlg::OnBnClickedOk()
{
	UpdateData(TRUE); //�������ݸ��µ�����
	CDialog::OnOK();
}

//4����������˹����
void CMaskSetDlg::OnBnClickedBtnLap4()
{
	int tb[3][3] = {0, -1, 0, -1, 4, -1, 0, -1, 0}; //�任��
	memcpy(m_mask.mk, tb, sizeof(int)*9);
	UpdateData(FALSE);//�������ݵ�����
}

//8����������˹����
void CMaskSetDlg::OnBnClickedBtnLap8()
{
	int tb[3][3] = {-1, -1, -1, -1, 8, -1, -1, -1, -1};
	memcpy(m_mask.mk, tb, sizeof(int)*9);
	UpdateData(FALSE);//�������ݵ�����
}

//x�����ݶȵ�Roberts����
void CMaskSetDlg::OnBnClickedBtnRobertsx()
{
	int tb[3][3] = {1, 0, 0, 0, -1, 0, 0, 0, 0};
	memcpy(m_mask.mk, tb, sizeof(int)*9);
	UpdateData(FALSE);//�������ݵ�����
}

//y�����ݶȵ�Roberts����
void CMaskSetDlg::OnBnClickedBtnRobertsy()
{
	int tb[3][3] = {0, 1, 0, -1, 0, 0, 0, 0, 0};
	memcpy(m_mask.mk, tb, sizeof(int)*9);
	UpdateData(FALSE);//�������ݵ�����
}

//x�����ݶȵ�Sobel����
void CMaskSetDlg::OnBnClickedBtnSobelx()
{
	int tb[3][3] = {-1, 0, 1, -2, 0, 2, -1, 0, 1};
	memcpy(m_mask.mk, tb, sizeof(int)*9);
	UpdateData(FALSE);//�������ݵ�����
}

//y�����ݶȵ�Sobel����
void CMaskSetDlg::OnBnClickedBtnSobely()
{
	int tb[3][3] = {1, 2, 1, 0, 0, 0, -1, -2, -1};
	memcpy(m_mask.mk, tb, sizeof(int)*9);
	UpdateData(FALSE);//�������ݵ�����
}

//x�����ݶȵ�Prewitt����
void CMaskSetDlg::OnBnClickedBtnPrewittx()
{
	int tb[3][3] = {-1, 0, 1, -1, 0, 1, -1, 0, 1};
	memcpy(m_mask.mk, tb, sizeof(int)*9);
	UpdateData(FALSE);//�������ݵ�����
}

//y�����ݶȵ�Prewitt����
void CMaskSetDlg::OnBnClickedBtnPrewitty()
{
	int tb[3][3] = {1, 1, 1, 0, 0, 0, -1, -1, -1};
	memcpy(m_mask.mk, tb, sizeof(int)*9);
	UpdateData(FALSE);//�������ݵ�����
}

//��ֵ�˲�����
void CMaskSetDlg::OnBnClickedBtnAverange()
{
	int tb[3][3] = {1, 1, 1, 1, 1, 1, 1, 1, 1};
	memcpy(m_mask.mk, tb, sizeof(int)*9);
	m_mask.weight = 9;
	UpdateData(FALSE);//�������ݵ�����
}


BOOL CMaskSetDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	//���ô���ͼ��
	HICON m_hIcon = AfxGetApp()->LoadIcon(IDI_MASK);
	SetIcon(m_hIcon, FALSE);

	return TRUE; 
}



