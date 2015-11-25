/**********************************************************************************
  Copyright @ Robin 2015-2025
  File name:  EnhanceOperat.cpp
  Author: Robin   Version: V1.0   Date:2015.11.05 
  Description: this class contain all relevent Enhance algorithms in the project.  
  Others:         
***********************************************************************************/
#include "StdAfx.h"
#include "EnhanceOperat.h"

CEnhanceOperat::CEnhanceOperat(void)
{
}

CEnhanceOperat::~CEnhanceOperat(void)
{
}

/***************************************************************************************
Function: 多尺度汉森矩阵增强
Input:    src 待处理原图像 , nLeverL 尺度左值 , nLeverR 尺度右值
Output:   
Description: 
Return:   void
Others:   NULL
***************************************************************************************/
Mat CEnhanceOperat::HessianEnhance(Mat src, int nLeverL, int nLeverR)
{
	Mat dst = Mat::zeros(src.size(), CV_8UC1);
	Mat srcClone;
	double fxx = 0.0, fyy = 0.0, fxy = 0.0;			//二阶偏导	
	double dbR = 0.0, dbQ = 0.0;					//方便计算特征值使用
	double lamda1 = 0.0, lamda2 = 0.0;              //Hessian矩阵两个特征值
	double tpla1 = 0.0, tpla2 = 0.0;				//两个特征值的中间结果
	double k = 0.0, q2 = 0.0, qr = 0.0, r1 = 0.0, r2 = 0.0;
	double rt = 0.0;								//待填充结果
	int nCol = src.cols * src.channels();			//每行的像素个数
	int sigma = nLeverL;

	while (sigma <= nLeverR)
	{
		GaussianBlur(src, srcClone, cv::Size(3, 3), sigma, sigma);
		for (int i = 1; i < srcClone.rows-1; i++)
		{
			uchar* pData = srcClone.ptr<uchar>(i);
			for (int j = 1; j < nCol-1; ++j)
			{
				fxx = static_cast<double>((pData-1)[j] + (pData+1)[j] - 2*pData[j]);				//x方向上二阶偏导
				fyy = static_cast<double>( pData[j-1] + pData[j+1] - 2*pData[j]);					//y方向上二阶偏导
				fxy = static_cast<double>((pData+1)[j+1] + pData[j] - (pData+1)[j] - pData[j+1]);	//混合二阶偏导
				dbR = static_cast<double>((fxx + fyy)/2);
				dbQ = static_cast<double>(sqrt(fxx*fyy - fxy*fxy));		//观察这里
				tpla1 = dbR + sqrt(dbR*dbR - dbQ*dbQ);					//计算两个特征值
				tpla2 = dbR - sqrt(dbR*dbR - dbQ*dbQ);					
				lamda1 = cv::max(fabs(tpla1), fabs(tpla2));				//保证特征值满足 lamda1 > lamda2
				lamda2 = cv::min(fabs(tpla1), fabs(tpla2));
				
				if (lamda1 > 0.0 && lamda2 > 0.0						//圆形结构
					&& lamda1/lamda2 < 2.0)						
				{
					rt = lamda1*lamda1 / lamda2;
					if (rt > 255)
						rt = 255;
					dst.ptr<uchar>(i)[j] = static_cast<uchar>(rt);
				}
				//else if (lamda1 > 0.0 && lamda2 < 0.01)					//类型线性结构
				//{
				//	rt = lamda1 - lamda2;
				//	dst.ptr<uchar>(i)[j] = static_cast<uchar>(rt);
				//}
			}
		}
		++sigma;
	}
	return dst;
}


/***************************************************************************************
Function: 多尺度汉森矩阵增强
Input:    src 待处理原图像 , nLeverL 尺度左值 , nLeverR 尺度右值
Output:   
Description: 
Return:   void
Others:   NULL
***************************************************************************************/
Mat ManyLevelHessianEnhance2(cv::Mat src)
{
	Mat srcClone = src.clone();
	//*dst = src->clone();
	Mat dst = Mat::zeros(src.size(), CV_8UC1);
	Mat temp;
	Mat filter = Mat::zeros(src.size(), CV_8UC1);
	int sigma = 2;
	double fxx = 0.0, fyy = 0.0, fxy = 0.0;
	double k = 0.0, q2 = 0.0, qr = 0.0, r1 = 0.0, r2 = 0.0;
	//int nCol = src.cols * src.channels();		//每行的像素个数

	while (sigma <= 10)
	{
		GaussianBlur(srcClone, temp, cv::Size(3, 3), sigma, sigma);

		for (int i = 1; i < src.rows-1; i++)
		{
			for (int j = 1; j < src.cols-1; j++)
			{
				fxx = temp.at<uchar>(i-1, j) + temp.at<uchar>(i+1,j) - 2*temp.at<uchar>(i,j);
				fyy = temp.at<uchar>(i, j-1) + temp.at<uchar>(i,j+1) - 2*temp.at<uchar>(i,j);
				fxy = temp.at<uchar>(i+1, j+1) + temp.at<uchar>(i,j) - temp.at<uchar>(i+1,j) - temp.at<uchar>(i,j+1);

				k = (fxx + fyy) / 2;
				q2 = fxx * fyy - fxy * fxy;
				qr = sqrt(double(k * k - q2));
				r1 = k + qr;
				r2 = k - qr;

				if ((r1 < 0) && (r2 < 0))
				{
					r2 = cv::max(fabs(r1), fabs(r2));//???
					r1 = cv::min(fabs(r1), fabs(r2));

					//double rb = pow(r2 / r1, 2);
					//double s = pow(r1, 2) + pow(r2, 2);

					double value = r2*r2/(r1);
					if (value > filter.at<uchar>(i,j))
					{
						filter.at<uchar>(i,j) = (uchar)value;
					}
				}
			}
		}
		sigma++;
	}

	for (int i = 0; i < src.rows; i++)
	{
		for (int j = 0; j<src.cols; j++)
		{
			if (filter.at<uchar>(i, j) == 0)
			{
				//dst.at<uchar>(i,j) = 0;
			}
			else
			{
				dst.at<uchar>(i,j) += filter.at<uchar>(i,j);
			}
		}
	}
	return dst;
}

