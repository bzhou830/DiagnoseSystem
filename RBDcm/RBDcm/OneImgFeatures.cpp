#include "StdAfx.h"
#include "OneImgFeatures.h"


COneImgFeatures::COneImgFeatures(void)
{
}


COneImgFeatures::~COneImgFeatures(void)
{
}


/***************************************************************************************
Function: 计算一个联通域的灰度均值
Input:    cv::Rect rect 待计算的联通域最小包络矩形
Output:   计算得到的灰度均值
Description: 
Return:   void
Others:   NULL
***************************************************************************************/
double COneImgFeatures::CalcuGrayMean(cv::Rect rect, double& Area)
{
	double fgary = 0.0;											//灰度
	int count = 0;												//记录非0灰度值的点的个数
	for (int nX=rect.x; nX<(rect.x+rect.width); ++nX)			//遍历矩形区域内的每一个像素点	
	{
		for (int nY=rect.y; nY<(rect.y+rect.height); ++nY)
		{
			if (m_SrcImg.at<uchar>(nY, nX) == 0)				//像素点的灰度值为0，则不纳入计算
				continue;

			fgary += m_SrcImg.at<uchar>(nY, nX);				//像素点的灰度值不是0，则纳入计算
			++count;
		}
	}
	Area = count;												//带出联通域面积
	return fgary/count;											//返回灰度均值
}


/***************************************************************************************
Function: 计算一个联通域的灰度方差
Input:    cv::Rect rect 待计算的联通域最小包络矩形
		  double mean 联通域的灰度均值
Output:   得到的联通域灰度方差
Description: 
Return:   void
Others:   NULL
***************************************************************************************/
double COneImgFeatures::CalcuVariance(cv::Rect rect, double mean)
{
	double fVariance = 0.0;										//灰度方差
	int count = 0;												//记录非0灰度值的点的个数
	for (int nX=rect.x; nX<(rect.x+rect.width); ++nX)			//遍历矩形区域内的每一个像素点	
	{
		for (int nY=rect.y; nY<(rect.y+rect.height); ++nY)
		{
			uchar tmp = m_SrcImg.at<uchar>(nY, nX);				//取出像素点灰度值
			if (tmp == 0)										//像素点的灰度值为0，则不纳入计算
				continue;
			fVariance += (mean - tmp) * (mean - tmp);		
			++count;
		}
	}
	return fVariance;											//返回灰度方差
}



/***************************************************************************************
Function: 计算一张图像中的疑似联通域的特征
Input:    void
Output:   
Description:  遍历带入的序列联通域，计算联通域的特征
Return:   void
Others:   NULL
***************************************************************************************/
void COneImgFeatures::CalculateFeatures()
{
	Mat drawing = Mat::zeros(m_SrcImg.size(), CV_8UC3 );				//创建一个三通道的RGB彩色Mat图像
	cv::Rect rect;														//联通域包络矩形
	for( int i = 0; i< m_contour->size(); i++ )							//遍历联通域序列
	{
		CContourFeatures features;												//一个联通域的特征信息
		rect = boundingRect(m_contour->at(i));									//联通域的最小包络矩形
		features.m_fGrayMean = CalcuGrayMean(rect, features.m_fArea);			//灰度均值 区域面积
		features.m_fVariance = CalcuVariance(rect, features.m_fGrayMean);		//灰度方差
		features.m_fCircumference = cv::arcLength(m_contour->at(i),true);		//周长
		//中心点，半径用其拟合最小外接圆信息来计算得出
		cv::minEnclosingCircle(m_contour->at(i), features.m_ptPosition, features.m_fDiameter);		
		features.m_fDiameter *= 2.0;
		//圆似度
		features.m_fSameCircle = 4*features.m_fArea/features.m_fDiameter/features.m_fCircumference;  
		//扁度
		cv::RotatedRect rorect = cv::minAreaRect(m_contour->at(i));
		if (rorect.size.height > rorect.size.width)
			features.m_fFlaten = rorect.size.width / rorect.size.height;
		else
			features.m_fFlaten = rorect.size.height / rorect.size.width;
		//紧凑型
		features.m_fCompactedness = 4*3.1415*features.m_fArea / features.m_fCircumference;  
		//凹形率
		features.m_fConcavity = (rorect.size.height * rorect.size.width - features.m_fArea) / features.m_fArea;
		//矩形度
		features.m_fSameRect = features.m_fArea / (rorect.size.height * rorect.size.width);

		//写入xml文件
		

		//m_features.push_back(features);
		for (int nX=rect.x; nX<(rect.x+rect.width); ++nX)			//
		{
			for (int nY=rect.y; nY<(rect.y+rect.height); ++nY)
			{
				if (m_SrcImg.at<uchar>(nY,nX) == 0)
					continue;
				//if ((features.m_ptPosition.x - nX < 1) && (features.m_ptPosition.y - nY < 1))
				//{
				//	drawing.at<cv::Vec3b>(nY,nX)[0] = 0;
				//	drawing.at<cv::Vec3b>(nY,nX)[1] = 255;
				//	drawing.at<cv::Vec3b>(nY,nX)[2] = 255;
				//}
				//else
				{
					/*CvFont font;
					cvInitFont(&font,CV_FONT_HERSHEY_COMPLEX,1.0,1.0,0,2,8);*/
					char tt[10];
					sprintf_s(tt,"%d",i);
					string abc(tt); 
					drawing.at<cv::Vec3b>(nY, nX)[0] = 0;
					drawing.at<cv::Vec3b>(nY, nX)[1] = 0;
					drawing.at<cv::Vec3b>(nY, nX)[2] = 255;
					
					putText(drawing, abc, Point(features.m_ptPosition.x, features.m_ptPosition.y),
					CV_FONT_HERSHEY_PLAIN, 0.7, Scalar(0,255,255), 1);
				}
			}
		}
	}
	namedWindow("test", CV_WINDOW_AUTOSIZE);
	imshow("test",drawing);
}