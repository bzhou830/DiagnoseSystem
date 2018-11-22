#include "StdAfx.h"
#include "ContourFeatures.h"


CContourFeatures::CContourFeatures(void)
{
	m_fGrayMean = 0;
	m_fVariance = 0;
	m_fArea = 0;
	m_fCircumference = 0;
	m_fDiameter = 0;
	m_fSameCircle = 0;
	m_fFlaten = 0;
	m_fCompactedness = 0;
	m_fConcavity = 0;
	m_fSameRect = 0;
	m_fEnergy = 0;
	m_fIdm = 0;
	m_fCon = 0;
	m_fEntropy = 0;
}


CContourFeatures::~CContourFeatures(void)
{
}

//计算灰度共生矩阵
Mat CContourFeatures::calGLCM(cv::Mat src, int mode, int step)
{
	Mat Init = Mat::zeros(src.size(),CV_8UC1);					//降低灰度等级的矩阵
	Mat glcm = Mat::zeros(16, 16, CV_8UC1);						//灰度共生矩阵
	for (int i=0; i < Init.rows; ++i)							//计算获取降低等级后的灰度图
	{
		for (int j=0; j < Init.cols; ++j)
		{
			Init.at<uchar>(i, j) = src.at<uchar>(i, j) / 16;
		}
	}

	//计算灰度共生矩阵
	uchar tmp = 0;
	uchar recd = 0;
	uchar nRows = Init.rows;									//图像的行数
	uchar nCols = Init.cols;									//图像列数
	for (int i=0; i<nRows; ++i)
	{
		for (int j=0; j < nCols; ++j)
		{
			tmp = Init.at<uchar>(i, j);
			if (tmp == 0)										//去掉黑色背景
				continue;
			
			if (mode & HORIZON)									//水平方向
			{
				if (j+step < nCols && j-step >= 0)				//超出图像左/右边界
				{
					recd = Init.at<uchar>(i, j+step);			//读取邻近像素值
					glcm.at<uchar>(tmp, recd) += 1;
					recd = Init.at<uchar>(i, j-step);
					glcm.at<uchar>(tmp, recd) += 1;
				}
			}

			if (mode & VERTICAL)								//竖直方向
			{
				if (i+step < nRows && i-step >= 0)				//超出图像上/下边界
				{
					recd = Init.at<uchar>(i+step, j);			//读取邻近像素值
					glcm.at<uchar>(tmp, recd) += 1;
					recd = Init.at<uchar>(i-step, j);
					glcm.at<uchar>(tmp, recd) += 1;
				}
			}

			if (mode & ANGLE45)									//45度斜线方向
			{
				if (i+step < nRows && i-step >= 0 
					&& j+step < nCols && j-step >= 0)			//超出图像上/下边界
				{
					recd = Init.at<uchar>(i-step, j+step);		//读取邻近像素值
					glcm.at<uchar>(tmp, recd) += 1;
					recd = Init.at<uchar>(i+step, j-step);
					glcm.at<uchar>(tmp, recd) += 1;
				}
			}

			if (mode & ANGLE135)								//135度斜线方向
			{
				if (i+step < nRows && i-step >= 0 
					&& j+step < nCols && j-step >= 0)			//超出图像上/下边界
				{
					recd = Init.at<uchar>(i+step, j+step);		//读取邻近像素值
					glcm.at<uchar>(tmp, recd) += 1;
					recd = Init.at<uchar>(i-step, j-step);
					glcm.at<uchar>(tmp, recd) += 1;
				}
			}
		}
	}
	return glcm.clone();
}


void CContourFeatures::CalTextTure(cv::Mat src)
{
	Mat glcm = calGLCM(src, HORIZON|VERTICAL|ANGLE45|ANGLE135, 2).clone();
	int tmp = 0;
	for (int i=0; i < glcm.rows; ++i)
	{
		for (int j=0; j<glcm.cols; ++j)
		{
			tmp = glcm.at<uchar>(i,j);
			m_fEnergy  +=  tmp * tmp;								//能量
			m_fEntropy -= tmp * log10(static_cast<double>(tmp));	//熵
			m_fCon += (i-j)*(i-j)*tmp;								//对比度
			m_fIdm += tmp / (1 + (i-j)*(i-j));						//逆差矩
		}
	}
}