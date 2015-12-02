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
Function: 控件中数值与变量数值间数值转换
Input:    CDataExchange类指针
Output:   void
Description: MFC内部数据转换机制
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


// COprtView 诊断
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
Function: 窗体初始化
Input:    NULL
Output:  void
Description: 设置Button上显示Icon
Return:   void
Others:   NULL
***************************************************************************************/
void COprtView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	m_pMainFrm = (CMainFrame*)AfxGetMainWnd();
	m_reset.SetIcon(IDI_RESET);             //复原
	m_btnHist.SetIcon(IDI_HIST);            //直方图
	m_btnEnhance.SetIcon(IDI_ENHANCE);      //全局直方图增强
	m_btnCut.SetIcon(IDI_CUT);              //手动阈值
	m_btnMask.SetIcon(IDI_MASK);            //Mask
	m_btnMed.SetIcon(IDI_MED);              //中值滤波
	m_btnEntropy.SetIcon(IDI_ENTROPY);      //熵分割
	m_btnOSTU.SetIcon(IDI_OSTU);            //OSTU分割
	m_btnHence.SetIcon(IDI_HENCE);          //高级增强
	m_btnSrc.SetIcon(IDI_SRC);              //查看原图
	m_btnLung.SetIcon(IDI_LUNG);            //分割肺实质
	m_btnCam.SetIcon(IDI_ENHANCE);          //摄像头
	m_btnPlay.SetIcon(IDI_PLAY);			//序列播放
	m_btnLast.SetIcon(IDI_LAST);			//上一张
	m_btnNext.SetIcon(IDI_NEXT);			//下一张
	m_btnMean.SetIcon(IDI_MEAN);			//均值迭代
	m_btnMoM.SetIcon(IDI_OSTU);				//
	m_btnLoadXML.SetIcon(IDI_OSTU);			//
	m_btnSerialSeg.SetIcon(IDI_MEAN);		//
}



/***************************************************************************************
Function: 根据按下按键执行相应的消息执行相应的操作
Input:    NULL
Output:  void
Description: 响应消息综合到一起，为了代码的简化
Return:   void
Others:   NULL
***************************************************************************************/
void COprtView::Executive(int msg)
{
	CRBDcmDoc* m_pDoc = ((CMainFrame*)AfxGetMainWnd())->m_pRBView->GetDocument();
	if (m_pDoc->m_srcImage.empty())          //显示窗口中没有图像数据时就不用处理，直接返回
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
	(m_pMainFrm->m_pRBView)->Invalidate(FALSE);
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
	CFileDialog dlg(TRUE);								//文件打开对话框
	CFileFind ff;										//文件查找类
	CStringArray dcmPathstr;							//文件路径+文件名列表 dcm文件
	dlg.m_ofn.lpstrTitle = "打开图像序列";				//指定打开文件对话框名称
	dlg.m_ofn.lpstrFilter = "Dcm Files(*.dcm)\0*.dcm\0All Files(*.*)\0*.*\0\0";  //文件过滤器

	if (IDCANCEL == dlg.DoModal())						//弹出文件打开对话框，选择取消则直接返回
		return;

	CString strFileName = dlg.GetPathName();			//获取文件路径+文件名
	int end = strFileName.ReverseFind('\\');			//逆向查找
	CString strFilePath = strFileName.Left(end);		//去掉最后的文件名得到文件目录

	if(!ff.FindFile(strFilePath + "\\*.dcm"))			//查找目录下是否存在dcm文件，不存在则退出
		return;

	while(ff.FindNextFile())							//存在dcm文件，将其文件完整路径存入dcmPathstr
		dcmPathstr.Add(strFilePath + "\\" + ff.GetFileName());

	m_ImgSerial.Clear();								//导入数据前先清除
	CString strXml = NULL;								//文件路径
	for (int i=0; i<dcmPathstr.GetSize(); ++i)		
	{
		strFileName = dcmPathstr.GetAt(i);				//从序列中取得文件路径
		m_ImgSerial.LoadDcm(strFileName);				//加载Dcm文件
	}
	SetSum(m_ImgSerial.GetImageNum());					//显示当前图像总数
	SetCurrent(m_ImgSerial.GetCurrentNum());			//显示当前图像序号
	sOneImg info = m_ImgSerial.GetCurrentMatImg();		//取得当前图像
	(m_pMainFrm->m_pRBView)->SetImgData(info);			//将图像设置到CRBDcm类中
}


