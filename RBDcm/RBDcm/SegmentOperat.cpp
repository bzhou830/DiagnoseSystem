#include "StdAfx.h"
#include "SegmentOperat.h"

#include <algorithm>

using namespace std;
using namespace cv;    //for mat operator

CSegmentOperat::CSegmentOperat(void)
{
}


CSegmentOperat::~CSegmentOperat(void)
{
}


/***************************************************************************************
Function: 均值阈值迭代分割算法
Input:    Mat 待分割的原图像  n初始阈值
Output:   分割后图像
Description: 
Return:   Mat
Others:   NULL
***************************************************************************************/
Mat CSegmentOperat::IsodataSeg(Mat src, int n)
{
	int threshold = 0;							//历史阈值
	int MeansO = 0;								//前景灰度均值
	int nObject = 0;							//实质像素点个数
	int MeansB = 0;								//背景灰度均值
	int nBack = 0;								//背景像素点个数
	int nCol = src.cols * src.channels();		//每行的像素个数
	Mat dst;									//分割后图像
	while (abs(threshold - n) > 10)				//迭代停止条件
	{
		threshold = n;
		for (int i=0; i<src.rows; ++i)
		{
			uchar* pData = src.ptr<uchar>(i);
			for (int j=0; j<nCol; ++j)
			{
				//CT图像无效黑色区域过多，去掉多余的像素，避免对计算带来干扰
				if (pData[j] == 0)			//去掉黑色的点再做计算
				{
					++j;
					continue;
				}
					
				if (pData[j] < threshold)   //背景
				{
					++nBack;
					MeansB += pData[j];
				}
				else						//物体
				{
					++nObject;
					MeansO += pData[j];
				}
			}
		}
		n = (MeansB/nBack + MeansO/nObject) / 2;
	}
	cv::threshold(src, dst, n, 255, 0);    //进行阈值分割
	return dst.clone();
}


/***************************************************************************************
Function: 均值阈值迭代分割算法
Input:    Mat 待分割的原图像  n初始阈值
Output:   分割后图像
Description: 
Return:   Mat
Others:   NULL
***************************************************************************************/
Mat CSegmentOperat::OstuSeg(Mat src)
{
	int tbHist[256] = {0};						//直方图数组
	double average = 0.0;						//平均像素值
	double cov = 0.0;							//方差
	double maxcov = 0.0;						//方差最大值
	int index = 0;								//分割像素值
	Mat dst;
	int nCol = src.cols * src.channels();		//每行的像素个数
	for (int i = 0; i < src.rows; i++)
	{
		uchar* pData = src.ptr<uchar>(i);
		for (int j = 0; j < nCol; ++j)
		{
			if (pData[j] == 0)					//排除掉黑色的像素点
			{
				++j;
				continue;
			}
			tbHist[pData[j]] += 1;
		}
	}

	int sum = 0;
	for (int i=1; i<256; ++i)					//排除黑色像素点
		sum += tbHist[i];

	double w0=0.0, w1=0.0, u0=0.0, u1=0.0;
	int count0 = 0;
	for (int i = 0; i < 255; ++i)
	{
		u0 = 0;
		count0 = 0;
		for (int j=0; j<=i; ++j)
		{
			u0 += j*tbHist[j];
			count0 += tbHist[j];
		}
		u0 = u0/count0;
		w0 = (float)count0/sum;

		u1 = 0;
		for (int j=i+1; j<256; ++j)
			u1 += j*tbHist[j];

		u1 = u1/(sum - count0);
		w1 = 1 - w0;
		cov = w0*w1*(u1-u0)*(u1-u0);
		if (cov > maxcov)
		{
			maxcov = cov;
			index = i;
		}
	}
	cv::threshold(src, dst, index, 255, 0);    //进行阈值分割
	return dst.clone();
}


