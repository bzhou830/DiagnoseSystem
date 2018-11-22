/*================================================================================*
* Copyright @ Robin 2015 - 2025
* File name:  MainFrm.cpp
* Author: Robin   
* Version: V2.0   
* Date: 2015.11.05 
*=================================================================================*/

#include "stdafx.h"
#include "RBDcm.h"
#include "Global.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame
IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_MESSAGE(UM_PROSSESS, OnProgress)
	ON_COMMAND(ID_BTN_RESET, &CMainFrame::OnBtnAIP)
	ON_COMMAND(ID_BTN_ZOOM, &CMainFrame::OnBtnZoom)
	ON_COMMAND(ID_BTN_SRCIMG, &CMainFrame::OnBtnSrcimg)
	ON_COMMAND(ID_BTN_PLAYIMGS, &CMainFrame::OnBtnPlayimgs)
	ON_COMMAND(ID_BTN_NEXTIMG, &CMainFrame::OnBtnExtract)
	ON_COMMAND(ID_BTN_LOADXML, &CMainFrame::OnBtnLoadxml)
	ON_COMMAND(ID_BTN_LASTIMG, &CMainFrame::OnBtnGMM)
	ON_COMMAND(ID_BTN_HISTIMG, &CMainFrame::OnBtnHistimg)
	ON_COMMAND(ID_BTN_CAMERA, &CMainFrame::OnBtnCamera)
	ON_COMMAND(ID_SERILE_OPEN, &CMainFrame::OnSerileOpen)
	ON_COMMAND(ID_FILE_OPEN_ONE, &CMainFrame::OnFileOpenOne)
	ON_COMMAND(ID_BTN_ENTROPY, &CMainFrame::OnBtnEntropy)
	ON_COMMAND(ID_BTN_OTSU, &CMainFrame::OnBtnOtsu)
	ON_COMMAND(ID_BTN_REALLUNG, &CMainFrame::OnBtnVTK)
	ON_COMMAND(ID_BTN_NUDOUL, &CMainFrame::OnBtnNudoul)
	ON_COMMAND(ID_BTN_MOM, &CMainFrame::OnBtnMom)
	ON_COMMAND(ID_BTN_MEAN, &CMainFrame::OnBtnMean)
	ON_COMMAND(IDM_SAVEIMG, &CMainFrame::OnSaveimg)
	ON_COMMAND(ID_BTN_TEST, &CMainFrame::OnBtnTest)
	ON_COMMAND(IDM_TRAINING, &CMainFrame::OnTraining)
	ON_COMMAND(IDM_WINDOW, &CMainFrame::OnWindow)
	ON_COMMAND(IDM_SAVESERIAL, &CMainFrame::OnSaveserial)
	ON_COMMAND(ID_TEST, &CMainFrame::OnTest)
	ON_COMMAND(IDM_DISPLAY_NOTE, &CMainFrame::OnDisplayNote)
	ON_UPDATE_COMMAND_UI(IDM_DISPLAY_NOTE, &CMainFrame::OnUpdateDisplayNote)
	ON_COMMAND(ID_BTN_CLICK, &CMainFrame::OnBtnClick)
	ON_UPDATE_COMMAND_UI(ID_BTN_CLICK, &CMainFrame::OnUpdateBtnClick)
	ON_COMMAND(ID_TOOLBAR_FILEOPEN, &CMainFrame::OnToolbarFileopen)
	ON_COMMAND(ID_TOOLBAR_FILEPRINT, &CMainFrame::OnToolbarFileprint)
	ON_COMMAND(ID_TOOLBAR_FILESAVE, &CMainFrame::OnToolbarFilesave)
	ON_COMMAND(ID_TOOLBAR_FILESAVEAS, &CMainFrame::OnToolbarFilesaveas)
//	ON_WM_SIZE()
ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

