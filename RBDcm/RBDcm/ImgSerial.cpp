/**********************************************************************************
Copyright @ Robin 2015-2025
  文件名:  MatImg.cpp
  作  者:  Robin
  描  述:  将标准的.dcm文件(符合dicom3.0标准)读取成OpenCV支持的Mat阵列型数据文件
          使用vector对读取后的数据进行保存。
  日  期:  2015.11.05
  修  改:  添加加载xml数据文件功能函数
***********************************************************************************/
#include "StdAfx.h"
#include "ImgSerial.h"
#include "Global.h"

//不同编码形式的dcm文件UID定义
const string losslessTransUID = "1.2.840.10008.1.2.4.70";						//JPEG Lossless,Non-Hierarchical
const string lossTransUID     = "1.2.840.10008.1.2.4.51";						//JPEG Extended
const string losslessP14      = "1.2.840.10008.1.2.4.57";						//JPEG Lossless
const string lossyP1          = "1.2.840.10008.1.2.4.50";						//JPEG Baseline
const string lossyRLE         = "1.2.840.10008.1.2.5";							//RLE Lossless

char			CImgSerial::flag		= 0x00;									//图像加载状态
HANDLE			CImgSerial::m_hMutex	= NULL;									//互斥量，用于同步
CStringArray	CImgSerial::csa[8];												//图像路径
list<sOneImg>	CImgSerial::li[8];												//图像临时存储
list<sOneImg>	CImgSerial::m_vImgSerial;										//图像存储
vector<Mat>		CImgSerial::m_aip;												//AIP后图像
vector<Mat>		CImgSerial::m_mskaip;											//AIP图像对应mask
vector<vector<Point> >	CImgSerial::cntr(5000);
vector<Vec4i>			CImgSerial::hy(20);


/*========================================================================================
* lpszPathName Dicom图像的全路径
==========================================================================================*/
CImgSerial::CImgSerial(CString lpszPathName)
{
	LoadDcm(lpszPathName);
}

/*========================================================================================
* 加载Dcm图像，供给类的外部调用接口,lpszPathName Dicom图像的全路径
==========================================================================================*/
BOOL CImgSerial::LoadDcm(CString lpszPathName)
{
	//图像进行转换, 图像加载的时候结节信息全部清零
	//将一张图的数据添加到序列中,指定迭代器itr指向序列的开头
	m_vImgSerial.push_back(Dcm2MatGroup(lpszPathName));										
	m_itr = m_vImgSerial.begin();											
	return TRUE;
}

//图像序列的排序，按照zposition参数升序进行排序
bool CompareRules(sOneImg& X, sOneImg& Y)
{
	if (X.fSliceLocation > Y.fSliceLocation)
		return true;
	else
		return false;
}

/*=====================================================================================*
线程加载函数
*======================================================================================*/
DWORD WINAPI CImgSerial::ProcLoad(LPVOID lp)
{
	int num = (int)lp;
	for (int i=0;i<csa[num].GetSize();i++)
	{
		li[num].push_back(Dcm2MatGroup(csa[num].GetAt(i)));
	}
	WaitForSingleObject(m_hMutex, INFINITE);
	flag |= (0x01 << num); 
	ReleaseMutex(m_hMutex);
	return 0;
}

/*=====================================================================================*
线程加载函数, 静态函数，创建线程，启动线程
*======================================================================================*/
BOOL CImgSerial::LoadDcms(CStringArray* paths)
{
	int cnt = paths->GetSize();

	for (int i=0; i<cnt; i++)
		csa[i%ThreadNum].Add(paths->GetAt(i));

	m_hMutex = CreateMutex(NULL, FALSE, NULL);									//创建互斥量，用于数据同步
	for (int i=0;i<ThreadNum;i++)
	{
		HANDLE han = CreateThread(NULL, 0, ProcLoad, (LPVOID)i, 0, NULL);		//创建线程用于显示读取进度
		CloseHandle(han);
	}

	while (true)																//等待线程执行完成
	{
		WaitForSingleObject(m_hMutex, INFINITE);
		if (flag == -1) break;
		ReleaseMutex(m_hMutex);
	}
	return TRUE;
}