/***************************************************************************************
Function: 最大熵分割算法
Input:    Mat 待分割的原图像
Output:   分割后图像
Description: 
Return:   Mat
Others:   NULL
***************************************************************************************/
Mat CSegmentOperat::EntropySeg(Mat src)
{
	int tbHist[256] = {0};					//每个像素值个数
	int index = 0;							//最大熵对应的灰度
	double Property = 0.0;					//像素所占概率
	double maxEntropy = -1.0;				//最大熵
	double frontEntropy = 0.0;				//前景熵
	double backEntropy = 0.0;				//背景熵
	int TotalPixel = 0;						//纳入计算的总像素数
	int nCol = src.cols * src.channels();	//每行的像素个数
	for (int i = 0; i < src.rows; i++)
	{
		uchar* pData = src.ptr<uchar>(i);
		for (int j = 0; j < nCol; ++j)
		{
			if (pData[j] == 0)				//排除掉黑色的像素点
			{
				++j;
				continue;
			}
			++TotalPixel;
			tbHist[pData[j]] += 1;
		}
	}

	for (int i = 0; i < 256; i++) 
	{
		//计算背景像素数
		double backTotal = 0;
		for (int j = 0; j < i; j++)
		{
			backTotal += tbHist[j];
		}

		//背景熵
		for (int j = 0; j < i; j++)
		{
			if (tbHist[j] != 0)
			{
				Property = tbHist[j] / backTotal;
				backEntropy += -Property * logf((float)Property);
			}
		}
		//前景熵
		for (int k = i; k < 256; k++)
		{
			if (tbHist[k] != 0)
			{
				Property = tbHist[k] / (TotalPixel - backTotal);
				frontEntropy += -Property * logf((float)Property);
			}
		}

		if (frontEntropy + backEntropy > maxEntropy)	//得到最大熵
		{
			maxEntropy = frontEntropy + backEntropy;
			index = i;
		}
		//清空本次计算熵值
		frontEntropy = 0.0;
		backEntropy = 0.0;
	}
	Mat dst;
	cv::threshold(src, dst, index, 255, 0);    //进行阈值分割
	return dst.clone();
}


/***************************************************************************************
Function:  矩量保持法
Input:     Mat 待分割的原图像
Output:    分割后图像
Description: 
Return:    Mat
Others:    NULL
***************************************************************************************/
Mat CSegmentOperat::MoMSeg(Mat src)
{
	int tbHist[256] = {0};					//每个像素值个数
	Mat dst;
	double moment1 = 0.0;					//图像的一阶矩
	double P1 = 0.0;						//P1
	double tmpP = 0.0;
	int TotalPixel = 0;						//纳入计算的总像素数
	int index = 0;
	int nCol = src.cols * src.channels();	//每行的像素个数
	for (int i = 0; i < src.rows; i++)
	{
		uchar* pData = src.ptr<uchar>(i);
		for (int j = 0; j < nCol; ++j)
		{
			if (pData[j] == 0)				//排除掉黑色的像素点
			{
				++j;
				continue;
			}
			++TotalPixel;
			tbHist[pData[j]] += 1;			//计算每个像素的个数
		}
	}

	//计算图像的一阶矩
	for (int i=0; i< 256; ++i)
	{
		moment1 += i * tbHist[i];
	}
	moment1 = moment1 / TotalPixel;
	P1 = (255 - moment1) / 255;

	int sum = 0;
	double div = 500.0;							//差值
	for (int i=0; i<256; ++i)
	{
		sum = 0;
		for (int j=0; j<i; ++j)
		{
			sum += tbHist[j];
		}
		tmpP = (double)sum / TotalPixel;
		
		if (abs(tmpP - P1) < div)
		{
			div = abs(tmpP - P1);
			index  = i;
		}
	}
	index += 10;
	cv::threshold(src, dst, index, 255, 0);    //进行阈值分割
	Mat  et= getStructuringElement(MORPH_ELLIPSE,cv::Size(5,5)); 
	erode(dst, dst, et);
	return dst.clone();
}