//״̬��ָʾ��
static UINT indicators[] =
{
	ID_SEPARATOR,									//״̬��ָʾ��
	IDS_ALLIMGNUM,
	IDS_CURRENTIMGNUM,
	ID_INDICATOR_NUM,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

HANDLE CMainFrame::m_hMutex = NULL;
int CMainFrame::m_LoadStatus = FALSE;

/*================================================================================*
* ����ܳ���Ĺ��캯��
* �Գ�Ա��������ֵ��ʼ�����ر���ָ���Ա
*=================================================================================*/
CMainFrame::CMainFrame()
	: m_isLeftPaneMin(FALSE)
	, m_isDisplayNote(false)
	, m_bCanClick(false)
{
	m_PlayDlg = NULL;
	m_AipDlg = NULL;
}

/*================================================================================*
* ����ܳ������������
* �ڴ˺����жԶ�̬����Ķ�����м�飬��ֹ�ڴ�й©
*=================================================================================*/
CMainFrame::~CMainFrame()
{
	if (m_PlayDlg != NULL)
	{
		delete m_PlayDlg;
		m_PlayDlg = NULL;
	}
	if (m_AipDlg != NULL)
	{
		delete m_AipDlg;
		m_AipDlg = NULL;
	}
}

/*================================================================================*
* Function: �����ܴ���ǰ��Ϣ��Ӧ
* Description: �ڴ������޸ĳ�������ʽ
*=================================================================================*/
BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// CREATESTRUCT cs ���޸Ĵ��������ʽ
	cs.style = WS_OVERLAPPED | WS_CAPTION | FWS_ADDTOTITLE
		 | WS_MINIMIZEBOX | WS_SYSMENU;
	cs.lpszName = "RBDcm������������ϵͳ";
	cs.style &= ~FWS_ADDTOTITLE;												//ȥ��������ʾ���ļ�����

	//��ȡ������Ļ�Ĵ�С
	cs.cx = ::GetSystemMetrics(SM_CXSCREEN);
	CRect rect;
	SystemParametersInfo(SPI_GETWORKAREA, 0, &rect, 0);
	cs.cy = rect.bottom;
	//cs.cy = ::GetSystemMetrics(SM_CYSCREEN);
	cs.y = 0;																	//����������������Ļ��С
	cs.x = 0;
	
	return TRUE;
}

// CMainFrame ���
#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}
#endif //_DEBUG

/*================================================================================*
* Function:    �������д����ͻ��� ��Ϣ��Ӧ
* Description: �ڴ����зָ��
*=================================================================================*/
BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext)
{
	if( !m_splt.CreateStatic(this, 1, 2) )										//�ָ�Ϊ1��2��
	{
		AfxMessageBox("���ڷָ�ʧ�ܣ�");
		return FALSE;
	}
	CRect rct;
	GetWindowRect(&rct);														//��ȡ�ͻ�������
	int nRcWidth = rct.Width();													//
	int nRcHeight = rct.Height();												//
	nRcHeight -= 60;															//���⣬״̬��һ��Ĵ�����߶�Ϊ60

	//�Ҳ����ʾ����
	if (!m_splt2.CreateStatic(&m_splt, 2, 2, WS_CHILD|WS_VISIBLE, m_splt.IdFromRowCol(0, 0)))
	{
		AfxMessageBox("���ڷָ�ʧ�ܣ�");
		return FALSE;
	}
	nRcWidth = (nRcWidth - INFOWINDOWWIDTH)/2;
	nRcHeight = nRcHeight/2;
	m_splt2.CreateView(0, 0, RUNTIME_CLASS(CLUView), CSize(nRcWidth, nRcHeight), pContext);
	m_pLUView = (CLUView*)m_splt2.GetPane(0,0);
	m_splt2.CreateView(0, 1, RUNTIME_CLASS(CRUView), CSize(nRcWidth, nRcHeight), pContext);
	m_pRUView = (CRUView*)m_splt2.GetPane(0,1);
	m_splt2.CreateView(1, 0, RUNTIME_CLASS(CLDView), CSize(nRcWidth, nRcHeight), pContext);
	m_pLDView = (CLDView*)m_splt2.GetPane(1,0);
	m_splt2.CreateView(1, 1, RUNTIME_CLASS(CLDView), CSize(nRcWidth, nRcHeight), pContext);
	m_pRDView = (CLDView*)m_splt2.GetPane(1,1);
	//m_pInfoView = (CInfoView*)m_splt2.GetPane(1,1);
	
	m_splt.CreateView(0, 1, RUNTIME_CLASS(CInfoView), CSize(INFOWINDOWWIDTH, nRcHeight), pContext);
	m_pInfoView = (CInfoView*)m_splt.GetPane(0,1);

	m_splt.SetColumnInfo(0, rct.Width()-INFOWINDOWWIDTH, 500);
	m_splt.SetColumnInfo(1,  INFOWINDOWWIDTH, 100);
	return TRUE;
}


