#pragma once

#include "ContourFeatures.h"		//ÿ����ͨ��������

//һ��ͼ���е����ƽ��������Ϣ
class COneImgFeatures
{
public:
	COneImgFeatures(void);
	~COneImgFeatures(void);
private:
	cv:: Mat m_SrcImg;											//ԭͼ��
	cv::Rect m_rect;											//���ƽ�������С��������
	std::vector<std::vector< cv::Point > >* m_contour;			//��������
	std::vector<CContourFeatures> m_features;					//��������

private:
	double CalcuGrayMean(cv::Rect rect, double& Area);			//����ҶȾ�ֵ�����������������������
	double CalcuVariance(cv::Rect rect, double mean);			//����Ҷȷ���

public:
	void CalculateFeatures();									//����һ��ͼ���е��������ƽ�ڵ�����
	//����������ͼ�����������ͨ����Ϣ
	void SetMatImgAndContours(cv::Mat src, std::vector<std::vector<cv::Point> > &contours)
	{
		m_SrcImg = src.clone();
		m_contour = &contours;
	}
};

