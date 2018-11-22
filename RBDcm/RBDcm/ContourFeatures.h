#pragma once

//灰度共生矩阵方向
const int HORIZON  = 1;
const int VERTICAL = 2;
const int ANGLE45  = 4;
const int ANGLE135 = 8;


//单个结节相关的特征信息
class CContourFeatures
{
public:
	CContourFeatures(void);					
	~CContourFeatures(void);
public:
	double m_fGrayMean;						//灰度均值
	double m_fVariance;						//灰度方差
	double m_fArea;							//面积
	double m_fCircumference;				//周长
	float  m_fDiameter;						//直径
	double m_fSameCircle;					//圆似度
	double m_fFlaten;						//扁度
	double m_fCompactedness;				//紧凑性
	double m_fConcavity;					//凹形率
	double m_fSameRect;						//矩形度
	double m_fEnergy;						//能量
	double m_fIdm;							//逆差矩
	double m_fCon;							//对比度
	double m_fEntropy;						//熵
	cv::Point2f m_ptPosition;				//位置
public:
	cv::Mat calGLCM(cv::Mat src, int mode, int step = 2);
	void CalTextTure(cv::Mat src);			//计算纹理四个特征
};

