#include "stdafx.h"
#include "MainFrm.h"
#include "TransVideoDlg.h"
#include "Histogram1d.h"
#include "GMM/GMM.h"
#include "GMM/KMeans.h"
#include <fstream>

using namespace std;

/*===========================================================================*
* 验证最小包络矩形是否符合结节的大体轮廓
* 规则定为最小包络矩形的长宽均大于5个像素点
*============================================================================*/
bool CMainFrame::verifySizes(RotatedRect mr)
{
	Size2f sz  = mr.size;
	float width = sz.width;
	float height = sz.height;
	float rat = max(width, height) / min(width, height);						//宽高比
	if (width <= 5 || height <= 5)												//长宽小于2像素，认为是细小干扰
		return false;
	if (rat > 3)																//宽高严重不成比例可能是血管
		return false;
	return true;
}


/*===========================================================================*
* 验证矩形区域
* 定义规则为区域中的有效图像像素个数至少占矩形区域的一般以上
*============================================================================*/
bool CMainFrame::verifySizes(Mat m)
{
	int area = m.cols * m.rows;													//矩形区域面积
	int valid = 0;
	for (int k=0; k<m.rows; k++)												//获取肺实质像素	
		for (int n=0; n<m.cols; n++)
			if (m.at<uchar>(k,n) != 0) valid++;
	if (valid*2 < area)
		return false;
	return true;
}


