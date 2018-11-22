/**********************************************************************************
  Copyright @ Robin 2015-2025
  File name:  OprtView.cpp
  Author: Robin   Version: V1.0   Date:2015.11.05 
  Description:  operate form interface class
  Others:         
***********************************************************************************/
#include "stdafx.h"
#include "RBDcm.h"
#include "OprtView.h"

//define Command code of operations
const int  HIST     =		0;
const int  CUT      =		1;
const int  RST      =		2;
const int  MASK     =		3;
const int  MEDIAN   =		4;
const int  DRAWHIST =		5;
const int  ENTROPY  =		6;
const int  OSTU     =		7;
const int  ENHENCE  =		8;
const int  SRCIMG   =		9;
const int  LUNG     =		10;
const int  VIDEO    =		11; 
const int  PLAY     =		12; 
const int  LAST     =		13;
const int  NEXT     =		14;
const int  MEAN     =		15; 
const int  MOM      =		16;
const int LOADXML   =       17;


// COprtView
IMPLEMENT_DYNCREATE(COprtView, CFormView)

COprtView::COprtView()
	: CFormView(COprtView::IDD)
	, m_nSUM(0)
	, m_nCurrent(0)
	, m_PlayDlg(NULL)
{
}

COprtView::~COprtView()
{
}

/***************************************************************************************
Function: �ؼ�����ֵ�������ֵ����ֵת��
Input:    CDataExchange��ָ��
Output:   void
Description: MFC�ڲ�����ת������
Return:   void
Others:   NULL
***************************************************************************************/
void COprtView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BTN_RST, m_reset);
	DDX_Control(pDX, IDC_BTN_DRAWHIST, m_btnHist);
	DDX_Control(pDX, IDC_BTN_HIST, m_btnEnhance);
	DDX_Control(pDX, IDC_BTN_CUT, m_btnCut);
	DDX_Control(pDX, IDC_BTN_MASK, m_btnMask);
	DDX_Control(pDX, IDC_BTN_MEDIAN, m_btnMed);
	DDX_Control(pDX, IDC_BTN_ENTROPY, m_btnEntropy);
	DDX_Control(pDX, IDC_BTN_OSTU, m_btnOSTU);
	DDX_Control(pDX, IDC_BTN_HENCE, m_btnHence);
	DDX_Control(pDX, IDC_BTN_SRCIMG, m_btnSrc);
	DDX_Control(pDX, IDC_BTN_LUNG3, m_btnLung);
	DDX_Control(pDX, IDC_BTN_VIDEO, m_btnCam);
	DDX_Control(pDX, IDC_BTN_PLAY, m_btnPlay);
	DDX_Control(pDX, IDC_BTN_LAST, m_btnLast);
	DDX_Control(pDX, IDC_BTN_NEXT, m_btnNext);
	DDX_Control(pDX, IDC_BTN_MEAN, m_btnMean);
	DDX_Control(pDX, IDC_BTN_MOM, m_btnMoM);
	DDX_Control(pDX, IDC_BTN_LOADXML, m_btnLoadXML);
	DDX_Control(pDX, IDC_BTN_SERIAL, m_btnSerialSeg);
	DDX_Text(pDX, IDC_EDIT_SUM, m_nSUM);
	DDX_Text(pDX, IDC_EDIT_CURRENT, m_nCurrent);
}