/*=====================================================================================*
合并加载的图像资源
*======================================================================================*/
void CImgSerial::Merge()
{
	m_vImgSerial.clear();
	for (int i=0;i<ThreadNum;i++)
	{
		m_vImgSerial.splice (m_vImgSerial.begin(), li[i]);
	}
}

/***************************************************************************************
Function: Dcm图像转换成容易处理的对象（Mat图像 + 附加信息）
Input:    LPCTSTR lpszPathName dcm 图像的全路径
Output:   dcm转换后的Mat + info结构类型
Description: 使用DCMTK库函数，原DCM图像转换为8bit灰度图像会损失精度
***************************************************************************************/
sOneImg CImgSerial::Dcm2MatGroup(CString lpszPathName)
{
	sOneImg mg;																//申明一个MatGroup组
	int num = 0;
	DcmFileFormat fileformat;
	OFCondition oc = fileformat.loadFile((LPSTR)(LPCTSTR)lpszPathName);		//读取一个dcm文件
	if(!oc.good())															//判断文件是否打开成功
	{
		AfxMessageBox("打开文件失败, 不是正常的dicom文件！");						//文件打开失败后退出
		((CMainFrame*)AfxGetMainWnd())->m_Log.WriteLog("%s",
			"打开文件失败了，文件不是正常的dicom文件");
		return mg;
	}

	DcmDataset *mDataset = fileformat.getDataset();							//获取数据集
	E_TransferSyntax xfer = mDataset->getOriginalXfer();					//传输语法

	const char *pStr;
	mg.info.isDisplay = TRUE;												//显示图像信息
	mDataset->findAndGetString(DCM_PatientName, pStr);						//患者姓名
	mg.info.sName = pStr;
	mDataset->findAndGetString(DCM_PatientID, pStr);						//患者ID
	mg.info.sId = pStr;
	mDataset->findAndGetString(DCM_InstitutionName, pStr);
	mg.info.sInstitutionName  = pStr;										//患者姓名
	mDataset->findAndGetString(DCM_PatientSex, pStr);
	mg.info.sSex  = pStr;													//患者性别
	mDataset->findAndGetString(DCM_ImagePositionPatient, pStr);
	mg.info.sImgPositionPatient = pStr;										//患者图像位置
	mDataset->findAndGetString(DCM_SliceThickness, pStr);
	mg.info.slicesickness = CType::pChar2Double(pStr);						//CT层厚

	const char* thick;
	mDataset->findAndGetString(DCM_SliceLocation, thick);					//层位
	mg.fSliceLocation = CType::pChar2Double(thick);
	
	mDataset->findAndGetString(DCM_PixelSpacing, thick);					//像素中心间的间距
	CString* pPixelSpacing = CImgInfoLayer::SplitString(thick, '\\', num);
	mg.fSliceSpacing = CType::CString2Double(pPixelSpacing[0]);

	//患者拍片姿势X，Y，Z方向数据
	CString* pPosition = CImgInfoLayer::SplitString(mg.info.sImgPositionPatient,'\\', num);
	if (pPosition != NULL && num == 3)										//没有数据该数据时则写空
	{
		mg.info.nPositionX = CType::CString2Double(pPosition[0]);
		mg.info.nPositionY = CType::CString2Double(pPosition[1]);
		mg.info.nPositionZ = CType::CString2Double(pPosition[2]);
	}
	else
		mg.info.nPositionX = mg.info.nPositionY = mg.info.nPositionZ = 0;

	//获取数据集中的窗宽窗位数据
	mDataset->findAndGetString(DCM_WindowCenter, pStr);						//获取原图像中窗位
	mg.info.sWindowCenter = pStr;											//设置到输出类中窗位
	mDataset->findAndGetString(DCM_WindowWidth, pStr);						//获取原图像中窗宽
	mg.info.sWindowWidth = pStr;											//设置到输出类中窗宽

	const char* transferSyntax;
	fileformat.getMetaInfo()->findAndGetString(DCM_TransferSyntaxUID, transferSyntax);

	if(transferSyntax == losslessTransUID || transferSyntax == lossTransUID ||
		transferSyntax == losslessP14 || transferSyntax == lossyP1 || transferSyntax == lossyRLE)
	{
		AfxMessageBox("不支持图像编码格式！");
		return mg;
	}
	
	DicomImage *dcmImg = new DicomImage(lpszPathName);							//根据路径创建dicom图像

	mg.pDcmimg = dcmImg;														//保存dcm图像
	int width  = dcmImg->getWidth();											//获取图像宽度
	int height = dcmImg->getHeight();											//获取图像高度
	
	dcmImg->setWindow(-500, 1200);												//设置窗宽窗位  肺窗
	Uint16 * pixel = (Uint16 *)(dcmImg->getOutputData(16));
	mg.src_u16 = Mat(width, height, CV_16UC1, pixel).clone();
	//模板窗
	dcmImg->setWindow(-318, 203);
	char* pimod = (char *)(dcmImg->getOutputData(8));							//更新指针
	mg.masking = Mat(width, height, CV_8UC1, pimod).clone();					//肺实质模板
	//m_index = 1;																//有图像导入，则定位到第一张上
	//delete dcmImg;															//不删除图像，后续窗宽窗位需要使用
	mg.isGood = true;
	return mg;
}

