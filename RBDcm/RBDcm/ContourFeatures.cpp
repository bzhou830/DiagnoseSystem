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

//����Ҷȹ�������
Mat CContourFeatures::calGLCM(cv::Mat src, int mode, int step)
{
	Mat Init = Mat::zeros(src.size(),CV_8UC1);					//���ͻҶȵȼ��ľ���
	Mat glcm = Mat::zeros(16, 16, CV_8UC1);						//�Ҷȹ�������
	for (int i=0; i < Init.rows; ++i)							//�����ȡ���͵ȼ���ĻҶ�ͼ
	{
		for (int j=0; j < Init.cols; ++j)
		{
			Init.at<uchar>(i, j) = src.at<uchar>(i, j) / 16;
		}
	}

	//����Ҷȹ�������
	uchar tmp = 0;
	uchar recd = 0;
	uchar nRows = Init.rows;									//ͼ�������
	uchar nCols = Init.cols;									//ͼ������
	for (int i=0; i<nRows; ++i)
	{
		for (int j=0; j < nCols; ++j)
		{
			tmp = Init.at<uchar>(i, j);
			if (tmp == 0)										//ȥ����ɫ����
				continue;
			
			if (mode & HORIZON)									//ˮƽ����
			{
				if (j+step < nCols && j-step >= 0)				//����ͼ����/�ұ߽�
				{
					recd = Init.at<uchar>(i, j+step);			//��ȡ�ڽ�����ֵ
					glcm.at<uchar>(tmp, recd) += 1;
					recd = Init.at<uchar>(i, j-step);
					glcm.at<uchar>(tmp, recd) += 1;
				}
			}

			if (mode & VERTICAL)								//��ֱ����
			{
				if (i+step < nRows && i-step >= 0)				//����ͼ����/�±߽�
				{
					recd = Init.at<uchar>(i+step, j);			//��ȡ�ڽ�����ֵ
					glcm.at<uchar>(tmp, recd) += 1;
					recd = Init.at<uchar>(i-step, j);
					glcm.at<uchar>(tmp, recd) += 1;
				}
			}

			if (mode & ANGLE45)									//45��б�߷���
			{
				if (i+step < nRows && i-step >= 0 
					&& j+step < nCols && j-step >= 0)			//����ͼ����/�±߽�
				{
					recd = Init.at<uchar>(i-step, j+step);		//��ȡ�ڽ�����ֵ
					glcm.at<uchar>(tmp, recd) += 1;
					recd = Init.at<uchar>(i+step, j-step);
					glcm.at<uchar>(tmp, recd) += 1;
				}
			}

			if (mode & ANGLE135)								//135��б�߷���
			{
				if (i+step < nRows && i-step >= 0 
					&& j+step < nCols && j-step >= 0)			//����ͼ����/�±߽�
				{
					recd = Init.at<uchar>(i+step, j+step);		//��ȡ�ڽ�����ֵ
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
			m_fEnergy  +=  tmp * tmp;								//����
			m_fEntropy -= tmp * log10(static_cast<double>(tmp));	//��
			m_fCon += (i-j)*(i-j)*tmp;								//�Աȶ�
			m_fIdm += tmp / (1 + (i-j)*(i-j));						//����
		}
	}
}