//���ô��ڴ�С�䶯
//tips: ���͸������Ӵ��ڵ���Ϣ�����ڴ�·�ɣ�����Ҫ�жϾ����������Ӵ����϶����Ӱ��.
BOOL CMainFrame::PreTranslateMessage(MSG* pMsg)
{
	//�ػ�����ڱ�����˫����Ϣ
	HWND hWnd = AfxGetMainWnd()->m_hWnd;
	if(pMsg->message == WM_NCLBUTTONDBLCLK && pMsg->wParam == HTCAPTION && pMsg->hwnd == hWnd)
		return TRUE;
	//�ػ�����ڱ������϶���Ϣ
	if (pMsg->message == WM_NCLBUTTONDOWN  && pMsg->wParam == HTCAPTION && pMsg->hwnd == hWnd)
		return TRUE;
	return CFrameWnd::PreTranslateMessage(pMsg);
}


/***************************************************************************************
Function:    �Զ�����Ϣ������
Description: �ڴ�������״̬����ӻ���
***************************************************************************************/
LRESULT CMainFrame::OnProgress(WPARAM, LPARAM)
{
	/*
	CRect rect;
	m_wndStatusBar.GetItemRect(3, &rect);  //��ȡҪ��ʾ������Ĵ�С
	//�ʵ���С���壬���ڱ�Ե�����ڸ�
	rect.top += 2;
	rect.bottom -= 2;
	rect.left += 2;
	rect.right -= 2;
	//��״̬������һ������ؼ�
	if (!m_slider.Create(TBS_HORZ|WS_CHILD|WS_VISIBLE, rect, &m_wndStatusBar, 123))
	{
		AfxMessageBox("���鴴��ʧ��");
		return -1;
	}
	*/
	return 0;
}



/*===========================================================================*
* ����ĳһ���Ӵ�Ϊ��ǰ����Ӵ��������Ӵ�������
*============================================================================*/
void CMainFrame::SetMainFrm(int num)
{
	CRect rct;
	GetClientRect(&rct);														//��ȡ�����ͻ�����С
	int nRcWidth = rct.Width();
	int nRcHeight = rct.Height();
	if (m_isLeftPaneMin)
		nRcWidth = nRcWidth;
	else
		nRcWidth = (nRcWidth-INFOWINDOWWIDTH);
	switch (num)
	{
	case 0:
		m_splt2.SetColumnInfo(0, nRcWidth, 10);									//���õ�һ���п�Ϊ���ڿ��
		m_splt2.SetRowInfo(0, nRcHeight, 10);									//���õ�һ���и�Ϊ���ڸ߶�
		break;
	case 1:
		m_splt2.SetColumnInfo(0,0,10);											//���õ�һ���п�Ϊ0
		m_splt2.SetColumnInfo(1,nRcWidth,10);									//���õڶ����п�Ϊ���ڿ��
		m_splt2.SetRowInfo(0,nRcHeight,10);										//���õ�һ���и�Ϊ���ڸ߶�
		break;
	case 2:
		m_splt2.SetColumnInfo(0,nRcWidth,10);
		m_splt2.SetRowInfo(0,0,10);
		m_splt2.SetRowInfo(1,nRcHeight,10);
		break;
	case 3:
		m_splt2.SetColumnInfo(1,nRcWidth,10);
		m_splt2.SetRowInfo(1,nRcHeight,10);
		break;
	default:
		break;
	}
	m_splt2.RecalcLayout();														//�޸��Ӵ���Ҫˢ��
	m_pLUView->Invalidate(FALSE);
	m_pRUView->Invalidate(FALSE);
	m_pLDView->Invalidate(FALSE);
}


