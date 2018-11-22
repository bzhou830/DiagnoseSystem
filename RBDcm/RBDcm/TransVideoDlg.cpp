// TransVideoDlg.cpp : 实现文件
#include "stdafx.h"
#include "TransVideoDlg.h"
#include "afxdialogex.h"

// CTransVideoDlg 对话框
IMPLEMENT_DYNAMIC(CTransVideoDlg, CDialogEx)

CTransVideoDlg::CTransVideoDlg(list<sOneImg> &im, CWnd* pParent)
	: CDialogEx(CTransVideoDlg::IDD, pParent)
	, m_nWidth(512)
	, m_nHeight(512)
	, m_nFps(20)
	, m_radio_gray(FALSE)
	, m_lt(im)
{
	lt.push_back(make_pair("MPEG-1", "PIM1"));
	lt.push_back(make_pair("motion-jepg", "MJPG"));
	lt.push_back(make_pair("MPEG-4.2", "MP42"));
	lt.push_back(make_pair("MPEG-4.3", "DIV3"));
	lt.push_back(make_pair("MPEG-4", "DIVX"));
	lt.push_back(make_pair("H263", "U263"));
	lt.push_back(make_pair("FLV1", "FLV1"));
}

CTransVideoDlg::~CTransVideoDlg()
{
}

void CTransVideoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_VIDEOFMT, m_comb);
	DDX_Text(pDX, IDC_EDIT_VEDIOWIDTH, m_nWidth);
	DDX_Text(pDX, IDC_EDIT_VIDEOHEIGHT, m_nHeight);
	DDX_Text(pDX, IDC_EDIT_VIDEOFPS, m_nFps);
	DDX_Radio(pDX, IDC_RADIO_VIDEO_GRAY, m_radio_gray);
	DDX_Control(pDX, IDC_PROGRESS_VIDEO, m_proctrl);
}


BEGIN_MESSAGE_MAP(CTransVideoDlg, CDialogEx)
	ON_BN_CLICKED(IDC_RADIO_VIDEO_GRAY, &CTransVideoDlg::OnBnClickedRadioVideoGray)
	ON_BN_CLICKED(IDC_RADIO_VIDEO_RGB, &CTransVideoDlg::OnBnClickedRadioVideoRgb)
	ON_BN_CLICKED(IDC_BUTTON_STARTTRANS, &CTransVideoDlg::OnBnClickedButtonStarttrans)
END_MESSAGE_MAP()


// CTransVideoDlg 消息处理程序
BOOL CTransVideoDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	m_comb.AddString("MPEG-1");
	m_comb.AddString("motion-jepg");
	m_comb.AddString("MPEG-4.2");
	m_comb.AddString("MPEG-4.3");
	m_comb.AddString("MPEG-4");
	m_comb.AddString("H263");
	m_comb.AddString("FLV1");
	m_comb.SetCurSel(1);
	return TRUE;  // return TRUE unless you set the focus to a control
}


void CTransVideoDlg::OnBnClickedRadioVideoGray()
{
	m_radio_gray = 0;
}


void CTransVideoDlg::OnBnClickedRadioVideoRgb()
{
	m_radio_gray = 1;
}

void CTransVideoDlg::OnBnClickedButtonStarttrans()
{
	CString str;
	int nSel = m_comb.GetCurSel();						
	m_comb.GetLBText(nSel, str);						
	string fmt;													//视频编码格式
	for(int i = 0; i < lt.size(); i++)
	{
		if(lt[i].first == str)
		{
			fmt = lt[i].second;
			break;
		}
	}

	bool iscolor  = false;
	if(m_radio_gray == 0) 
		iscolor  = false;
	else 
		iscolor  = true;
	
	int fourcc  = CV_FOURCC(fmt[0], fmt[1], fmt[2], fmt[3]);		//编解码类型
	
	string path;
	bool rt = Global::GetFolder(path, "Saved AS", this->m_hWnd);
	if(rt == false) return;
	
	path += "\\testVedio.avi";
	int count = m_lt.size();
	m_proctrl.SetRange(0, count);
	list<sOneImg>::iterator itr = m_lt.begin();
	VideoWriter Writer;
	Writer = VideoWriter(path, fourcc, m_nFps, Size(m_nWidth, m_nHeight), iscolor);
	for (int i=0; i<count; i++)
	{
		Writer.write(Global::u16Tou8(itr->src_u16));
		itr++;
		m_proctrl.SetPos(i);
	}
}


