/**********************************************************************************
  Copyright @ Robin 2015-2025
  File name:  RBDcmDoc.cpp
  Author: Robin   Version: V1.0   Date: 2015.11.05 
  Description: Software Frame  Docment Class 
               This class contain main image algorithm  
  Others:         
***********************************************************************************/
#include "stdafx.h"

#ifndef SHARED_HANDLERS
#include "RBDcm.h"
#endif

#include "MainFrm.h"
#include "RBDcmDoc.h"

#include "MaskSetDlg.h"
#include "SetValueDlg.h"
#include "DrawHistDlg.h"
#include "HenceDlg.h"
#include "SrcShowDlg.h"
#include "OprtView.h"
#include "PlaySeriesDlg.h"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CRBDcmDoc
IMPLEMENT_DYNCREATE(CRBDcmDoc, CDocument)

BEGIN_MESSAGE_MAP(CRBDcmDoc, CDocument)
	ON_COMMAND(ID_FILE_SAVE, &CRBDcmDoc::OnFileSave)
	ON_COMMAND(ID_IMGINFODIS, &CRBDcmDoc::OnImginfodis)
	ON_UPDATE_COMMAND_UI(ID_IMGINFODIS, &CRBDcmDoc::OnUpdateImginfodis)
	ON_COMMAND(IDM_SERILE, &CRBDcmDoc::OnSerile)
END_MESSAGE_MAP()

// CRBDcmDoc 构造/析构
CRBDcmDoc::CRBDcmDoc()
	: m_PlayDlg(NULL)
{}

CRBDcmDoc::~CRBDcmDoc()
{}

BOOL CRBDcmDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;
	return TRUE;
}


/***************************************************************************************
Function: CRBDcmDoc 序列化 (mfc自动生成代码，暂时没有使用)
Input:    
Output:   NULL
Description: NULL
Return:   MatGroup
Others:   NULL
***************************************************************************************/
void CRBDcmDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: 在此添加存储代码
	}
	else
	{
		// TODO: 在此添加加载代码
	}
}

#ifdef SHARED_HANDLERS

// 缩略图的支持
void CRBDcmDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// 修改此代码以绘制文档数据
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// 搜索处理程序的支持
void CRBDcmDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// 从文档数据设置搜索内容。
	// 内容部分应由“;”分隔
	// 例如:  strSearchContent = _T("point;rectangle;circle;ole object;")；
	SetSearchContent(strSearchContent);
}

void CRBDcmDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = NULL;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != NULL)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CRBDcmDoc 诊断
#ifdef _DEBUG
void CRBDcmDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CRBDcmDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


