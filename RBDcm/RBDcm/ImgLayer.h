/**********************************************************************************
  Copyright @ Robin 2015-2025
  File name:  ImgLayer.h
  Author: Robin   Version: V1.0   Date:2015.11.05 
  Description: image pixles data layer class 
  Others:         
***********************************************************************************/
#pragma once
#include "layer.h"
#include "BaseStruct.h"

using namespace cv;

class CImgDataLayer : public CLayer
{
public:
	CImgDataLayer(void);
	~CImgDataLayer(void);

private:
	Mat m_MxDisImg;		//图像像素层绘制的数据

public:
	void SetLayerInfoData(sOneImgInfo info){};				//仅继承自父类，不需要使用该方法
	sOneImgInfo GetLayerInfoData(){sOneImgInfo s;return s;};
	void SetLayerImgData(Mat img);
	void OnDraw(CDC* pDC, CRect rect);
	void OnLButtonDown(UINT nFlags, CPoint point);
	void OnLButtonUp(UINT nFlags, CPoint point);

public:
	inline void SetTimes(double n)   //设置放缩倍数
	{
		m_times = n;
		m_SumTimes = m_SumTimes + m_times - 1;
	}

	inline double GetTimes(void)     //获取当前放缩倍数
	{
		return m_SumTimes;
	}

	inline CSize GetImgSize()
	{
		CSize sz;
		sz.cx = static_cast<LONG>(m_MxDisImg.cols * m_SumTimes);
		sz.cy = static_cast<LONG>(m_MxDisImg.rows * m_SumTimes);
		return sz;
	}

private:
	double m_times;					//图像的放大倍数
	double m_SumTimes;				//叠加放大倍数

};