/***************************************************************************************
Function:  获取肺实质所在的区域 
Input:     Mat类型 已经被二值化的图像
Output:    肺实质的所在的区域 实质区是白色，其他区域是黑色
Description: 
Return:    Mat
Others:    NULL
***************************************************************************************/
Mat CSegmentOperat::GetLungArea(Mat src)
{
	Mat  et= getStructuringElement(MORPH_ELLIPSE,cv::Size(7,7));
	morphologyEx( src, src, MORPH_OPEN, et);

	Mat tmp = src.clone();
	Mat dst(src.size(), CV_8U, cv::Scalar(255));
	std::vector<std::vector<cv::Point>> contours;    //边界点集合
	std::vector<cv::Vec4i> hierarchy;				 //边界
	findContours(src, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));

	//找到最大轮廓
	std::vector<std::vector<cv::Point>>::const_iterator itc = contours.begin();
	UINT nSize = itc->size();
	int cId = 0;
	int nMaxId = 0;

	//遍历寻找最大的的边界，最大的边界就是肺部图像
	while (itc != contours.end())
	{
		if (itc->size() > nSize)
		{
			nSize = itc->size();
			nMaxId = cId;
		}
		++itc;
		++cId;
	}

	//画出最大轮廓到result，轮廓的结果是内部为黑色，外部为白色
	drawContours(dst, contours, nMaxId, Scalar(0), CV_FILLED, 1, hierarchy);
	bitwise_not(dst, dst);
	
	cv::Mat ele(9, 9, CV_8U, cv::Scalar(1));    //9X9结构元素
	erode(dst, dst, ele);						//对图像进行腐蚀,去掉细小的边界
	cv::subtract(dst, tmp, dst);
	
	//使用圆形模板进行闭运算，对细小的缺口进行修补（吹球法）
	Mat element = getStructuringElement(MORPH_ELLIPSE,cv::Size(7,7));
	morphologyEx( dst, dst, MORPH_CLOSE, element);
	
	//再次找图像边界，对图像内部进行填充
	findContours(dst, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
	drawContours(dst, contours, -1, Scalar(255), CV_FILLED, 1, hierarchy);
	
	element = getStructuringElement(MORPH_ELLIPSE,cv::Size(5,5));    //使用5*5原型模板进行腐蚀
	erode(dst, dst, element);
	return dst.clone();
}


/***************************************************************************************
Function:  区域生长算法
Input:     src 待处理原图像 pt 初始生长点 th 生长的阈值条件
Output:    肺实质的所在的区域 实质区是白色，其他区域是黑色
Description: 生长结果区域标记为白色(255),背景色为黑色(0)
Return:    Mat
Others:    NULL
***************************************************************************************/
Mat CSegmentOperat::RegionGrow(Mat src, Point2i pt, int th)
{
	Point2i ptGrowing;								//待生长点位置
	int nGrowLable = 0;								//标记是否生长过
	int nSrcValue = 0;								//生长起点灰度值
	int nCurValue = 0;								//当前生长点灰度值
	Mat matDst = Mat::zeros(src.size(), CV_8UC1);	//创建一个空白区域，填充为黑色
	//生长方向顺序数据
	int DIR[8][2] = {{-1,-1}, {0,-1}, {1,-1}, {1,0}, {1,1}, {0,1}, {-1,1}, {-1,0}};  
	Vector<Point2i> vcGrowPt;						//生长点栈
	vcGrowPt.push_back(pt);							//将生长点压入栈中
	matDst.at<uchar>(pt.y, pt.x) = 255;				//标记生长点
	nSrcValue = src.at<uchar>(pt.y, pt.x);			//记录生长点的灰度值
	
	while (!vcGrowPt.empty())						//生长栈不为空则生长
	{
		pt = vcGrowPt.back();						//取出一个生长点
		vcGrowPt.pop_back();						

		//分别对八个方向上的点进行生长
		for (int i = 0; i<9; ++i)
		{
			ptGrowing.x = pt.x + DIR[i][0];		
			ptGrowing.y = pt.y + DIR[i][1]; 
			//检查是否是边缘点
			if (ptGrowing.x < 0 || ptGrowing.y < 0 || ptGrowing.x > (src.cols-1) || (ptGrowing.y > src.rows -1))
				continue;

			nGrowLable = matDst.at<uchar>(ptGrowing.y, ptGrowing.x);			//当前待生长点的灰度值

			if (nGrowLable == 0)												//如果标记点还没有被生长
			{
				nCurValue = src.at<uchar>(ptGrowing.y, ptGrowing.x);			
				if (abs(nSrcValue - nCurValue) < th)							//在阈值范围内则生长
				{
					matDst.at<uchar>(ptGrowing.y, ptGrowing.x) = 255;			//标记为白色
					vcGrowPt.push_back(ptGrowing);								//将下一个生长点压入栈中
				}
			}
		}
	}
	return matDst.clone();
}