BEGIN_MESSAGE_MAP(COprtView, CFormView)
	ON_BN_CLICKED(IDC_BTN_HIST, &COprtView::OnBnClickedBtnHist)
	ON_BN_CLICKED(IDC_BTN_CUT, &COprtView::OnBnClickedBtnCut)
	ON_BN_CLICKED(IDC_BTN_RST, &COprtView::OnBnClickedBtnRst)
	ON_BN_CLICKED(IDC_BTN_MASK, &COprtView::OnBnClickedBtnMask)
	ON_BN_CLICKED(IDC_BTN_MEDIAN, &COprtView::OnBnClickedBtnMedian)
	ON_BN_CLICKED(IDC_BTN_DRAWHIST, &COprtView::OnBnClickedBtnDrawhist)
	ON_BN_CLICKED(IDC_BTN_ENTROPY, &COprtView::OnBnClickedBtnEntropy)
	ON_BN_CLICKED(IDC_BTN_OSTU, &COprtView::OnBnClickedBtnOstu)
	ON_BN_CLICKED(IDC_BTN_HENCE, &COprtView::OnBnClickedBtnHence)
	ON_BN_CLICKED(IDC_BTN_SRCIMG, &COprtView::OnBnClickedBtnSrcimg)
	ON_BN_CLICKED(IDC_BTN_LUNG3, &COprtView::OnBnClickedBtnLung)
	ON_BN_CLICKED(IDC_BTN_VIDEO, &COprtView::OnBnClickedBtnVideo)
	ON_BN_CLICKED(IDC_BTN_PLAY, &COprtView::OnBnClickedBtnPlay)
	ON_BN_CLICKED(IDC_BTN_LAST, &COprtView::OnBnClickedBtnLast)
	ON_BN_CLICKED(IDC_BTN_NEXT, &COprtView::OnBnClickedBtnNext)
	ON_BN_CLICKED(IDC_BTN_MEAN, &COprtView::OnBnClickedBtnMean)
	ON_BN_CLICKED(IDC_BTN_MOM, &COprtView::OnBnClickedBtnMom)
	ON_BN_CLICKED(IDC_BTN_LOADXML, &COprtView::OnBnClickedBtnLoadxml)
	ON_BN_CLICKED(IDC_BTN_SERIAL, &COprtView::OnBnClickedBtnSerial)
	ON_COMMAND(ID_FILE_SAVE, &COprtView::OnFileSave)
	ON_COMMAND(ID_SERILE_OPEN, &COprtView::OnSerileOpen)
	ON_COMMAND(ID_FILE_OPEN_ONE, &COprtView::OnFileOpenOne)
	ON_BN_CLICKED(IDC_BTN_GETNUDOUL, &COprtView::OnBnClickedBtnGetnudoul)
END_MESSAGE_MAP()


// COprtView ���
#ifdef _DEBUG
void COprtView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void COprtView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


/***************************************************************************************
Function: �����ʼ��
Input:    NULL
Output:  void
Description: ����Button����ʾIcon
Return:   void
Others:   NULL
***************************************************************************************/
void COprtView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	m_pMainFrm = (CMainFrame*)AfxGetMainWnd();
	m_reset.SetIcon(IDI_RESET);             //��ԭ
	m_btnHist.SetIcon(IDI_HIST);            //ֱ��ͼ
	m_btnEnhance.SetIcon(IDI_ENHANCE);      //ȫ��ֱ��ͼ��ǿ
	m_btnCut.SetIcon(IDI_CUT);              //�ֶ���ֵ
	m_btnMask.SetIcon(IDI_MASK);            //Mask
	m_btnMed.SetIcon(IDI_MED);              //��ֵ�˲�
	m_btnEntropy.SetIcon(IDI_ENTROPY);      //�طָ�
	m_btnOSTU.SetIcon(IDI_OSTU);            //OSTU�ָ�
	m_btnHence.SetIcon(IDI_HENCE);          //�߼���ǿ
	m_btnSrc.SetIcon(IDI_SRC);              //�鿴ԭͼ
	m_btnLung.SetIcon(IDI_LUNG);            //�ָ��ʵ��
	m_btnCam.SetIcon(IDI_ENHANCE);          //����ͷ
	m_btnPlay.SetIcon(IDI_PLAY);			//���в���
	m_btnLast.SetIcon(IDI_LAST);			//��һ��
	m_btnNext.SetIcon(IDI_NEXT);			//��һ��
	m_btnMean.SetIcon(IDI_MEAN);			//��ֵ����
	m_btnMoM.SetIcon(IDI_OSTU);				//
	m_btnLoadXML.SetIcon(IDI_OSTU);			//
	m_btnSerialSeg.SetIcon(IDI_MEAN);		//
}