/*===========================================================================*
* 提取疑似结节区域，将疑似区域存储到图像中
* 从疑似结节区域中粗选
*============================================================================*/
void CMainFrame::OnBtnExtract()
{
	int size = m_ImgSerial.m_aip.size();												//aip之后的图像
	
	for (int i = 0; i < size; i++)
	{
		Mat tmp = m_ImgSerial.m_aip[i].clone();											//查找边界会影响图像的原像素
		Mat watch = tmp.clone();
		CImgSerial::cntr.clear();
		CImgSerial::hy.clear();
		findContours(tmp, CImgSerial::cntr, CImgSerial::hy,						
			CV_RETR_EXTERNAL,
			CV_CHAIN_APPROX_NONE);
		
		vector<vector<Point> >::iterator it = CImgSerial::cntr.begin();
		vector<RotatedRect> rects;
		while (it != CImgSerial::cntr.end())											//去掉明显不符合要求的区域
		{
			RotatedRect mr = minAreaRect(Mat(*it));										//最小包络矩形
			if( !verifySizes(mr))
				it = CImgSerial::cntr.erase(it);										//不和尺寸要求，删除该边界
			else
			{
				++it;
				rects.push_back(mr);													//符合要求的边界矩形存入
			}
		}

		it = CImgSerial::cntr.begin();
		Rect rect;
		int j = 0;
		while (it != CImgSerial::cntr.end())
		{
			rect = boundingRect((Mat)*it);												//疑似结节包络矩形
			
			Mat ss = m_ImgSerial.m_aip[i](Range(rect.y, rect.y + rect.height),
				Range(rect.x, rect.x + rect.width));									//取出对应的那块图像
					
			if (!verifySizes(ss))														//从断层图像上进行验证
			{
				it++;
				continue;
			}
			

//区域中的最亮的点作为生长点，不合适！
/*
			int curVal = m_ImgSerial.m_aip[i].at<uchar>(rect.y, rect.x);
			int row = rect.x;
			int col = rect.y;

			//找到区域中最亮的点，从该点生长			
			for(int x = rect.x; x < rect.x + rect.height; x++)
			{
				for (int y=rect.y; y<rect.y + rect.width; y++)
				{
					if(m_ImgSerial.m_aip[i].at<uchar>(x,y) > curVal)
					{
						curVal = m_ImgSerial.m_aip[i].at<uchar>(x,y);
						row = x;
						col = y;
					}
				}
			}
*/

//选择区域的中点
			int row = rect.y + rect.width/2;
			int col = rect.x + rect.height/2;

			Mat cor = Global::u16Tou8(m_ImgSerial.GetCoronal(col, fSlice));				//得到矢状面图像
			Mat sag = Global::u16Tou8(m_ImgSerial.GetSagittal(row, fSlice));			//等到冠状面图像

			
			//获取高度,定位在辅助面的位置
			int height = (i+1) * m_aipFrameNum * fSlice / m_ImgSerial.GetImageNum()
				- (m_aipFrameNum / 2) * fSlice / m_ImgSerial.GetImageNum();

			//辅助面验证 (row, height) - 冠状面 (col, height) - 矢状面
			Mat mk_cor = cor(Range(height-rect.width, height+rect.width), Range(row-rect.width, row+rect.width));
			Mat mk_sag = sag(Range(height-rect.width, height+rect.width), Range(col-rect.height, col+rect.height));
			threshold(mk_cor, mk_cor, Global::ThresholdNum, 255, THRESH_TOZERO);
			threshold(mk_sag, mk_sag, Global::ThresholdNum, 255, THRESH_TOZERO);
			//int floodFill(InputOutputArray image, InputOutputArray mask, Point seedPoint,Scalar newVal, Rect* rect=0, Scalar loDiff=Scalar(), Scalar upDiff=Scalar(), int flags=4 )
			//Mat msk1 = Mat::zeros(cv::Size(cor.cols+2, cor.rows+2), CV_8UC1);
			//Mat msk2 = Mat::zeros(cv::Size(sag.cols+2, sag.rows+2), CV_8UC1);
			//cv::Rect rt;
			//floodFill(cor, msk1, cv::Point(row, height), Scalar(255), &rt, 10, 10, 8 | FLOODFILL_MASK_ONLY | 255<<8);
			//floodFill(sag, msk2, cv::Point(col, height), Scalar(255), &rt, 10, 10, 8 | FLOODFILL_MASK_ONLY | 255<<8);
			
			//cor.clone();
			//Mat msk2 = sag.clone();
			
			//floodFill(msk1, cv::Point(row, height), Scalar(255), &rt, 10, 10, 8);
			//floodFill(msk2, cv::Point(col, height), Scalar(255), &rt, 10, 10, 8);

			string filename = "E:\\training\\" + file + "\\" + std::to_string((long long)i);
			filename += "\\" + to_string((long long)j++);									//一个疑似结节，路径
								
			Global::CreateMuliteDirectory(filename.c_str());								//创建路径
			imwrite(filename + "\\A.jpg", ss);											//断层图
			imwrite(filename + "\\B.jpg", mk_cor);										//矢状面
			imwrite(filename + "\\C.jpg", mk_sag);										//冠状面
			ofstream fout(filename + "\\pos.txt");
			//fout.open(, ios::in | ios::out);
			fout<<row<<","<<col;
			fout.close();
			
			it++;
		}
	}
}


/*===========================================================================*
* 显示平均密度投影后的图
*============================================================================*/
void CMainFrame::OnBtnAIP()
{
	if(m_AipDlg != NULL)
	{
		m_AipDlg->SetImgSerial(m_ImgSerial.m_aip.begin(), m_ImgSerial.m_aip.size());
		m_AipDlg->ShowWindow(SW_NORMAL);
		return;
	}
	m_AipDlg = new CAIPDlg();
	m_AipDlg->Create(MAKEINTRESOURCE(IDD_DLG_AIP), NULL);
	CRect rct;
	m_AipDlg->GetWindowRect(&rct);
	m_AipDlg->MoveWindow(630, 62, rct.Width(),rct.Height());	//移动到右侧方便对比观察
	m_AipDlg->SetImgSerial(m_ImgSerial.m_aip.begin(), m_ImgSerial.m_aip.size());
	m_AipDlg->ShowWindow(SW_NORMAL);
}

void CMainFrame::OnBtnZoom()
{
	ShowZoom();
}

/*===========================================================================*
* 查看原图
*============================================================================*/
void CMainFrame::OnBtnSrcimg()
{
	CSrcShowDlg dlg;
	dlg.SetImgData(m_ImgSerial.GetCurrentMatImg().src_u16.clone());
	dlg.DoModal();
}