/*===========================================================================*
* ��ʾ�Ӵ��ص�ԭ��λ��
*============================================================================*/
int CMainFrame::ResetFrm()
{
	CRect rct;
	GetClientRect(&rct);														//��ȡ�ͻ����������С
	int nRcWidth = rct.Width();			
	int nRcHeight = rct.Height();
	nRcHeight -= 60;															//ȥ�����⣬�˵���״̬��
	if (m_isLeftPaneMin == TRUE)												//����ʾ������		
		nRcWidth = nRcWidth/2;													//
	else																		//��ʾ������
		nRcWidth = (nRcWidth-INFOWINDOWWIDTH)/2;								//
	m_splt2.SetColumnInfo(0, nRcWidth, 10);										//
	m_splt2.SetRowInfo(0, nRcHeight / 2, 10);									//
	m_splt2.RecalcLayout();
	m_pLUView->Invalidate(FALSE);
	m_pRUView->Invalidate(FALSE);
	m_pLDView->Invalidate(FALSE);
	return 0;
}


/*===========================================================================*
* ��ͼ������ʱ �̺߳��� ������ʾ�߳�
*============================================================================*/
DWORD WINAPI CMainFrame::FunProc(LPVOID lp)
{
	CLoadDlg* pDlg = new CLoadDlg;
	pDlg->Create(IDD_LOAD_DLG);													//�����������Ի���
	pDlg->ShowWindow(SW_SHOW);													//��ʾ�������Ի���
	pDlg->SetFocus();															//������������ʾ����ǰ��
	pDlg->SetSum(100);															//����������
	pDlg->SetText("���м����У��Ե�...");									//�Ի�������ʾ�ı�
	int m = 0;
	BOOL flag = TRUE;
	BOOL isChange = FALSE;
	while (flag)																//һֱ��ȡ����ͼ����������ʱ�˳�
	{
		m = (m + 1) % 100;
		pDlg->SetProcess(m);													//���ý�����
		
		WaitForSingleObject(m_hMutex, INFINITE);
		if (m_LoadStatus == 1 && isChange == FALSE)
		{
			pDlg->SetText("�ָ������У��������...");
			isChange = TRUE;
		}
		else if (m_LoadStatus == 2)
			flag = FALSE;
		ReleaseMutex(m_hMutex);	
		Sleep(50);
	}
	delete pDlg;																//���ٶԻ���	
	m_LoadStatus = 0;														
	return 0;
}


