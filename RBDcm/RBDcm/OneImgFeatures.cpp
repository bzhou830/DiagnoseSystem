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
	// 计算图像特征
}