/*===========================================================================*
* 播放图像序列
*============================================================================*/
void CMainFrame::OnBtnPlayimgs()
{
	if (m_PlayDlg != NULL)											//若对象已经存在内存中，则显示就可以了，避免创建多个对象
	{
		m_PlayDlg->SetImgSerial(m_ImgSerial.GetBegin(), m_ImgSerial.GetImageNum());
		m_PlayDlg->ShowWindow(SW_NORMAL);
		return;
	}
	m_PlayDlg = new CPlaySeriesDlg();
	//CLUView *pView = ((CMainFrame*)AfxGetMainWnd())->m_pLUView;
	m_PlayDlg->Create(MAKEINTRESOURCE(IDD_PLAY_SERI), NULL);
	m_PlayDlg->SetImgSerial(m_ImgSerial.GetBegin(), m_ImgSerial.GetImageNum());
	m_PlayDlg->ShowWindow(SW_NORMAL);
}

//加载XML数据
void CMainFrame::OnBtnLoadxml()
{
	CFileDialog dlg(TRUE);														//文件打开对话框
	dlg.m_ofn.lpstrTitle = "打开Xml文件";											//指定打开文件对话框名称
	//文件过滤器
	dlg.m_ofn.lpstrFilter = "xml Files(*.xml)\0*.xml\0All Files(*.*)\0*.*\0\0";  
	if (IDCANCEL == dlg.DoModal())												//弹出文件打开对话框，选择取消则直接返回
		return;
	CString strFileName = dlg.GetPathName();									//获取文件路径+文件名
	m_ImgSerial.LoadXml(strFileName);											//加载xml文件
	sOneImg info = m_ImgSerial.GetCurrentMatImg();
	m_pInfoView->SetImgData(info);												//向信息显示窗口传入结节的信息
}

//显示上一断层
void CMainFrame::lastimg()
{
	sOneImg img = m_ImgSerial.LastMatImg();
	m_pLUView->SetImgData(img);													//设置1号窗数据
	CString str;
	str.Format("当前图像序号【%d】", m_ImgSerial.GetCurrentNum());
	SetStateText(2, str);
}

//显示下一断层
void CMainFrame::nextimg()
{
	sOneImg img = m_ImgSerial.NextMatImg();
	m_pLUView->SetImgData(img);													//设置1号窗数据
	CString str;
	str.Format("当前图像序号【%d】", m_ImgSerial.GetCurrentNum());
	SetStateText(2, str);
}