/***************************************************************************************
Function: 返回序列下一张图片，迭代器向后移动
Input:    NULL
Output:   如果迭代器所指的不是序列最后一张则返回下一张，若是最后一张则返回当前张
Description: 注意序列的end()并不是序列的最后一张，而是序列最后一张的下一个元素位置，是不可访问的内存区
Return:   MatGroup
***************************************************************************************/
sOneImg CImgSerial::NextMatImg()
{
	if (++m_itr != m_vImgSerial.end())
	{
		++m_index;																//图像张数递加
		return *m_itr;
	}
	else
	{
		MessageBox(NULL,"已经是最后一张图片了！","RBDcm提示您",MB_OK);
		return *(--m_itr);
	}
}

/***************************************************************************************
Function: 返回序列上一张图片，迭代器向后移动
Input:    NULL
Output:   如果迭代器所指的不是序列第一张则返回上一张，若是第一张则返回当前张
Return:   MatGroup
***************************************************************************************/
sOneImg CImgSerial::LastMatImg()
{
	if (m_itr != m_vImgSerial.begin())
	{
		m_itr--;
		--m_index;			//图像张数递减
		return *m_itr;
	}
	else
	{
		MessageBox(NULL,"已经是第一张图片了！","RBDcm提示您",MB_OK);
		return *(m_itr);
	}
}