/*===========================================================================*
* ��ͼ�����в���
*============================================================================*/
void CMainFrame::OnSerileOpen()
{
	CFileDialog dlg(TRUE);														//�ļ��򿪶Ի���
	CFileFind ff;																//�ļ�������
	CStringArray dcmPathstr;													//�ļ�·��+�ļ����б� dcm�ļ�
	dcmPathstr.RemoveAll();
	dlg.m_ofn.lpstrTitle = "��ͼ������";											//ָ�����ļ��Ի�������
	dlg.m_ofn.lpstrFilter = "Dcm Files(*.dcm)\0*.dcm\0All Files(*.*)\0*.*\0\0"; //�ļ�������

	if (IDCANCEL == dlg.DoModal())												//�����ļ��򿪶Ի���ѡ��ȡ����ֱ�ӷ���
		return;

	CString strFileName = dlg.GetPathName();									//��ȡ�ļ�·��+�ļ���
	int end = strFileName.ReverseFind('\\');									//������ҵ��ָ����ļ���
	CString strFilePath = strFileName.Left(end);								//ȥ�������ļ����õ��ļ�Ŀ¼
	if(!ff.FindFile(strFilePath + "\\*.dcm"))									//����Ŀ¼���Ƿ����dcm�ļ������������˳�
		return;

	while(ff.FindNextFile())													//����dcm�ļ��������ļ�����·������dcmPathstr
		dcmPathstr.Add(strFilePath + "\\" + ff.GetFileName());

	m_ImgSerial.Clear();														//��������ǰ�����
	CString strXml = NULL;														//�ļ�·��

	HANDLE hHANDLE = CreateThread(NULL, 0, FunProc, NULL, 0, NULL);				//�����߳�������ʾ��ȡ����
	CloseHandle(hHANDLE);														//�رվ��
	//m_hMutex = CreateMutex(NULL, FALSE, NULL);								//��������������������ͬ��

	m_ImgSerial.LoadDcms(&dcmPathstr);											//���̼߳���
	m_ImgSerial.Merge();														//�ϲ���������

	/*
	һ��һ�ż���
	for (int i=0; i<dcmPathstr.GetSize(); ++i)									//���μ���ͼ��
	{
		strFileName = dcmPathstr.GetAt(i);										//��������ȡ���ļ�·��
		m_ImgSerial.LoadDcm(strFileName);										//����Dcm�ļ�
	}
	*/

	WaitForSingleObject(m_hMutex, INFINITE);
	m_LoadStatus = 1;															//�������
	ReleaseMutex(m_hMutex);
	m_ImgSerial.SortImg();														//��ͼ�����򣨰���z��˳��
	
	fSlice = fabs(m_ImgSerial.GetEnd()->fSliceLocation 
		- m_ImgSerial.GetBegin()->fSliceLocation);								//����λ��
	fSlice = fSlice / m_ImgSerial.GetBegin()->fSliceSpacing;					//����CT���
	int dealtime = m_ImgSerial.SegSeries();										//�ָ�����ͼ��
	
	m_aipFrameNum = (int)(7.5 / m_ImgSerial.GetBegin()->info.slicesickness);	//AIP����

	m_ImgSerial.ImgAIP(m_aipFrameNum);											//ƽ���ܶ�ͶӰ
	m_ImgSerial.mskAIP(m_aipFrameNum);

	SetStateText(3, "�ָ���ʱ:" + CType::int2CString(dealtime) + "ms");			//״̬����ʾ�ָ���ʱ

	WaitForSingleObject(m_hMutex, INFINITE);
	m_LoadStatus = 2;
	ReleaseMutex(m_hMutex);
	
	sOneImg coronal;															//�ؽ��õ�ʸ״��ͼ��
	coronal.masking = m_ImgSerial.GetCoronal(120, static_cast<int>(fSlice));		
	coronal.info = m_ImgSerial.GetBegin()->info;
	m_pRUView->SetImgData(coronal);												//��ʾʸ״��ͼ��

	sOneImg sagittal;															//�ؽ��õ���״��ͼ��
	sagittal.masking = m_ImgSerial.GetSagittal(256,static_cast<int>(fSlice));	
	sagittal.info = coronal.info;
	m_pLDView->SetImgData(sagittal);											//��ʾ��״��ͼ��

	sOneImg info = m_ImgSerial.GetCurrentMatImg();								//ȡ�õ�ǰͼ��
	m_pLUView->SetImgData(info);												//��ʾ��ԭͼ�񴰿�
	CString szText;
	szText.Format("���й��С�%d����ͼ��", m_ImgSerial.GetImageNum());				//��״̬����ʾͼ������
	SetStateText(1, szText);

	file = info.info.sId;														//���浱ǰ��������
}

/*===========================================================================*
* �򿪵���ͼ��
*============================================================================*/
void CMainFrame::OnFileOpenOne()
{
	CFileDialog dlg(TRUE);														//�ļ��򿪶Ի���
	dlg.m_ofn.lpstrTitle = "�򿪵���ͼ��";											//ָ�����ļ��Ի�������
	//�ļ�������
	dlg.m_ofn.lpstrFilter = "Dcm Files(*.dcm)\0*.dcm\0All Files(*.*)\0*.*\0\0";		
	if (IDCANCEL == dlg.DoModal())												//�����ļ��򿪶Ի���ѡ��ȡ����ֱ�ӷ���
		return;
	CString strFileName = dlg.GetPathName();									//��ȡ�ļ�·��+�ļ���
	m_ImgSerial.Clear();														//���ԭ������
	m_ImgSerial.LoadDcm(strFileName);											//��������
	//SetSum(m_ImgSerial.GetImageNum());										//��ʾ��ǰͼ������
	//SetCurrent(m_ImgSerial.GetCurrentNum());									//��ʾ��ǰͼ�����
	sOneImg info = m_ImgSerial.GetCurrentMatImg();								//�����е�ǰͼ��
	m_pLUView->SetImgData(info);												//���õ�ǰͼ��
}