/*===========================================================================*
* 高斯混合模型, 分割疑似结节
*============================================================================*/
void CMainFrame::OnBtnGMM()
{
	Mat src;
	int size = m_ImgSerial.m_aip.size();
	double *data = new double[512 * 512 * size];								//数据
	int index = 0;
	//统计出整个序列的肺区数据
	for (int i=0;i<size;i++)
	{
		for (int m=0; m<512; m++)												//获取肺实质像素
		{	
			for (int n=0; n<512; n++)
			{
				if (m_ImgSerial.m_mskaip[i].at<uchar>(m, n) != 0)
					data[index++] = m_ImgSerial.m_aip[i].at<uchar>(m, n);
			}
		}
	}

	if (Global::isDumpCsv)														//读取配置文件设定值，判断是否导出csv文件
	{
		ofstream savefile("data.csv");
		for (int i=0;i<index-1;i++)
			savefile<<data[i]<<",";
		savefile<<data[index-1];
		savefile.close();
	}

	GMM *gmm = new GMM(1, 6);													//一维数据，2个混合模型
	gmm->Train(data, index);
	gmm->SetEndError(0.01);
	double g1[3] = {gmm->m_priors[0], gmm->m_means[0][0], gmm->m_vars[0][0]};	//三个高斯模型的参数
	double g2[3] = {gmm->m_priors[1], gmm->m_means[1][0], gmm->m_vars[1][0]};
	double g3[3] = {gmm->m_priors[2], gmm->m_means[2][0], gmm->m_vars[2][0]};
	double g4[3] = {gmm->m_priors[3], gmm->m_means[3][0], gmm->m_vars[3][0]};
	double g5[3] = {gmm->m_priors[4], gmm->m_means[4][0], gmm->m_vars[4][0]};
	double g6[3] = {gmm->m_priors[5], gmm->m_means[5][0], gmm->m_vars[5][0]};
	delete gmm;
	int th = max(max(g1[1], g2[1]), g3[1]);
	Mat mmt = getStructuringElement(MORPH_ELLIPSE, Size(3, 3), Point(1, 1));
	for (int i=0;i<size;i++)
	{
		threshold(m_ImgSerial.m_aip[i], m_ImgSerial.m_aip[i], th, 255, THRESH_TOZERO);
		//morphologyEx(m_ImgSerial.m_aip[i], m_ImgSerial.m_aip[i], MORPH_OPEN, mmt);//处理后都变成了圆形，不好
	}
	Global::ThresholdNum = th;
	/*
	for (int i=0; i<size; ++i)
	{
		GMM *gmm = new GMM(1, 2);												//一维数据，2个混合模型
		src = m_ImgSerial.m_mskaip[i];
		bitwise_and(m_ImgSerial.m_mskaip[i],m_ImgSerial.m_aip[i],m_ImgSerial.m_aip[i]);
		for (int m=0; m<512; m++)												//获取肺实质像素
		{	
			for (int n=0; n<512; n++)
			{
				if (m_ImgSerial.m_mskaip[i].at<uchar>(m,n) != 0)
					data[index++] = m_ImgSerial.m_aip[i].at<uchar>(m, n);
			}
		}

		if (index >= 500)
		{
			gmm->Train(data, index);
			double g1[3] = {gmm->m_priors[0], gmm->m_means[0][0], gmm->m_vars[0][0]};
			double g2[3] = {gmm->m_priors[1], gmm->m_means[1][0], gmm->m_vars[1][0]};
			//计算交点
			int th = (int)(gmm->m_means[0][0] + 3* sqrt(gmm->m_vars[0][0]));//Global::GuassAcross(g1, g2);
			Mat low, high;
			threshold(m_ImgSerial.m_aip[i], low, th, 255, THRESH_TOZERO); //二值化图像
			threshold(m_ImgSerial.m_aip[i], high, 130, 255, THRESH_TOZERO);
			m_ImgSerial.m_aip[i] = low;
			th = 0;
		}
		delete gmm;
		index = 0;
	}
	*/
	delete[] data;
}

void CMainFrame::OnBtnHistimg()
{
}

/*图像序列转换成视频*/
void CMainFrame::OnBtnCamera()
{
	CTransVideoDlg dlg(m_ImgSerial.m_vImgSerial);
	dlg.DoModal();
}

void CMainFrame::OnBtnEntropy()
{
	AfxMessageBox("还没有移植过来额！");
	std::ifstream file("c:\\abcd.csv", ifstream::in);	//读取csv文件
	string line, path, classlabel;
	Mat dst = Mat::zeros(40, 10, CV_32F);
	vector<int> row(12, 1);
	for (int k=0;k<28;++k)
		row.push_back(0);
	int i = 0, j = 0;
	while (getline(file, line))
	{
		stringstream liness(line);
		while (getline(liness, path, ','))
		{
			dst.at<float>(i,j) = static_cast<float>(CType::string2Double(path));
			j++;
		}
		j = 0, i++;
	}
	Mat fe(row);
	FileStorage fs("C:\\ttt.xml", FileStorage::WRITE);
	fs << "TrainingData" << dst;
	fs << "classes" << fe;
	fs.release();
}

void CMainFrame::OnBtnOtsu()
{
	CSegmentOperat seg;
	sOneImg src = m_pLUView->GetOneImg();
	cv::Mat m_dstImage = seg.OstuSeg(src.masking.clone()).clone();
	sOneImg info = m_ImgSerial.GetCurrentMatImg();
	info.masking = m_dstImage.clone();
	m_pRUView->SetImgData(info);
}