/***************************************************************************************
Function: 响应菜单"打开序列图像"
Input:    NULL
Output:   NULL
Description: 对指定的文件目录下的dcm图像进行转换操作，转换成8bitMat数据存入
             成员变量m_SerileMat(vector)
Return:   void
Others:   NULL
***************************************************************************************/
void CRBDcmDoc::OnSerile()
{
	CFileDialog dlg(TRUE);							//文件打开对话框
	CFileFind ff;									//文件查找类
	CStringArray dcmPathstr;						//文件路径+文件名列表 dcm文件
	dlg.m_ofn.lpstrTitle = "打开图像序列";			//指定打开文件对话框名称
	dlg.m_ofn.lpstrFilter = "Dcm Files(*.dcm)\0*.dcm\0All Files(*.*)\0*.*\0\0";  //文件过滤器
	
	if (FALSE == dlg.DoModal())						//弹出文件打开对话框，选择取消则直接返回
		return;

	CString strFileName = dlg.GetPathName();	   //获取文件路径+文件名
	int end = strFileName.ReverseFind('\\');       
	CString strFilePath = strFileName.Left(end);   //去掉最后的文件名得到文件目录

	if(!ff.FindFile(strFilePath + "\\*.dcm"))      //查找目录下是否存在dcm文件，不存在则退出
		return;

	while(ff.FindNextFile())						//存在dcm文件，将其文件完整路径存入dcmPathstr
		dcmPathstr.Add(strFilePath + "\\" + ff.GetFileName());

	m_matimg.Clear();								//导入数据前先清除
	CString strXml = NULL;
	for (int i=0; i<dcmPathstr.GetSize(); ++i)		
	{
		strFileName = dcmPathstr.GetAt(i);
		m_matimg.LoadDcm(strFileName);
	}

	//向控制窗体上编辑框内写入图像序列总张数
	
	//COprtView *pOpt = (COprtView*)(((CMainFrame*)AfxGetMainWnd())->m_splt.GetPane(0, 0));
	((CMainFrame*)AfxGetMainWnd())->m_pOpt->SetSum(m_matimg.GetImageNum());
	((CMainFrame*)AfxGetMainWnd())->m_pOpt->SetCurrent(m_matimg.GetCurrentNum());

	m_srcImage = m_matimg.GetCurrentMatImg().pixle.clone();
	m_showImage = m_srcImage.clone();
	m_dstImage = m_srcImage.clone();
	
	CLayer* pLayer = NULL;
	m_ls.RemoveAll();								//删除所有图层
	pLayer = CLayer::Create(IMG_LAYER);				//创建图像数据显示层
	m_ls.Add(pLayer);
	pLayer->SetLayerImgData(m_showImage.clone());
	
	pLayer = CLayer::Create(IMG_INFO_LAYER);		//创建图像相关信息显示层
	m_ls.Add(pLayer);
	pLayer->SetLayerInfoData(m_matimg.GetCurrentMatImg().info);
	UpdateAllViews(0, 0, 0);
}


/***************************************************************************************
Function: 打开文件响应
Input:    LPCTSTR lpszPathName完整文件路径
Output:   BOOL
Description: NULL
Return:   MatGroup
Others:   NULL
***************************************************************************************/
BOOL CRBDcmDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;

	m_matimg.Clear();
	m_matimg.LoadDcm(lpszPathName);				//Load Dcm image to m_matimg struct

	COprtView *pOpt = (COprtView*)(((CMainFrame*)AfxGetMainWnd())->m_splt.GetPane(0, 0));
	pOpt->SetSum(1);
	pOpt->SetCurrent(1);

	//Clone mat image to src and showing
	m_srcImage = m_matimg.GetCurrentMatImg().pixle.clone();
	m_showImage = m_srcImage.clone();

	CLayer* pLayer = NULL;						//Create layers
	m_ls.RemoveAll();							//delete all layers
	
	pLayer = CLayer::Create(IMG_LAYER);			//create images layer 
	m_ls.Add(pLayer);							//add to the layer array
	pLayer->SetLayerImgData(m_showImage);       //add layer data 
	
	pLayer = CLayer::Create(IMG_INFO_LAYER);	//create info layer 
	m_ls.Add(pLayer);
	pLayer->SetLayerInfoData(m_matimg.GetCurrentMatImg().info);
	
	return TRUE;
}

/***************************************************************************************
Function: 直方图均衡
Input:    void
Output:   对显示的图像进行处理，处理结果存在dst图像
Description: 调用opencv直方图均衡函数对图像进行处理
Return:   void
Others:   NULL
***************************************************************************************/
void CRBDcmDoc::Hist(void)
{
	if (!m_showImage.empty())
	{
		equalizeHist(m_showImage, m_dstImage);  //opencv库函数
		m_showImage = m_dstImage.clone();
	}
}


/***************************************************************************************
Function: 阈值分割
Input:    void
Output:   对显示的图像进行处理，处理结果存在dst图像
Description: 调用opencv阈值处理函数对图像进行处理
Return:   void
Others:   NULL
***************************************************************************************/
void CRBDcmDoc::Cut(void)
{
	CSetValueDlg dlg;
	CSegmentOperat seg;
	dlg.SetTitle("设定阈值");
	dlg.SetValue(120);
	if (dlg.DoModal() == IDCANCEL)
		return;
	if (!m_showImage.empty())
	{
		//m_dstImage = seg.IsodataSeg(m_showImage);
		threshold(m_showImage, m_dstImage, dlg.GetValue(), 255, 0);
		m_showImage = m_dstImage.clone();
		ShowImg();
	}
}


