#pragma once

#include "ContourFeatures.h"		//每个联通区的特征

//一张图像中的疑似结节特征信息
class COneImgFeatures
{
public:
	COneImgFeatures(void);
	~COneImgFeatures(void);
private:
	cv:: Mat m_SrcImg;											//原图像
	cv::Rect m_rect;											//疑似结节外接最小矩形区域
	std::vector<std::vector< cv::Point > >* m_contour;			//轮廓集合
	std::vector<CContourFeatures> m_features;					//特征集合

private:
	double CalcuGrayMean(cv::Rect rect, double& Area);			//计算灰度均值并带出区域像素数（面积）
	double CalcuVariance(cv::Rect rect, double mean);			//计算灰度方差

public:
	void CalculateFeatures();									//计算一张图像中的所有疑似结节的特征
	//带入待计算的图像及相关疑似联通域信息
	void SetMatImgAndContours(cv::Mat src, std::vector<std::vector<cv::Point> > &contours)
	{
		m_SrcImg = src.clone();
		m_contour = &contours;
	}
};