void CMainFrame::SetStateText(int num, CString szText)
{
	GetStatusBar()->SetPaneText(num, szText);
}

//���ô���λ��������
void CMainFrame::OnWindow()
{
	CWndWidthPosDlg dlg;
	dlg.DoModal();
}

//��������ͼ��
void CMainFrame::OnSaveserial()
{
	CSaveImgDlg dlg(m_ImgSerial.m_vImgSerial);
	if (IDCANCEL == dlg.DoModal())
		return;
}

//��ʾʸ״��
void CMainFrame::ShowSagittal(int col, int line)
{
	Mat src = m_ImgSerial.GetSagittal(col, fSlice);
	Mat tmp(src.size(),CV_8UC3);
	cvtColor(src, tmp, CV_GRAY2RGB);
	//�����ߣ���״���Ӧλ��
	for (int i=0;i<tmp.rows;++i)
	{
		//tmp.at<cv::Vec3b>(i,line) = cv::Vec3b
		tmp.at<cv::Vec3w>(i,line)[0] = 65535;
		tmp.at<cv::Vec3w>(i,line)[1] = 0;
		tmp.at<cv::Vec3w>(i,line)[2] = 0;
	}
	//�����ߣ�������Ӧλ��
	int num = m_ImgSerial.GetCurrentNum();
	num = num * fSlice/m_ImgSerial.GetImageNum();
	for (int i=0;i<tmp.cols;++i)
	{
		tmp.at<cv::Vec3w>(num,i)[0] = 0;
		tmp.at<cv::Vec3w>(num,i)[1] = 0;
		tmp.at<cv::Vec3w>(num,i)[2] = 65535;
	}
	sOneImg img;
	img.info = m_ImgSerial.GetBegin()->info;
	img.masking = tmp.clone();
	m_pLDView->SetImgData(img);
}

//��ʾ��״��
void CMainFrame::ShowCoronal(int row, int line)
{
	Mat src = m_ImgSerial.GetCoronal(row,fSlice);
	Mat tmp(src.size(),CV_8UC3);
	cvtColor(src, tmp, CV_GRAY2RGB);
	for (int i=0;i<tmp.rows;++i)
	{
		tmp.at<cv::Vec3w>(i,line)[0] = 65535;
		tmp.at<cv::Vec3w>(i,line)[1] = 0;
		tmp.at<cv::Vec3w>(i,line)[2] = 0;
	}
	//�����ߣ�������Ӧλ��
	int num = m_ImgSerial.GetCurrentNum();
	num = num*fSlice/m_ImgSerial.GetImageNum();
	for (int i=0;i<tmp.cols;++i)
	{
		tmp.at<cv::Vec3w>(num, i)[0] = 0;
		tmp.at<cv::Vec3w>(num, i)[1] = 0;
		tmp.at<cv::Vec3w>(num, i)[2] = 65535;
	}
	sOneImg img;
	img.info = m_ImgSerial.GetBegin()->info;
	img.masking = tmp.clone();
	m_pRUView->SetImgData(img);
}

//��ʾר�ұ�����ݲ˵���Ӧ
void CMainFrame::OnDisplayNote()
{
	m_isDisplayNote = (m_isDisplayNote == FALSE);
}

//��ʾר�ұ�����ݲ˵���Ӧ���˵�״̬
void CMainFrame::OnUpdateDisplayNote(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_isDisplayNote == TRUE);
}

void CMainFrame::OnBtnClick()
{
	m_bCanClick = (m_bCanClick == false);
}

void CMainFrame::OnUpdateBtnClick(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_bCanClick == TRUE);
}