/***************************************************************************************
Function: 图像复位
Input:    void
Output:   清楚dst中的图像数据，拷贝src中图像到show
Description: 
Return:   void
Others:   NULL
***************************************************************************************/
void CRBDcmDoc::Rst(void)
{
	if (!m_ls.IsEmpty())
	{
		CImgDataLayer *pImgLayer = (CImgDataLayer*)(m_ls[0]);
		pImgLayer->SetTimes(1.0);
	}
	
	m_dstImage.release();               //清除处理后的图像
	m_showImage = m_srcImage.clone();   
}


/***************************************************************************************
Function: 图像掩膜
Input:    void
Output:   使用结构元素对图像进行掩膜操作，掩膜函数调用opencv
          处理结果放在dst
Description: 
Return:   void
Others:   NULL
***************************************************************************************/
void CRBDcmDoc::Mask(void)
{
}


/***************************************************************************************
Function: 图像中值滤波
Input:    void
Output:   对图像进行中值滤波，窗口大小由用户确定
          处理结果放在dst
Description: 
Return:   void
Others:   NULL
***************************************************************************************/
void CRBDcmDoc::Median(void)
{
	CSetValueDlg dlg;
	dlg.SetValue(3);
	dlg.SetTitle("设置窗口大小");
	if (m_showImage.empty())
	{
		AfxMessageBox("没有处理对象!");
		return;
	}
	if(dlg.DoModal() == IDCANCEL)
		return;
	//检查输入值是否合乎要求，模板为偶数时会导致异常
	if ((dlg.GetValue() % 2) == 0)
	{
		AfxMessageBox("窗口值必须为奇数！");
		return;
	}
	medianBlur(m_showImage, m_dstImage, dlg.GetValue());
	m_showImage = m_dstImage.clone();
}



void CRBDcmDoc::DrawHist(void)
{
}


/***************************************************************************************
Function: 多尺度汉森矩阵增强
Input:    void
Output:   
Description: 
Return:   void
Others:   NULL
***************************************************************************************/
void CRBDcmDoc::Enhance(void)
{
	/*CSegmentOperat seg;
	m_dstImage = seg.RegionGrow(m_showImage.clone(),CPoint(150,227),20).clone();
	m_showImage = m_dstImage.clone();*/
	
	m_dstImage = CEnhanceOperat::HessianEnhance(m_showImage,1,10).clone();
	m_showImage = m_dstImage.clone();

	/*
	CHenceDlg dlg;
	if (IDOK == dlg.DoModal())
	{
		//CSetValueDlg dlg;
		//if (IDOK == dlg.DoModal())
		//{
		//	int abc = dlg.GetValue();
		//	GaussianBlur(m_showImage,m_dstImage,cv::Size(3, 3), abc, abc);
		//}
		
		m_dstImage = ManyLevelHessianEnhance2(m_showImage.clone()).clone();
		m_showImage = m_dstImage.clone();
	}
	*/
}


/***************************************************************************************
Function: 最大熵分割
Input:    void
Output:   
Description: 
Return:   void
Others:   NULL
***************************************************************************************/
void CRBDcmDoc::Entropy(void)
{
	if (m_showImage.empty())
	{
		AfxMessageBox("没有处理对象!");
		return;
	}
	CSegmentOperat seg;
	m_dstImage = seg.EntropySeg(m_showImage.clone()).clone();
	/*CString str;
	str.Format("分割阈值为:%d",index);
	SetStatusBar(str);*/
	//AddTimeToStatusBar(::GetTickCount() - start);
	m_showImage = m_dstImage.clone();
}