/***************************************************************************************
Function: 加载Xml文件
Input:    文件路径+文件名
***************************************************************************************/
BOOL CImgSerial::LoadXml(CString lpszPathName)
{
	const char* pFile = (LPSTR)(LPCTSTR)lpszPathName;						//XML文件路径
	m_vSingle.clear();														//加载前先清除信息
	TiXmlDocument doc(pFile);												//创建读取XML临时对象
	BOOL loadOK = doc.LoadFile();											//加载XML文件
	if (!loadOK)															//加载失败弹出提示并退出该函数
	{
		MessageBox(NULL, "xml文件读取失败！", "RBDcm提示您", MB_OK);
		return FALSE;
	}

	TiXmlElement* root = doc.RootElement();									//XML的根节点
	for (TiXmlNode* SpecialistItem = root->FirstChild("readingSession");    //对readingSession节点进行循环
		SpecialistItem; SpecialistItem = SpecialistItem->NextSibling("readingSession"))
	{
		TiXmlNode* unblindedReadNodule = SpecialistItem->FirstChild("unblindedReadNodule");		//结节
		TiXmlNode* nonNodule = SpecialistItem->FirstChild("nonNodule");							//非结节

		while (unblindedReadNodule)								//节点unblindedReadNodule循环
		{
			TiXmlNode* roi = unblindedReadNodule->FirstChild("roi");
			while (roi)											//节点roi循环
			{
				SingleImgNodule single;
				TiXmlNode* imageZposition = roi->FirstChild("imageZposition");	 //imageZposition信息
				const char* Zposition = imageZposition->ToElement()->GetText();
				single.ZPosition = CType::pChar2Double(Zposition);
				TiXmlNode* edgeMap = roi->FirstChild("edgeMap");
				while (edgeMap)													//edgeMap节点循环
				{
					TiXmlNode* xCoord = edgeMap->FirstChild("xCoord");
					TiXmlNode* yCoord = edgeMap->FirstChild("yCoord");
					const char* szX = xCoord->ToElement()->GetText();
					const char* szY = yCoord->ToElement()->GetText();
					NodulePoint pt;
					pt.nt = ISNODULE;
					pt.x = CType::pChar2int(szX);
					pt.y = CType::pChar2int(szY);
					single.vcNodulePoint.push_back(pt);
					edgeMap = edgeMap->NextSibling("edgeMap");
				}
				m_vSingle.push_back(single);
				roi = roi->NextSibling("roi");
			}
			unblindedReadNodule = unblindedReadNodule->NextSibling("unblindedReadNodule");
		}

		while (nonNodule)
		{
			TiXmlNode* roi = nonNodule->FirstChild("roi");
			while (roi)
			{
				SingleImgNodule single;
				TiXmlNode* imageZposition = roi->FirstChild("imageZposition");		//获取到了新的节点
				const char* Zposition = imageZposition->ToElement()->GetText();		//zPosition的数值
				single.ZPosition = CType::pChar2Double(Zposition);
				TiXmlNode* edgeMap = roi->FirstChild("edgeMap");
				while (edgeMap)
				{
					TiXmlNode* xCoord = edgeMap->FirstChild("xCoord");
					TiXmlNode* yCoord = edgeMap->FirstChild("yCoord");
					const char* szX = xCoord->ToElement()->GetText();
					const char* szY = yCoord->ToElement()->GetText();
					NodulePoint pt;
					pt.nt = NONENODULE;
					pt.x = CType::pChar2int(szX);
					pt.y = CType::pChar2int(szY);
					single.vcNodulePoint.push_back(pt);
					edgeMap = edgeMap->NextSibling("edgeMap");
				}
				//存放到序列中
				m_vSingle.push_back(single);
				roi = roi->NextSibling("roi");
			}
			nonNodule = nonNodule->NextSibling("nonNodule");
		}
	}

	//解析完成的信息与原数据进行关联
	list<sOneImg>::iterator itr = m_vImgSerial.begin();
	int count = 1;
	for (;itr != m_vImgSerial.end(); ++count)
	{
		double dbTmp = itr->info.nPositionZ;
		list<SingleImgNodule>::iterator itrSg = m_vSingle.begin();			//xml解析信息
 		for (; itrSg != m_vSingle.end(); ++itrSg)							//在解析的表中查找匹配项，然后导入
		{
			double tmp = itrSg->ZPosition;
			if (fabs(tmp - dbTmp) > 0.1)
			{
				itr->pSingleNodules = NULL;									//没有相关信息时则清空指针
				++count;
				continue;
			}
			itr->pSingleNodules = &(*itrSg);								//如果写入了结节信息，直接结束本轮查询
			break;
		}
		++itr;
	}
	return TRUE;
}