/***************************************************************************************
Function:  获取肺实质形心，便于使用区域生长
Input:     src 待处理原图像（二值化后） 
		   pt1 左肺形心点   pt2 右肺形心点
Output:    void
Description: 获取的形心点可以直接交由区域生长处理，并且可以迭代至图像下一层，整体上提高了运算速度.
Return:    
Others:    NULL
***************************************************************************************/
Mat CSegmentOperat::GetObjectCenter(Mat src,vector<Point2i>& vcPoint)
{
	//对图像进行腐蚀减小轮廓
	Mat  et= getStructuringElement(MORPH_ELLIPSE,cv::Size(5,5)); 
	dilate(src, src, et);
	Mat tmp = src.clone();
	Mat dst(src.size(), CV_8U, cv::Scalar(255));	 
	std::vector<std::vector<cv::Point>> contours;    //边界点集合
	std::vector<cv::Vec4i> hierarchy;				 //边界
	findContours(src, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
	//找到最大轮廓
	std::vector<std::vector<cv::Point>>::const_iterator itc = contours.begin();
	UINT nSize = itc->size();
	int cId = 0;
	int nMaxId = 0;
	//遍历寻找最大的的边界，最大的边界就是肺部图像
	while (itc != contours.end())
	{
		if (itc->size() > nSize)
		{
			nSize = itc->size();
			nMaxId = cId;
		}
		++itc;
		++cId;
	}
	//画出最大轮廓到result，轮廓的结果是内部为黑色，外部为白色
	drawContours(dst, contours, nMaxId, Scalar(0), CV_FILLED, 1, hierarchy);
	bitwise_not(dst, dst);
	cv::Mat ele(9, 9, CV_8U, cv::Scalar(1));    //9X9结构元素
	erode(dst, dst, ele);						//对图像进行腐蚀,去掉细小的边界
	cv::subtract(dst, tmp, dst);
	
	
	//再次找图像边界，对图像内部进行填充
	findContours(dst, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
	//drawContours(dst, contours, -1, Scalar(255), CV_FILLED, 1, hierarchy);
	std::vector<std::vector<cv::Point>>::const_iterator itc2 = contours.begin();
	cId = 0;
	while (itc2 != contours.end())
	{
		double area = fabs(contourArea(*itc2));
		if (area >= 200)
			drawContours(dst, contours, cId, Scalar(255), CV_FILLED, 1, hierarchy);
		++itc2;
		++cId;
	}
	Mat element = getStructuringElement(MORPH_ELLIPSE, cv::Size(5, 5));
	erode( dst, dst, element);
	
	Mat drawing = Mat::zeros( dst.size(), CV_8UC3 );
	RNG rng(12345);
	vector<RotatedRect> minRect( contours.size() );
	vector<float> dis( contours.size() );
	int j = 0;
	while (1)
	{
		erode( dst, dst, element);
		findContours(dst, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
		for( int i = 0; i < contours.size(); i++ )
		{
			minRect[i] = minAreaRect( Mat(contours[i]) );
			dis.push_back(max<float>(minRect[i].size.height, minRect[i].size.width));
		}
		if (dis.size() <= 2 || 100 > *max_element(dis.begin(), dis.end()))     //腐蚀到最大边界都是小于100的时候返回
		{
			break;
		}
		dis.clear();
	}

	for (int i = 0; i< dis.size(); ++i)
	{
		vcPoint.push_back(minRect[i].center);
	}



	/*namedWindow("aaa", CV_WINDOW_AUTOSIZE);
	imshow("aaa", tmp);*/

	
	//for( int i = 0; i< contours.size(); i++ )
	//{
	//	Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
	//	// contour
	//	drawContours( drawing, contours, i, color, 1, 8, vector<Vec4i>(), 0, Point() );
	//	// rotated rectangle
	//	Point2f rect_points[4]; 
	//	minRect[i].points( rect_points );
	//	for( int j = 0; j < 4; j++ )
	//		line( drawing, rect_points[j], rect_points[(j+1)%4], color, 1, 8 );
	//}

	

	return dst.clone();
	/*
	//使用圆形模板进行闭运算，对细小的缺口进行修补（吹球法）
	Mat element = getStructuringElement(MORPH_ELLIPSE,cv::Size(7,7));
	morphologyEx( dst, dst, MORPH_CLOSE, element);

	//再次找图像边界，对图像内部进行填充
	findContours(dst, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
	drawContours(dst, contours, -1, Scalar(255), CV_FILLED, 1, hierarchy);

	element = getStructuringElement(MORPH_ELLIPSE,cv::Size(5,5));    //使用5*5原型模板进行腐蚀
	erode(dst, dst, element);
	*/
	//return dst.clone();
}