/***************************************************************************************
Function: Ostu分割
Input:    void
Output:   
Description: 处理结果存在dst中 调用CSegmentoperat类中的函数进行分割
Return:   void
Others:   NULL
***************************************************************************************/
void CRBDcmDoc::Ostu(void)
{
	if (m_showImage.empty())
	{
		AfxMessageBox("没有处理对象!");
		return;
	}
	CSegmentOperat seg;
	m_dstImage = seg.OstuSeg(m_showImage.clone()).clone();
	m_showImage = m_dstImage.clone();
}


/***************************************************************************************
Function: 均值迭代分割
Input:    void
Output:   
Description: 
Return:   void
Others:   NULL
***************************************************************************************/
void CRBDcmDoc::Mean(void)
{
	CSegmentOperat seg;
	m_dstImage = seg.IsodataSeg(m_showImage).clone();
	m_showImage = m_dstImage.clone();
}


/***************************************************************************************
Function: 矩量分割
Input:    void
Output:   
Description: 
Return:   void
Others:   NULL
***************************************************************************************/
void CRBDcmDoc::MoM(void)
{
	CSegmentOperat seg;
	m_showImage = seg.MoMSeg(m_showImage).clone();
	m_dstImage = m_showImage.clone();
}


/***************************************************************************************
Function: 文件保存
Input:    void
Output:   
Description: 保存显示的图像到文件中
Return:   void
Others:   NULL
***************************************************************************************/
void CRBDcmDoc::OnFileSave()
{
	CFileDialog dlg(FALSE);
	dlg.m_ofn.lpstrTitle = "文件保存";
	dlg.m_ofn.lpstrFilter = "Bmp Files(*.bmp)\0*.bmp\0All Files(*.*)\0*.*\0\0";
	dlg.m_ofn.lpstrDefExt = "bmp";
	if (dlg.DoModal() == IDOK)
	{
		imwrite((LPCTSTR)dlg.GetPathName(), m_showImage);
	}
}


/***************************************************************************************
Function: 显示原图像
Input:    void
Output:   
Description: 在弹出窗口中显示原图像，方便对比处理结果
Return:   void
Others:   NULL
***************************************************************************************/
void CRBDcmDoc::SrcImg(void)
{
	CSrcShowDlg dlg;
	dlg.SetImgData(m_srcImage.clone());
	dlg.DoModal();
}


/***************************************************************************************
Function: 分割肺实质区
Input:    void
Output:   
Description: 在弹出窗口中显示原图像，方便对比处理结果
Return:   void
Others:   NULL
***************************************************************************************/
void CRBDcmDoc::SegLung(void)
{
	//1、使用阈值分割（寻求最优阈值）得到二值图像
	//2、寻找肺实质区轮廓,并填充区域
	//3、模板图像与原图像进行点乘运算
	CSegmentOperat seg;
	Mat ar = seg.GetLungArea(m_showImage.clone()).clone();
	bitwise_and(ar, m_srcImage, m_dstImage);
	//m_dstImage = ar.clone();
	m_showImage = m_dstImage.clone();
	/*
	RNG rng(12345);
	Mat ar = seg.GetLungArea(m_showImage.clone()).clone();
	//bitwise_and(ar, m_srcImage, m_dstImage);
	//m_showImage = m_dstImage.clone();
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;
	findContours(ar, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );
	/// Find the convex hull object for each contour
	vector<vector<Point> > hull(contours.size());
	for( int i = 0; i < contours.size(); i++ )
	{ convexHull( Mat(contours[i]), hull[i], false ); }
	/// Draw contours + hull results
	Mat drawing = Mat::zeros( ar.size(), CV_8UC3 );
	for( int i = 0; i< contours.size(); i++ )
	{
		Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
		drawContours( drawing, contours, i,  color, 1, 8, vector<Vec4i>(), 0, Point() );
		drawContours( drawing, hull, i,  color, 1, 8, vector<Vec4i>(), 0, Point() );
	}
	char* source_window = "Source";
	//namedWindow( source_window, CV_WINDOW_AUTOSIZE );
	//imshow( source_window, drawing );
	m_showImage = drawing.clone();
	*/
	//m_showImage = drawing.clone();
}


