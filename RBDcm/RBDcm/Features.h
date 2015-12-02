#pragma once

class CFeatures
{
public:
	CFeatures(void);
	~CFeatures(void);
	void SetMatImgAndContours(cv::Mat src, std::vector<std::vector<cv::Point> > &contours)
	{
		m_SrcImg = src.clone();
		m_contour = &contours;
	}


private:
	cv:: Mat m_SrcImg;						//原图像
	cv::Rect m_rect;						//
	std::vector<std::vector< cv::Point > >* m_contour;	//轮廓
	double m_fGrayMean;						//灰度均值
	double m_fVariance;						//灰度方差
	double m_fArea;							//面积
	double m_fCircumference;				//周长
	double m_fDiameter;						//直径
	double m_fSameCircle;					//圆似度
	double m_fFlaten;						//扁度
	double m_fCompactedness;				//紧凑性
	double m_fConcavity;					//凹形率
	double m_fSlightness;					//细长度
	double m_fSameRect;						//矩形度
	Point m_ptPosition;						//位置

private:
	double CalcuGrayMean(); 
public:
	void CalculateFeatures();
};