/***************************************************************************************
Function: ���ݰ��°���ִ����Ӧ����Ϣִ����Ӧ�Ĳ���
Input:    NULL
Output:  void
Description: ��Ӧ��Ϣ�ۺϵ�һ��Ϊ�˴���ļ�
Return:   void
Others:   NULL
***************************************************************************************/
void COprtView::Executive(int msg)
{
	CRBDcmDoc* m_pDoc = ((CMainFrame*)AfxGetMainWnd())->m_pLUView->GetDocument();
	if (m_pDoc->m_srcImage.empty())          //��ʾ������û��ͼ������ʱ�Ͳ��ô���ֱ�ӷ���
	{
		return;
	}
	switch (msg)
	{
	case HIST:
		m_pDoc->Hist();
		break;
	case CUT:
		m_pDoc->Cut();
		break;
	case RST:
		m_pDoc->Rst();
		break;
	case MASK:
		m_pDoc->Mask();
		break;
	case MEDIAN:
		m_pDoc->Median();
		break;
	case DRAWHIST:
		m_pDoc->DrawHist();
		break;
	case ENTROPY:
		m_pDoc->Entropy();
		break;
	case OSTU:
		m_pDoc->Ostu();
		break;
	case ENHENCE:
		m_pDoc->Enhance();
		break;
	case SRCIMG:
		m_pDoc->SrcImg();
		break;
	case LUNG:
		m_pDoc->SegLung();
		break;
	case VIDEO:
		m_pDoc->Video();
		break;
	case PLAY:
		m_pDoc->PlaySerile();
		break;
	case LAST:
		m_pDoc->Last();
		break;
	case NEXT:
		m_pDoc->Next();
		break;
	case MEAN:
		m_pDoc->Mean();
		break;
	case MOM:
		m_pDoc->MoM();
		break;
	case LOADXML:
		m_pDoc->LoadXml();
	default:
		break;
	}
	m_pDoc->ShowImg();
	(m_pMainFrm->m_pLUView)->Invalidate(FALSE);
}

//Histogram Equalization
void COprtView::OnBnClickedBtnHist()
{
	Executive(HIST);
}

//Image data reset
void COprtView::OnBnClickedBtnRst()
{
	Executive(RST);
}

//median filter
void COprtView::OnBnClickedBtnMedian()
{
	Executive(MEDIAN);
}

//Max entropy segment
void COprtView::OnBnClickedBtnEntropy()
{
	//Executive(ENTROPY);
// 	createGaussianFilter(8,Size(5,5),1,1);
// 	cv::Mat kernel = getGaussianKernel(5,1);
	/*cv::Mat kernel = createGaussianFilter(8,Size(5,5),1,1);
	namedWindow("gaosi",CV_WINDOW_AUTOSIZE);
	cv::imshow("gaosi",kernel);*/
}



//Show source image 
void COprtView::OnBnClickedBtnSrcimg()
{
	Executive(SRCIMG);
}




void COprtView::SetSum(int num)
{
	m_nSUM = num;
	UpdateData(FALSE);
}


void COprtView::SetCurrent(int num)
{
	m_nCurrent = num;
	UpdateData(FALSE);
}


void COprtView::OnSerileOpen()
{
	/*
	CFileDialog dlg(TRUE);								//�ļ��򿪶Ի���
	CFileFind ff;										//�ļ�������
	CStringArray dcmPathstr;							//�ļ�·��+�ļ����б� dcm�ļ�
	dlg.m_ofn.lpstrTitle = "��ͼ������";				//ָ�����ļ��Ի�������
	dlg.m_ofn.lpstrFilter = "Dcm Files(*.dcm)\0*.dcm\0All Files(*.*)\0*.*\0\0";  //�ļ�������

	if (IDCANCEL == dlg.DoModal())						//�����ļ��򿪶Ի���ѡ��ȡ����ֱ�ӷ���
		return;

	CString strFileName = dlg.GetPathName();			//��ȡ�ļ�·��+�ļ���
	int end = strFileName.ReverseFind('\\');			//�������
	CString strFilePath = strFileName.Left(end);		//ȥ�������ļ����õ��ļ�Ŀ¼

	if(!ff.FindFile(strFilePath + "\\*.dcm"))			//����Ŀ¼���Ƿ����dcm�ļ������������˳�
		return;

	while(ff.FindNextFile())							//����dcm�ļ��������ļ�����·������dcmPathstr
		dcmPathstr.Add(strFilePath + "\\" + ff.GetFileName());

	m_ImgSerial.Clear();								//��������ǰ�����
	CString strXml = NULL;								//�ļ�·��
	for (int i=0; i<dcmPathstr.GetSize(); ++i)		
	{
		strFileName = dcmPathstr.GetAt(i);				//��������ȡ���ļ�·��
		m_ImgSerial.LoadDcm(strFileName);				//����Dcm�ļ�
	}
	SetSum(m_ImgSerial.GetImageNum());					//��ʾ��ǰͼ������
	SetCurrent(m_ImgSerial.GetCurrentNum());			//��ʾ��ǰͼ�����
	sOneImg info = m_ImgSerial.GetCurrentMatImg();		//ȡ�õ�ǰͼ��
	(m_pMainFrm->m_pRBView)->SetImgData(info);			//��ͼ�����õ�CRBDcm����
	*/
}