void COprtView::OnFileOpenOne()
{
	CFileDialog dlg(TRUE);								//文件打开对话框
	dlg.m_ofn.lpstrTitle = "打开单张图像";				//指定打开文件对话框名称
	//文件过滤器
	dlg.m_ofn.lpstrFilter = "Dcm Files(*.dcm)\0*.dcm\0All Files(*.*)\0*.*\0\0";		
	if (IDCANCEL == dlg.DoModal())						//弹出文件打开对话框，选择取消则直接返回
		return;
	CString strFileName = dlg.GetPathName();			//获取文件路径+文件名
	m_ImgSerial.Clear();								//清除原有数据
	m_ImgSerial.LoadDcm(strFileName);					//加载数据
	SetSum(m_ImgSerial.GetImageNum());					//显示当前图像总数
	SetCurrent(m_ImgSerial.GetCurrentNum());			//显示当前图像序号
	sOneImg info = m_ImgSerial.GetCurrentMatImg();		//序列中当前图像
	(m_pMainFrm->m_pRBView)->SetImgData(info);			//设置当前图像
	//(((CMainFrame*)AfxGetMainWnd())->m_pRBView)->SetImgData(info); //设置当前图像
}


void COprtView::OnBnClickedBtnLoadxml()
{
	CFileDialog dlg(TRUE);								//文件打开对话框
	dlg.m_ofn.lpstrTitle = "打开Xml文件";				//指定打开文件对话框名称
	//文件过滤器
	dlg.m_ofn.lpstrFilter = "xml Files(*.xml)\0*.xml\0All Files(*.*)\0*.*\0\0";  
	if (IDCANCEL == dlg.DoModal())						//弹出文件打开对话框，选择取消则直接返回
		return;
	CString strFileName = dlg.GetPathName();			//获取文件路径+文件名
	m_ImgSerial.LoadXml(strFileName);					//加载xml文件
	sOneImg info = m_ImgSerial.GetCurrentMatImg();
	//向最后一个界面传入结节的信息
	(m_pMainFrm->m_pInfoView)->SetImgData(info);
	//(((CMainFrame*)AfxGetMainWnd())->m_pInfoView)->SetImgData(info);  
}


//OSTU segment
void COprtView::OnBnClickedBtnOstu()
{
	CSegmentOperat seg;
	sOneImg src = (m_pMainFrm->m_pRBView)->GetOneImg();
	cv::Mat m_dstImage = seg.OstuSeg(src.pixle.clone()).clone();
	sOneImg info = m_ImgSerial.GetCurrentMatImg();
	info.pixle = m_dstImage.clone();
	(m_pMainFrm->m_pSegView)->SetImgData(info);
	//(((CMainFrame*)AfxGetMainWnd())->m_pSegView)->SetImgData(info);
}


//Get real area of Lung
void COprtView::OnBnClickedBtnLung()
{
	//肺实质的分割放在分割窗口中完成
	(m_pMainFrm->m_pSegView)->SegRealLung(m_ImgSerial.GetCurrentMatImg().pixle.clone());
	//(((CMainFrame*)AfxGetMainWnd())->m_pSegView)->SegRealLung(m_ImgSerial.GetCurrentMatImg().pixle.clone());
}





void COprtView::OnBnClickedBtnMean()
{
	sOneImg img;
	(m_pMainFrm->m_pSegView)->GetSegRealLungs(img);
	medianBlur(img.pixle, img.pixle, 3);
	(m_pMainFrm->m_pClassier)->SegNodulesMean(img);
	//Executive(MEAN);
}


//Hence Image
void COprtView::OnBnClickedBtnHence()
{
	sOneImg img;
	(m_pMainFrm->m_pSegView)->GetSegRealLungs(img);
	(m_pMainFrm->m_pClassier)->HenssisenEnhance(img);
}


void COprtView::OnBnClickedBtnLast()
{
	sOneImg img = m_ImgSerial.LastMatImg();
	(m_pMainFrm->m_pRBView)->SetImgData(img);
	(m_pMainFrm->m_pInfoView)->SetImgData(img);
	SetCurrent(m_ImgSerial.GetCurrentNum());
}


void COprtView::OnBnClickedBtnNext()
{
	sOneImg img = m_ImgSerial.NextMatImg();
	(m_pMainFrm->m_pRBView)->SetImgData(img);
	(m_pMainFrm->m_pInfoView)->SetImgData(img);
	SetCurrent(m_ImgSerial.GetCurrentNum());
}


/***************************************************************************************
Function: 绘制图像直方图
Input:    void
Output:   
Description:  绘制图像直方图，绘制结果弹出对话框表示, 绘制的是当前显示框里面的showImage图像
Return:   void
Others:   NULL
***************************************************************************************/
void COprtView::OnBnClickedBtnDrawhist()
{
	CDrawHistDlg dlg;
	dlg.SetHistValue(m_ImgSerial.GetCurrentMatImg().pixle.clone());
	dlg.DoModal();
}


/***************************************************************************************
Function: 图像掩膜
Input:    void
Output:   使用结构元素对图像进行掩膜操作，掩膜函数调用opencv
Description: 
Return:   void
Others:   NULL
***************************************************************************************/
void COprtView::OnBnClickedBtnMask()
{
	CMaskSetDlg dlg;
	if (IDCANCEL == dlg.DoModal())
		return;
	cv::Mat kernal;									//构建变换核
	cv::Mat dst;									//目标图像
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
	filter2D(m_ImgSerial.GetCurrentMatImg().pixle.clone(), dst, 8, kernal);
	
}


