#include "StdAfx.h"
#include "OneImgFeatures.h"


COneImgFeatures::COneImgFeatures(void)
{
}


COneImgFeatures::~COneImgFeatures(void)
{
}


/***************************************************************************************
Function: ����һ����ͨ��ĻҶȾ�ֵ
Input:    cv::Rect rect ���������ͨ����С�������
Output:   ����õ��ĻҶȾ�ֵ
Description: 
Return:   void
Others:   NULL
***************************************************************************************/
double COneImgFeatures::CalcuGrayMean(cv::Rect rect, double& Area)
{
	double fgary = 0.0;											//�Ҷ�
	int count = 0;												//��¼��0�Ҷ�ֵ�ĵ�ĸ���
	for (int nX=rect.x; nX<(rect.x+rect.width); ++nX)			//�������������ڵ�ÿһ�����ص�	
	{
		for (int nY=rect.y; nY<(rect.y+rect.height); ++nY)
		{
			if (m_SrcImg.at<uchar>(nY, nX) == 0)				//���ص�ĻҶ�ֵΪ0�����������
				continue;

			fgary += m_SrcImg.at<uchar>(nY, nX);				//���ص�ĻҶ�ֵ����0�����������
			++count;
		}
	}
	Area = count;												//������ͨ�����
	return fgary/count;											//���ػҶȾ�ֵ
}


/***************************************************************************************
Function: ����һ����ͨ��ĻҶȷ���
Input:    cv::Rect rect ���������ͨ����С�������
		  double mean ��ͨ��ĻҶȾ�ֵ
Output:   �õ�����ͨ��Ҷȷ���
Description: 
Return:   void
Others:   NULL
***************************************************************************************/
double COneImgFeatures::CalcuVariance(cv::Rect rect, double mean)
{
	double fVariance = 0.0;										//�Ҷȷ���
	int count = 0;												//��¼��0�Ҷ�ֵ�ĵ�ĸ���
	for (int nX=rect.x; nX<(rect.x+rect.width); ++nX)			//�������������ڵ�ÿһ�����ص�	
	{
		for (int nY=rect.y; nY<(rect.y+rect.height); ++nY)
		{
			uchar tmp = m_SrcImg.at<uchar>(nY, nX);				//ȡ�����ص�Ҷ�ֵ
			if (tmp == 0)										//���ص�ĻҶ�ֵΪ0�����������
				continue;
			fVariance += (mean - tmp) * (mean - tmp);		
			++count;
		}
	}
	return fVariance;											//���ػҶȷ���
}



/***************************************************************************************
Function: ����һ��ͼ���е�������ͨ�������
Input:    void
Output:   
Description:  ���������������ͨ�򣬼�����ͨ�������
Return:   void
Others:   NULL
***************************************************************************************/
void COneImgFeatures::CalculateFeatures()
{
	// ����ͼ������
}