/***************************************************************************************
Function: 设置图像信息是否显示
Input:    void
Output:   
Description: 
Return:   void
Others:   NULL
***************************************************************************************/
void CRBDcmDoc::OnImginfodis()
{
	if (m_ls.GetSize() < 1)
		return;
	sOneImgInfo info = m_ls[1]->GetLayerInfoData();
	info.isDisplay = (info.isDisplay == FALSE);
	m_ls[1]->SetLayerInfoData(info);
	//CRBDcmView *pView = (CRBDcmView*)(((CMainFrame*)AfxGetMainWnd())->m_splt.GetPane(0, 1));
	((CMainFrame*)AfxGetMainWnd())->m_pRBView->Invalidate(FALSE);
}


/***************************************************************************************
Function: 菜单栏状态更新
Input:    void
Output:   
Description: 
Return:   void
Others:   NULL
***************************************************************************************/
void CRBDcmDoc::OnUpdateImginfodis(CCmdUI *pCmdUI)
{
	if (m_ls.GetSize() < 1)
		return;
	sOneImgInfo info = m_ls[1]->GetLayerInfoData();
	pCmdUI->SetCheck(info.isDisplay == TRUE);
}


/***************************************************************************************
Function: 设置显示区图像层的数据
Input:    void
Output:   
Description: 
Return:   void
Others:   NULL
***************************************************************************************/
void CRBDcmDoc::ShowImg(void)
{
	m_ls[0]->SetLayerImgData(m_showImage);
	m_ls[1]->SetLayerInfoData(m_matimg.GetCurrentMatImg().info);
}


/***************************************************************************************
Function: 区域填充
Input:    void
Output:   
Description: 
Return:   void
Others:   NULL
***************************************************************************************/
void CRBDcmDoc::FillHoles(Mat img)
{
	CSegmentOperat seg;
	m_showImage = seg.GetLungArea(img.clone()).clone();
	m_dstImage = m_showImage.clone();
}


/***************************************************************************************
Function: 将数字设置到状态栏显示
Input:    void
Output:   
Description: 
Return:   void
Others:   NULL
***************************************************************************************/
void CRBDcmDoc::AddTimeToStatusBar(int time)
{
	CString str;
	str.Format("用时:%dms", time);
	SetStatusBar(str);
}


/***************************************************************************************
Function: 将数字设置到状态栏显示
Input:    void
Output:   
Description: 
Return:   void
Others:   NULL
***************************************************************************************/
void CRBDcmDoc::SetStatusBar(CString str)
{
	CMainFrame* pFrm = (CMainFrame *)AfxGetMainWnd();
	(pFrm->GetStatusBar())->SetPaneText(2, str);
}


/***************************************************************************************
Function: 打开摄像头
Input:    void
Output:   
Description: 
Return:   void
Others:   NULL
***************************************************************************************/
void CRBDcmDoc::Video(void)
{
}


/***************************************************************************************
Function: 播放序列
Input:    void
Output:   
Description:  使用单例设计模式
Return:   void
Others:   NULL
***************************************************************************************/
void CRBDcmDoc::PlaySerile(void)
{
	if (m_PlayDlg != NULL)			//若对象已经存在内存中，则显示就可以了，避免创建多个对象
	{
		m_PlayDlg->SetImgSerial(m_matimg.GetBegin(), m_matimg.GetImageNum());
		m_PlayDlg->ShowWindow(SW_NORMAL);
		return;
	}
	m_PlayDlg = new CPlaySeriesDlg();
	CRBDcmView *pView = ((CMainFrame*)AfxGetMainWnd())->m_pRBView;
	m_PlayDlg->Create(MAKEINTRESOURCE(IDD_PLAY_SERI), pView);
	m_PlayDlg->SetImgSerial(m_matimg.GetBegin(), m_matimg.GetImageNum());
	m_PlayDlg->ShowWindow(SW_NORMAL);
}