/*===========================================================================*
* 分割图像序列，得到肺实质蒙版
*============================================================================*/
int CImgSerial::SegSeries()
{
	list<sOneImg>::iterator itr = m_vImgSerial.begin();							//
	clock_t start, finish;														//
	start = clock();
	cv::Rect rect;
	Mat mt = getStructuringElement(MORPH_ELLIPSE, Size(7, 7), Point(3, 3));		//7*7圆形模板
	Mat mmt = getStructuringElement(MORPH_ELLIPSE, Size(3, 3), Point(1, 1));	//3*3圆形模板
	Mat msk;																		
	double sum = (double)m_vImgSerial.size();
	for (int i=1; itr != m_vImgSerial.end(); ++itr, ++i)						//针对每一帧图像进行分割
	{
		cntr.clear(); hy.clear();												//清除边界
		msk = itr->masking.clone();
		bitwise_and(Global::maskcicle, msk, msk);
		floodFill(msk, cv::Point(0, 0), Scalar(255), &rect, 5, 5);
		threshold(msk, msk, 254, 255, 0);
		morphologyEx(msk, msk, MORPH_CLOSE, mt);								//闭运算，去掉CT床
		bitwise_not(msk, msk);
		findContours(msk, cntr, hy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);

		Mat tmp = Mat::zeros(msk.size(), CV_8UC1);
		if(!cntr.empty() && !hy.empty())
		{
			for (unsigned int idx = 0; idx < cntr.size(); idx++)
			{
				if(contourArea(cntr[idx]) < Global::LUNGAREA) continue;			//基于面积筛选
				drawContours(tmp, cntr, idx, Scalar(255), CV_FILLED, 8, hy);
			}
		}
		erode(tmp, tmp, mmt);
		
		morphologyEx(tmp, tmp, MORPH_OPEN, mt);
		/*
		//2016.12.2采用分段修补的方式
		if ((i/sum < 0.1) || (i/sum > 0.9))
			;
		else if ((i/sum < 0.2) || (i/sum > 0.75))									//肺上页和肺下页
			tmp = ConcavityRepair(tmp, 20, 5);
		else
			tmp = ConcavityRepair(tmp);												//边界修补
		*/
		itr->masking = tmp;
	}
	finish = clock();
	return (finish - start);
}

/*========================================================================================
* 获取矢状面
==========================================================================================*/
Mat CImgSerial::GetSagittal(int col, int height)
{
	list<sOneImg>::iterator itr = m_vImgSerial.begin();
	Mat dst;
	for (; itr != m_vImgSerial.end(); ++itr)
	{
		Mat tmp = Global::Mask16(itr->src_u16, itr->masking).clone();
		dst.push_back(tmp.rowRange(col, col+1));
	}
	Mat rt;
	cv::resize(dst,rt,cv::Size(dst.cols, height),INTER_CUBIC);
	return rt.clone();
}


/*========================================================================================
* 获取冠状面
==========================================================================================*/
Mat CImgSerial::GetCoronal(int row, int height)
{
	list<sOneImg>::iterator itr = m_vImgSerial.begin();
	Mat dst;
	for (; itr != m_vImgSerial.end(); ++itr)
	{
		Mat tmp = Global::Mask16(itr->src_u16, itr->masking).clone();
		tmp = tmp.colRange(row, row + 1);
		tmp = tmp.t();
		dst.push_back(tmp);
	}
	Mat rt;
	cv::resize(dst, rt, cv::Size(dst.cols, height), INTER_CUBIC);
	return rt.clone();
}


//按照图像的切片位置排序
void CImgSerial::SortImg(void)
{
	m_vImgSerial.sort(CompareRules);			//传入排序规则函数
	m_itr = m_vImgSerial.begin();				//迭代器在排序后失效，重新获取
}