void COprtView::OnFileSave()
{

}


//摄像头开启程序
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
		//有时候读取出来的是空的
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
	if (m_PlayDlg != NULL)			//若对象已经存在内存中，则显示就可以了，避免创建多个对象
	{
		m_PlayDlg->SetImgSerial(m_ImgSerial.GetBegin(), m_ImgSerial.GetImageNum());
		m_PlayDlg->ShowWindow(SW_NORMAL);
		return;
	}
	m_PlayDlg = new CPlaySeriesDlg();
	CRBDcmView *pView = ((CMainFrame*)AfxGetMainWnd())->m_pRBView;
	m_PlayDlg->Create(MAKEINTRESOURCE(IDD_PLAY_SERI), pView);
	m_PlayDlg->SetImgSerial(m_ImgSerial.GetBegin(), m_ImgSerial.GetImageNum());
	m_PlayDlg->ShowWindow(SW_NORMAL);
}



//序列图像分割
//使用区域生长方法
void COprtView::OnBnClickedBtnSerial()
{
	CSegmentOperat seg;
	sOneImg img;					//肺实质窗口中的图像
	sOneImg src;                    //原始图像
	cv::Mat dst = Mat::zeros(src.pixle.size(), CV_8UC1);
	cv::Mat tmp = Mat::zeros(src.pixle.size(), CV_8UC1);
	src = (m_pMainFrm->m_pOpt)->m_ImgSerial.GetCurrentMatImg();
	(m_pMainFrm->m_pSegView)->GetSegRealLungs(img);
	CPoint pt1,pt2;
	vector<Point2i> vcPoint;
	seg.GetObjectCenter(img.pixle,vcPoint);  //处理

	for (size_t i = 0; i < vcPoint.size(); ++i)
	{
		tmp = seg.RegionGrow(src.pixle, vcPoint[i], 10).clone();			//区域生长结果
		Mat element = getStructuringElement(MORPH_ELLIPSE,cv::Size(7,7));	//对生长的结果边缘进行修补
		morphologyEx( tmp, tmp, MORPH_CLOSE, element);						//使用形态学闭运算对边缘进行修补
		if (i == 0)															//元图像为空的时候则进行克隆
			dst = tmp.clone();
		else                                                                //原图像不为空的时候则叠加图像
			addWeighted(dst, 1, tmp, 1, 0, dst);
	}

	std::vector<std::vector<cv::Point>> contours;    //边界点集合
	std::vector<cv::Vec4i> hierarchy;				 //边界
	findContours(dst.clone(), contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
	drawContours(dst, contours, -1, Scalar(255), CV_FILLED, 1, hierarchy);

	img.pixle = dst.clone();							
	namedWindow("aaa",CV_WINDOW_AUTOSIZE);									//显示分割后的结果
	imshow("aaa",img.pixle);
	(m_pMainFrm->m_pClassier)->SetImgData(img);
}


#include "Features.h"
//
void COprtView::OnBnClickedBtnGetnudoul()
{
	//(m_pMainFrm->m_pClassier)->SetImgData(img);
	sOneImg img = (m_pMainFrm->m_pClassier)->GetOneImg();	//获取结节窗中显示的图像，疑似结节的灰度图像
	std::vector<std::vector<cv::Point>> contours;			//边界点集合
	std::vector<cv::Vec4i> hierarchy;						//边界
	
	cv::medianBlur(img.pixle, img.pixle,3);
	findContours(img.pixle.clone(), contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
	drawContours(img.pixle, contours, -1, Scalar(255), CV_FILLED, 1, hierarchy);
	
	sOneImg simg;	//肺实质窗口中的图像
	(m_pMainFrm->m_pSegView)->GetSegRealLungs(simg);
	bitwise_and(simg.pixle, img.pixle, img.pixle);
	CFeatures features;
	features.SetMatImgAndContours(img.pixle, contours);
	features.CalculateFeatures();
	namedWindow("aaa",CV_WINDOW_AUTOSIZE);									//显示分割后的结果
	imshow("aaa",img.pixle);

}

//矩量分割
void COprtView::OnBnClickedBtnMom()
{
	sOneImg img;	//肺实质窗口中的图像
	(m_pMainFrm->m_pSegView)->GetSegRealLungs(img);
	medianBlur(img.pixle, img.pixle, 3);
	(m_pMainFrm->m_pClassier)->SegNodules(img);
	
	//(((CMainFrame*)AfxGetMainWnd())->m_pSegView)->GetSegRealLungs(img);
	//(((CMainFrame*)AfxGetMainWnd())->m_pClassier)->SegNodules(img);
}