void COprtView::OnFileOpenOne()
{
	CFileDialog dlg(TRUE);								//�ļ��򿪶Ի���
	dlg.m_ofn.lpstrTitle = "�򿪵���ͼ��";				//ָ�����ļ��Ի�������
	//�ļ�������
	dlg.m_ofn.lpstrFilter = "Dcm Files(*.dcm)\0*.dcm\0All Files(*.*)\0*.*\0\0";		
	if (IDCANCEL == dlg.DoModal())						//�����ļ��򿪶Ի���ѡ��ȡ����ֱ�ӷ���
		return;
	CString strFileName = dlg.GetPathName();			//��ȡ�ļ�·��+�ļ���
	m_ImgSerial.Clear();								//���ԭ������
	m_ImgSerial.LoadDcm(strFileName);					//��������
	SetSum(m_ImgSerial.GetImageNum());					//��ʾ��ǰͼ������
	SetCurrent(m_ImgSerial.GetCurrentNum());			//��ʾ��ǰͼ�����
	sOneImg info = m_ImgSerial.GetCurrentMatImg();		//�����е�ǰͼ��
	(m_pMainFrm->m_pLUView)->SetImgData(info);			//���õ�ǰͼ��
	//(((CMainFrame*)AfxGetMainWnd())->m_pRBView)->SetImgData(info); //���õ�ǰͼ��
}


void COprtView::OnBnClickedBtnLoadxml()
{
	CFileDialog dlg(TRUE);								//�ļ��򿪶Ի���
	dlg.m_ofn.lpstrTitle = "��Xml�ļ�";				//ָ�����ļ��Ի�������
	//�ļ�������
	dlg.m_ofn.lpstrFilter = "xml Files(*.xml)\0*.xml\0All Files(*.*)\0*.*\0\0";  
	if (IDCANCEL == dlg.DoModal())						//�����ļ��򿪶Ի���ѡ��ȡ����ֱ�ӷ���
		return;
	CString strFileName = dlg.GetPathName();			//��ȡ�ļ�·��+�ļ���
	m_ImgSerial.LoadXml(strFileName);					//����xml�ļ�
	sOneImg info = m_ImgSerial.GetCurrentMatImg();
	//�����һ�����洫���ڵ���Ϣ
	(m_pMainFrm->m_pInfoView)->SetImgData(info);
	//(((CMainFrame*)AfxGetMainWnd())->m_pInfoView)->SetImgData(info);  
}


//OSTU segment
void COprtView::OnBnClickedBtnOstu()
{
	CSegmentOperat seg;
	sOneImg src = (m_pMainFrm->m_pLUView)->GetOneImg();
	cv::Mat m_dstImage = seg.OstuSeg(src.masking.clone()).clone();
	sOneImg info = m_ImgSerial.GetCurrentMatImg();
	info.masking = m_dstImage.clone();
	(m_pMainFrm->m_pRUView)->SetImgData(info);
	//(((CMainFrame*)AfxGetMainWnd())->m_pSegView)->SetImgData(info);
}


//Get real area of Lung
void COprtView::OnBnClickedBtnLung()
{
	//��ʵ�ʵķָ���ڷָ�������
	(m_pMainFrm->m_pRUView)->SegRealLung(m_ImgSerial.GetCurrentMatImg().masking.clone());
	//(((CMainFrame*)AfxGetMainWnd())->m_pSegView)->SegRealLung(m_ImgSerial.GetCurrentMatImg().pixle.clone());
}





void COprtView::OnBnClickedBtnMean()
{
	sOneImg img;
	(m_pMainFrm->m_pRUView)->GetSegRealLungs(img);
	medianBlur(img.masking, img.masking, 3);
	(m_pMainFrm->m_pLDView)->SegNodulesMean(img);
	//Executive(MEAN);
}


//Hence Image
void COprtView::OnBnClickedBtnHence()
{
	sOneImg img;
	(m_pMainFrm->m_pRUView)->GetSegRealLungs(img);
	(m_pMainFrm->m_pLDView)->HenssisenEnhance(img);
}


void COprtView::OnBnClickedBtnLast()
{
	sOneImg img = m_ImgSerial.LastMatImg();
	(m_pMainFrm->m_pLUView)->SetImgData(img);
	(m_pMainFrm->m_pInfoView)->SetImgData(img);
	SetCurrent(m_ImgSerial.GetCurrentNum());
}