/***************************************************************************************
Function: 播放序列
Input:    void
Output:   
Description: 
Return:   void
Others:   NULL
***************************************************************************************/
DWORD WINAPI CRBDcmDoc::PlaySerileProc(LPVOID lpParameter)
{
	CRBDcmDoc* pDoc = (CRBDcmDoc*)lpParameter;
	int num = pDoc->m_matimg.GetImageNum();
	int index = pDoc->m_matimg.GetCurrentNum();
	CMainFrame *pMain = (CMainFrame *)AfxGetApp()->m_pMainWnd;
	CRBDcmView *pRDBView = (CRBDcmView *)pMain->GetActiveView();
	//COprtView *pOpt = (COprtView*)(((CMainFrame*)AfxGetMainWnd())->m_splt.GetPane(0, 0));
	while(num > index)
	{
		pDoc->m_showImage = pDoc->m_matimg.NextMatImg().pixle;
		pDoc->ShowImg();
		Sleep(1000);
		//SendMessage();
		//pRDBView->ReDraw();
		//pOpt->SetCurrent(pDoc->m_matimg.GetCurrentNum());
		index = pDoc->m_matimg.GetCurrentNum();
	}
	return 0;
}


/***************************************************************************************
Function: 上一张图像
Input:    void
Output:   
Description: 
Return:   void
Others:   NULL
***************************************************************************************/
void CRBDcmDoc::Last(void)
{
	m_srcImage = m_matimg.LastMatImg().pixle.clone();
	m_showImage = m_srcImage.clone();
	ShowImg();
	//COprtView *pOpt = (COprtView*)(((CMainFrame*)AfxGetMainWnd())->m_splt.GetPane(0, 0));
	((CMainFrame*)AfxGetMainWnd())->m_pOpt->SetCurrent(m_matimg.GetCurrentNum());
}


/***************************************************************************************
Function: 下一张图像
Input:    void
Output:   
Description: 
Return:   void
Others:   NULL
***************************************************************************************/
void CRBDcmDoc::Next(void)
{
	//MatGroup mg = m_matimg.NextMatImg();
	//Mat rt = Mat(mg.pixle.cols, mg.pixle.cols, CV_8UC3);;
	//if (mg.matNodule.data != NULL)
	//{
	//	cv::add(mg.pixle, mg.matNodule, rt);
	//}
	//else
	//    rt = mg.pixle.clone();
	//cv::subtract(mg.pixle, mg.matNodule, rt);
	m_srcImage = m_matimg.NextMatImg().pixle.clone();
	//m_srcImage = rt.clone();
	m_showImage = m_srcImage.clone();
	ShowImg();

	//COprtView *pOpt = (COprtView*)(((CMainFrame*)AfxGetMainWnd())->m_splt.GetPane(0, 0));
	((CMainFrame*)AfxGetMainWnd())->m_pOpt->SetCurrent(m_matimg.GetCurrentNum());
}


/***************************************************************************************
Function: 销毁此类在堆上创建的对象
Input:    void
Output:   
Description: 
Return:		void
Others:		NULL
***************************************************************************************/
void CRBDcmDoc::DestroyObject(void)
{
	delete m_PlayDlg;
}


/***************************************************************************************
Function: 加载XML文档
Input:    void
Output:   
Description: 
Return:		void
Others:		NULL
***************************************************************************************/
void CRBDcmDoc::LoadXml(void)
{
	CFileDialog dlg(TRUE);							//文件打开对话框
	CFileFind ff;									//文件查找类
	CStringArray xmlPathStr;						//xml文件路径+文件名  xml文件
	dlg.m_ofn.lpstrTitle = "打开Xml文件";			//指定打开文件对话框名称
	dlg.m_ofn.lpstrFilter = "xml Files(*.xml)\0*.xml\0All Files(*.*)\0*.*\0\0";  //文件过滤器

	if (FALSE == dlg.DoModal())						//弹出文件打开对话框，选择取消则直接返回
		return;

	CString strFileName = dlg.GetPathName();		//获取文件路径+文件名
	m_matimg.LoadXml(strFileName);
}