void CMainFrame::OnBtnNudoul()
{
	//(m_pMainFrm->m_pClassier)->SetImgData(img);
	sOneImg img = (m_pLDView)->GetOneImg();				//获取结节窗中显示的图像，疑似结节的灰度图像
	std::vector<std::vector<cv::Point> > contours;			//边界点集合
	std::vector<cv::Vec4i> hierarchy;						//边界

	cv::medianBlur(img.masking, img.masking,3);
	findContours(img.masking.clone(), contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
	drawContours(img.masking, contours, -1, Scalar(255), CV_FILLED, 1, hierarchy);

	sOneImg simg;	//肺实质窗口中的图像
	(m_pRUView)->GetSegRealLungs(simg);
	bitwise_and(simg.masking, img.masking, img.masking);
	COneImgFeatures features;
	features.SetMatImgAndContours(img.masking, contours);
	features.CalculateFeatures();

	//namedWindow("aaa",CV_WINDOW_AUTOSIZE);				//显示分割后的结果
	//imshow("aaa",img.pixle);
}


//状态栏矩量保持按钮响应
void CMainFrame::OnBtnMom()					//矩量
{
	sOneImg img;							//肺实质窗口中的图像
	(m_pRUView)->GetSegRealLungs(img);		//
	medianBlur(img.masking, img.masking, 3);
	(m_pLDView)->SegNodules(img);
}

//状态栏均值迭代算法响应
void CMainFrame::OnBtnMean()
{
	sOneImg img;
	(m_pRUView)->GetSegRealLungs(img);
	medianBlur(img.masking, img.masking, 3);
	(m_pLDView)->SegNodulesMean(img);
}

//保存选中图像
void CMainFrame::OnSaveimg()
{
	CFileDialog dlg(FALSE);
	dlg.m_ofn.lpstrTitle = "文件保存";
	dlg.m_ofn.lpstrFilter = "Bmp Files(*.bmp)\0*.bmp\0All Files(*.*)\0*.*\0\0";
	dlg.m_ofn.lpstrDefExt = "bmp";
	sOneImg img = m_pLUView->GetOneImg();
	if (dlg.DoModal() == IDOK)
		imwrite((LPCTSTR)dlg.GetPathName(), img.masking);
}

//对图像特征进行检测
void CMainFrame::OnBtnTest()
{
}


//数据训练
void CMainFrame::OnTraining()
{
	/*
	CFileDialog dlg(TRUE);
	dlg.m_ofn.lpstrTitle = "打开训练数据文件";
	dlg.m_ofn.lpstrFilter = "XML Files(*.XML)\0*.XML\0All Files(*.*)\0*.*\0\0";
	if (IDCANCEL == dlg.DoModal())						//弹出文件打开对话框，选择取消则直接返回
		return;
	CString strFileName = dlg.GetPathName();			//获取文件路径+文件名
	string str(strFileName.GetBuffer());
	cv::FileStorage fs;
	fs.open(str, FileStorage::READ);
	Mat SVM_TrainingData;
	Mat SVM_Classes;
	fs["TrainingData"] >> SVM_TrainingData;
	fs["classes"] >> SVM_Classes;
	CvSVMParams SVM_params;
	SVM_params.svm_type    = CvSVM::C_SVC;
	SVM_params.kernel_type = CvSVM::RBF;
	SVM_params.term_crit   = cvTermCriteria(CV_TERMCRIT_ITER, 100, 1e-6);
	try
	{
		m_svmClassifier.train(SVM_TrainingData, SVM_Classes, Mat(), Mat(), SVM_params);
	}
	catch(CException *e)
	{
		TCHAR szError[255];
		e->GetErrorMessage(szError, 255);
		CString str;
		str.Format("训练出问题了！异常原因：\r\n %s", szError);
		MessageBox(str, "RBDcm提示您", MB_OK);
		return;
	}
	MessageBox("分类器训练完成啦！！^_^", "RBDcm提示您", MB_OK);
	*/
}