void COprtView::OnBnClickedBtnNext()
{
	sOneImg img = m_ImgSerial.NextMatImg();
	(m_pMainFrm->m_pLUView)->SetImgData(img);
	(m_pMainFrm->m_pInfoView)->SetImgData(img);
	SetCurrent(m_ImgSerial.GetCurrentNum());
}


/***************************************************************************************
Function: ����ͼ��ֱ��ͼ
Input:    void
Output:   
Description:  ����ͼ��ֱ��ͼ�����ƽ�������Ի����ʾ, ���Ƶ��ǵ�ǰ��ʾ�������showImageͼ��
Return:   void
Others:   NULL
***************************************************************************************/
void COprtView::OnBnClickedBtnDrawhist()
{
	CDrawHistDlg dlg;
	dlg.SetHistValue(m_ImgSerial.GetCurrentMatImg().masking.clone());
	dlg.DoModal();
}


/***************************************************************************************
Function: ͼ����Ĥ
Input:    void
Output:   ʹ�ýṹԪ�ض�ͼ�������Ĥ��������Ĥ��������opencv
Description: 
Return:   void
Others:   NULL
***************************************************************************************/
void COprtView::OnBnClickedBtnMask()
{
	CMaskSetDlg dlg;
	if (IDCANCEL == dlg.DoModal())
		return;
	cv::Mat kernal;									//�����任��
	cv::Mat dst;									//Ŀ��ͼ��
	if (dlg.m_mask.weight != 0)
	{
		double tmp = (double)dlg.m_mask.weight;
		kernal = (Mat_<double>(3, 3) <<
			dlg.m_mask.mk[0][0]/tmp, dlg.m_mask.mk[0][1]/tmp, dlg.m_mask.mk[0][2]/tmp,
			dlg.m_mask.mk[1][0]/tmp, dlg.m_mask.mk[1][1]/tmp, dlg.m_mask.mk[1][2]/tmp,
			dlg.m_mask.mk[2][0]/tmp, dlg.m_mask.mk[2][1]/tmp, dlg.m_mask.mk[2][2]/tmp);
	}
	else
	{
		kernal = (Mat_<char>(3, 3) <<
			dlg.m_mask.mk[0][0], dlg.m_mask.mk[0][1], dlg.m_mask.mk[0][2],
			dlg.m_mask.mk[1][0], dlg.m_mask.mk[1][1], dlg.m_mask.mk[1][2],
			dlg.m_mask.mk[2][0], dlg.m_mask.mk[2][1], dlg.m_mask.mk[2][2]);
	}
	filter2D(m_ImgSerial.GetCurrentMatImg().masking.clone(), dst, 8, kernal);
	
}

//�����һ�������е�ͼ��
void COprtView::OnFileSave()
{
	/*
	CFileDialog dlg(FALSE);
	dlg.m_ofn.lpstrTitle = "�ļ�����";
	dlg.m_ofn.lpstrFilter = "Bmp Files(*.bmp)\0*.bmp\0All Files(*.*)\0*.*\0\0";
	dlg.m_ofn.lpstrDefExt = "bmp";
	sOneImg img = m_pMainFrm->m_pRBView->GetOneImg();
	if (dlg.DoModal() == IDOK)
	{
		imwrite((LPCTSTR)dlg.GetPathName(), img.pixle);
	}
	*/
}


//����ͷ��������
void COprtView::OnBnClickedBtnVideo()
{
	cv::VideoCapture cam(0);
	if(!cam.isOpened())
		return;
	cv::Mat frame;
	cv::Mat gary;
	bool stop = false;
	namedWindow("cam", 1);
	while (!stop)
	{
		cam>>frame;
		//��ʱ���ȡ�������ǿյ�
		if (frame.empty())
		{
			continue;
		}
		cvtColor(frame, gary, CV_BGR2GRAY);
		threshold(gary, gary, 100, 255, 0);  //
		imshow("cam", frame);
		if (waitKey(30) >= 0)
		{
			stop = true;
		}
	}
	HWND hWnd = ::FindWindow(NULL,"cam");
	::CloseWindow(hWnd);
	::DestroyWindow(hWnd);
	cam.release();
}


//threshold segmentation
void COprtView::OnBnClickedBtnCut()
{

}


