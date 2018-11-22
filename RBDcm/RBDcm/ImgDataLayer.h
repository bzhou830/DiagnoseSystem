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
	Mat m_MxDisImg;		//ͼ�����ز���Ƶ�����

public:
	void SetLayerInfoData(sOneImgInfo info){};				//���̳��Ը��࣬����Ҫʹ�ø÷���
	sOneImgInfo GetLayerInfoData(){sOneImgInfo s;return s;};
	void SetLayerImgData(Mat img);
	void OnDraw(CDC* pDC, CRect rect);
	void OnLButtonDown(UINT nFlags, CPoint point);
	void OnLButtonUp(UINT nFlags, CPoint point);

public:
	inline void SetTimes(double n)				//���÷�������
	{
		m_times = n;
		m_SumTimes = m_SumTimes + m_times - 1;
	}

	inline double GetTimes(void)				//��ȡ��ǰ��������
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
	double m_times;								//ͼ��ķŴ���
	double m_SumTimes;							//���ӷŴ���

};