BOOL CMainFrame::OnEraseBkgnd(CDC* pDC)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if (m_pLUView)
		m_pLUView->Invalidate(FALSE);
	if (m_pRUView)
		m_pRUView->Invalidate(FALSE);
	if (m_pLDView)
		m_pLDView->Invalidate(FALSE);
	if (m_pRDView)
		m_pRDView->Invalidate(FALSE);

	return TRUE;
	//return CFrameWnd::OnEraseBkgnd(pDC);
}


/*===========================================================================*
* �������Ϣ·��
*============================================================================*/
BOOL CMainFrame::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	return CFrameWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

/*===========================================================================*
* ����ܵĴ���
*============================================================================*/
int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (!m_wndStatusBar.Create(this))											//����״̬��					
		return -1;										
	m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT));	//״̬����������
	m_wndStatusBar.SetPaneInfo(0, ID_SEPARATOR, SBPS_NORMAL, 300);				//״̬����ʾ�������úͳ��ȴ�С����
	m_wndStatusBar.SetPaneInfo(1, IDS_ALLIMGNUM, SBPS_NORMAL, 150);
	m_wndStatusBar.SetPaneInfo(2, IDS_CURRENTIMGNUM, SBPS_NORMAL, 150);
	m_wndStatusBar.SetPaneInfo(3, IDS_CURRENTIMGNUM, SBPS_NORMAL, 120);

	if (!m_toolBarFunc.CreateEx(this,TBSTYLE_FLAT, WS_CHILD|WS_VISIBLE|CBRS_LEFT
		|CBRS_TOOLTIPS|CBRS_FLYBY|CBRS_SIZE_DYNAMIC)							//������๤����
		|| !m_toolBarFunc.LoadToolBar(IDR_TOOLBAR1))return -1;

	m_toolBarFunc.EnableDocking(CBRS_ALIGN_ANY);								//����������ͣ��
	EnableDocking(CBRS_ALIGN_ANY);												//����֧��ͣ��
	DockControlBar(&m_toolBarFunc);												//ͣ��������

	if (!m_toolBarFile.CreateEx(this,TBSTYLE_FLAT, WS_CHILD|WS_VISIBLE|CBRS_TOP	//�����Ϸ�������
		|CBRS_TOOLTIPS|CBRS_FLYBY|CBRS_SIZE_DYNAMIC)
		|| !m_toolBarFile.LoadToolBar(IDR_TOOLBAR2))return -1;

	m_toolBarFile.EnableDocking(CBRS_ALIGN_ANY);								//����������ͣ��
	EnableDocking(CBRS_ALIGN_ANY);												//����֧��ͣ��
	DockControlBar(&m_toolBarFile);												//ͣ��������

	PostMessage(UM_PROSSESS);													//������Ϣ����״̬���ϴ���������
				
	TCHAR szCurDir[512];						   								//��ȡEXEĿ¼
	memset(szCurDir, 0, sizeof(szCurDir));
	int length = 0;
	GetModuleFileName(NULL, szCurDir, length);									//��ȡӦ�ó���ǰ����·��
	m_szAppPath.Format("%s",szCurDir);
	int end = m_szAppPath.ReverseFind('\\');									//�������'\'ȥ��Ӧ�ó�����
	m_szAppPath = m_szAppPath.Left(end);			
	
	m_Log.InitLog(m_szAppPath);													//��ʼ����־����
	m_Log.WriteLog("%s","Ӧ�ó���򿪳ɹ�");										//д��־

	Global::InitStatics();														//��ʼ��ȫ�ֱ���
	
	m_toolBarFile.SetButtonText(0, "��");										//������������ʾ
	m_toolBarFile.SetButtonText(1, "����");
	m_toolBarFile.SetButtonText(2, "���");
	m_toolBarFile.SetButtonText(3, "��ӡ");
	CRect rect;
	m_toolBarFile.GetItemRect(0, &rect);
	m_toolBarFile.SetSizes(CSize(rect.Width(),rect.Height()),CSize(16,16));		//������������С
	m_toolBarFile.SetWindowText("�ļ�");
	FloatControlBar(&m_toolBarFile, CPoint(1220,80));							//����������
	m_hMutex = CreateMutex(NULL, FALSE, NULL);									//��������������������ͬ��
	return 0;
}