//play image serials
void COprtView::OnBnClickedBtnPlay()
{
	if (m_PlayDlg != NULL)			//�������Ѿ������ڴ��У�����ʾ�Ϳ����ˣ����ⴴ���������
	{
		m_PlayDlg->SetImgSerial(m_ImgSerial.GetBegin(), m_ImgSerial.GetImageNum());
		m_PlayDlg->ShowWindow(SW_NORMAL);
		return;
	}
	m_PlayDlg = new CPlaySeriesDlg();
	CLUView *pView = ((CMainFrame*)AfxGetMainWnd())->m_pLUView;
	m_PlayDlg->Create(MAKEINTRESOURCE(IDD_PLAY_SERI), pView);
	m_PlayDlg->SetImgSerial(m_ImgSerial.GetBegin(), m_ImgSerial.GetImageNum());
	m_PlayDlg->ShowWindow(SW_NORMAL);
}



//����ͼ��ָ�
//ʹ��������������
void COprtView::OnBnClickedBtnSerial()
{
	CSegmentOperat seg;
	sOneImg img;					//��ʵ�ʴ����е�ͼ��
	sOneImg src;                    //ԭʼͼ��
	cv::Mat dst = Mat::zeros(src.masking.size(), CV_8UC1);
	cv::Mat tmp = Mat::zeros(src.masking.size(), CV_8UC1);
	src = (m_pMainFrm->m_pOpt)->m_ImgSerial.GetCurrentMatImg();
	(m_pMainFrm->m_pRUView)->GetSegRealLungs(img);
	CPoint pt1,pt2;
	vector<Point2i> vcPoint;
	seg.GetObjectCenter(img.masking,vcPoint);  //����

	for (size_t i = 0; i < vcPoint.size(); ++i)
	{
		tmp = seg.RegionGrow(src.masking, vcPoint[i], 10).clone();			//�����������
		Mat element = getStructuringElement(MORPH_ELLIPSE,cv::Size(7,7));	//�������Ľ����Ե�����޲�
		morphologyEx( tmp, tmp, MORPH_CLOSE, element);						//ʹ����̬ѧ������Ա�Ե�����޲�
		if (i == 0)															//Ԫͼ��Ϊ�յ�ʱ������п�¡
			dst = tmp.clone();
		else                                                                //ԭͼ��Ϊ�յ�ʱ�������ͼ��
			addWeighted(dst, 1, tmp, 1, 0, dst);
	}

	std::vector<std::vector<cv::Point>> contours;    //�߽�㼯��
	std::vector<cv::Vec4i> hierarchy;				 //�߽�
	findContours(dst.clone(), contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
	drawContours(dst, contours, -1, Scalar(255), CV_FILLED, 1, hierarchy);

	img.masking = dst.clone();							
	namedWindow("aaa",CV_WINDOW_AUTOSIZE);									//��ʾ�ָ��Ľ��
	imshow("aaa",img.masking);
	(m_pMainFrm->m_pLDView)->SetImgData(img);
}


#include "OneImgFeatures.h"
//
void COprtView::OnBnClickedBtnGetnudoul()
{
	//(m_pMainFrm->m_pClassier)->SetImgData(img);
	sOneImg img = (m_pMainFrm->m_pLDView)->GetOneImg();	//��ȡ��ڴ�����ʾ��ͼ�����ƽ�ڵĻҶ�ͼ��
	std::vector<std::vector<cv::Point>> contours;			//�߽�㼯��
	std::vector<cv::Vec4i> hierarchy;						//�߽�
	
	cv::medianBlur(img.masking, img.masking,3);
	findContours(img.masking.clone(), contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
	drawContours(img.masking, contours, -1, Scalar(255), CV_FILLED, 1, hierarchy);
	
	sOneImg simg;	//��ʵ�ʴ����е�ͼ��
	(m_pMainFrm->m_pRUView)->GetSegRealLungs(simg);
	bitwise_and(simg.masking, img.masking, img.masking);
	COneImgFeatures features;
	features.SetMatImgAndContours(img.masking, contours);
	features.CalculateFeatures();
	namedWindow("aaa",CV_WINDOW_AUTOSIZE);									//��ʾ�ָ��Ľ��
	imshow("aaa",img.masking);

}

//�����ָ�
void COprtView::OnBnClickedBtnMom()
{
	sOneImg img;	//��ʵ�ʴ����е�ͼ��
	(m_pMainFrm->m_pRUView)->GetSegRealLungs(img);
	medianBlur(img.masking, img.masking, 3);
	(m_pMainFrm->m_pLDView)->SegNodules(img);
	//(((CMainFrame*)AfxGetMainWnd())->m_pSegView)->GetSegRealLungs(img);
	//(((CMainFrame*)AfxGetMainWnd())->m_pClassier)->SegNodules(img);
}