// SaveImgDlg.cpp : 实现文件
#include "stdafx.h"
#include "SaveImgDlg.h"
#include "afxdialogex.h"

//CSaveImgDlg 对话框
IMPLEMENT_DYNAMIC(CSaveImgDlg, CDialog)

CSaveImgDlg::CSaveImgDlg(list<sOneImg> &im, CWnd* pParent)
	: CDialog(CSaveImgDlg::IDD, pParent)
	, m_lt(im)
	, m_bpathok(false)
	, m_path("")
	, m_rd(FALSE)
	, m_rdb8(FALSE)
{
}

CSaveImgDlg::~CSaveImgDlg()
{
}

void CSaveImgDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROGRESS1, m_proctrl);
	DDX_Radio(pDX, IDC_RADIO_SRC, m_rd);
	DDX_Radio(pDX, IDC_RADIO_B8, m_rdb8);
}

BEGIN_MESSAGE_MAP(CSaveImgDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CSaveImgDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON1, &CSaveImgDlg::OnBnClickedButton1)
END_MESSAGE_MAP()

void CSaveImgDlg::OnBnClickedOk()
{
	//从界面上获取需要保存的类型
	if( ((CButton *)GetDlgItem(IDC_RADIO_SRC))->GetCheck() )
		tp = SRC_IMG;
	else if ( ((CButton *)GetDlgItem(IDC_RADIO2))->GetCheck() )
		tp = SEG_IMG;
	else if ( ((CButton *)GetDlgItem(IDC_RADIO3))->GetCheck() )
		tp = NOD_IMG;

	if ( ((CButton *)GetDlgItem(IDC_RADIO_B8))->GetCheck() )
		bt = BIT8U;
	else
		bt = BIT16U;

	if(m_bpathok == false)
	{
		AfxMessageBox("路径不正确！");
		return;
	}
	if(m_path.back() != '\\')
		m_path += "\\";

	char fileno[10] = {0};
	list<sOneImg>::iterator it = m_lt.begin();
	m_proctrl.SetRange(0, m_lt.size());
	string rpath;
	for (int i=0; it != m_lt.end(); it++,i++)
	{
		if (bt == BIT8U)
		{
			sprintf(fileno, "File%04d.bmp", i);
			rpath = m_path + fileno;
			if(tp == SRC_IMG)
				imwrite(rpath, Global::u16Tou8(it->src_u16));
			else if (tp == SEG_IMG)
			{
				Mat tmp = Global::u16Tou8(it->src_u16);
				Mat tmp2 = it->masking;
				Mat rt;
				bitwise_and(tmp,tmp2,rt);

				//tmp = Global::Mask(tmp, it->masking);
				//bitwise_and(tmp,it->masking,tmp);
				imwrite(rpath, rt);
			}
			else
			{

			}

		}
		else
		{
			if(tp == SRC_IMG)
			{

			}
			else if (tp == SEG_IMG)
			{

			}
			else
			{

			}
		}
		
		
		
		m_proctrl.SetPos(i);
	}
	AfxMessageBox("保存完成！");
}


void CSaveImgDlg::OnBnClickedButton1()
{
	m_bpathok = Global::GetFolder(m_path, "Saved AS", this->m_hWnd);
	if (m_bpathok)
		SetDlgItemText(IDC_EDIT1, m_path.c_str());
}