//对整个序列图像进行分析、划分结节点
void CImgSerial::AnalyseImg(void)
{
	list<sOneImg>::iterator ir = m_vImgSerial.begin();
	vector<vector<Point> > contours;
	//Mat tmp;
	for (; ir != m_vImgSerial.end(); ++ir)													//对序列中的每一副图像
	{
		cv::findContours(ir->matSel, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
		for (unsigned int i = 0; i<contours.size(); ++i)
		{
			//遍历一个结节中的所有像素
			cv::Rect rect = boundingRect(contours.at(i));									//外接矩形区域
			for(int nx = rect.x; nx < (rect.x + rect.width); ++nx)
			{
				for (int ny = rect.y; ny < (rect.y + rect.height); ++ny)
				{
					if (ir->matSel.at<uchar>(ny, nx) == 0)
						continue;
					list<sOneImg>::iterator itr = ir;
					itr++;
					while(itr != m_vImgSerial.end() && itr->matSel.at<uchar>(ny, nx) == 255)
					{
						cv::Rect rt;
						Mat img = itr->matSel;
						floodFill(itr->matSel, Point(nx, ny), 0, &rt, 5, 5);				//漫水填充
						floodFill(itr->masking, Point(nx, ny), 0, &rt, 5, 5);				//在原图上区域生长
						itr++;
					}
				}
			}
		}
		contours.clear();
	}
}


void CImgSerial::DealContours(void)
{
}


/*========================================================================================
* 计算两点间的几何距离
==========================================================================================*/
int dist(const Point& x, const Point& y)
{
	return (x.x - y.x) * (x.x - y.x) + (x.y - y.y) * (x.y - y.y);
}


/*========================================================================================
* 修补图像边界, 输入二值图像
==========================================================================================*/
Mat CImgSerial::ConcavityRepair(Mat& Im, int hullgap, int distgap)
{
	vector<Vec4i> hy;															//边界层集合
	vector<vector<Point> > cntr;												//边界点
	findContours(Im, cntr, hy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0));	//查找边界
	vector<vector<Point> > hull(cntr.size());									//凸包点集合
	for (int i = 0; i < cntr.size(); i++)										//计算凸包
		convexHull(Mat(cntr[i]), hull[i], false);
	Mat draw = Mat::zeros(Im.size(), CV_8UC1);									//结果展示

	for (int i = 0; i < cntr.size(); i++)
	{
		drawContours(draw,cntr,i,Scalar(255),1,8,vector<Vec4i>(),0,Point());	//绘制原边界
		//1.利用凸多边形修补外边界
		for (int k = 0; k < hull[i].size()-1; k++)
		{
			if (abs(hull[i][k].x - hull[i][k+1].x) + abs(hull[i][k].y - hull[i][k+1].y) < hullgap)
				line(draw, hull[i][k], hull[i][k+1], Scalar(255), 1);			//画线
		}
		//2.临近点修补内边界
		for (int k = 0; k < cntr[i].size(); k++)
		{
			for (int l = k+1; l < cntr[i].size(); l++)
			{
				if (dist(cntr[i][k], cntr[i][l]) < distgap)
					line(draw, cntr[i][k], cntr[i][l], Scalar(255), 1);			//画线
			}
		}
	}
	cntr.clear(); hy.clear();													//清除边界
	findContours(draw, cntr, hy, CV_RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);		//查找边界
	drawContours(draw, cntr, -1, Scalar(255), CV_FILLED);						//填充边界
	return draw;
}


/*========================================================================================
* 序列的平均密度投影算法，原序列没5张进行一次叠加
==========================================================================================*/
void CImgSerial::ImgAIP(int num)
{
	int count = 1;	
	list<sOneImg>::iterator it = m_vImgSerial.begin();							//遍历整个序列
	Mat tp = Global::Mask(it->src_u16.clone(),it->masking).clone();
	tp /= num;
	for (it++; it != m_vImgSerial.end();it++ ,count++)
	{
		if (count % num != 0)													//还未到达num张，则叠加
			tp += Global::Mask(it->src_u16.clone(),it->masking).clone()/num;
		else
		{
			m_aip.push_back(tp);												//到达num张讲图像进行保存
			tp = Global::Mask(it->src_u16.clone(),it->masking).clone();
			tp /= num;
		}
	}
	if (count%num != 0)
	{
		tp = tp * num / (count % num);
		m_aip.push_back(tp);
	}
}

/*========================================================================================
* 序列的平均密度投影的模板
==========================================================================================*/
void CImgSerial::mskAIP(int num)
{
	int count = 1;	
	list<sOneImg>::iterator it = m_vImgSerial.begin();							//遍历整个序列
	Mat tp = it->masking.clone();
	tp /= num;
	for (it++; it != m_vImgSerial.end();it++ ,count++)
	{
		if (count % num != 0)													//还未到达五张，则叠加
			bitwise_or(tp, it->masking, tp);
		else
		{
			m_mskaip.push_back(tp);												//到达num张讲图像进行保存
			tp = it->masking.clone();
		}
	}
	if (count%num != 0)
	{
		m_mskaip.push_back(tp);
	}
}

