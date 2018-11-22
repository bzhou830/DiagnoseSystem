/**********************************************************************************
  Copyright @ Robin 2015-2025
  File name:  ImgLayer.cpp
  Author: Robin   Version: V1.0   Date:2015.11.05 
  Description: image pixles data layer class 
  Others:         
***********************************************************************************/
#include "StdAfx.h"
#include "ImgDataLayer.h"
#include "ImgFormat.h"


CImgDataLayer::CImgDataLayer(void)
{
	m_times = 0.0;
	m_SumTimes = 1.0;
}


CImgDataLayer::~CImgDataLayer(void)
{
}


/***************************************************************************************
Function: 图像数据层绘制
Input:    CDC* 绘制的目标DC  CRect 传入DC的相关区域
Output:  void
Description:  在传入的DC目标区域中绘制图像，根据是否需要放缩来调整图像的大小
              根据图像绘制图像的尺寸标注
			  图像放缩的实现基于cubic插值算法，调用opencv内部函数resize实现
			  tips:  图像金字塔pryUp() 和 prydown()可以考虑修改使用
Return:   void
Others:   NULL
***************************************************************************************/
void CImgDataLayer::OnDraw(CDC* pDC, CRect rect)
{
	CRect rc = rect;
	cv::Size dSize;
	if (rc.Width() < rc.Height())
		dSize.width = dSize.height = rc.Width();
	else
		dSize.width = dSize.height = rc.Height();
	
	if (m_SumTimes != 1.0)   
	{
		//使用Cubic插值算法
		resize(m_MxDisImg, m_MxDisImg, 
			Size( static_cast<int>(m_MxDisImg.cols*m_SumTimes), static_cast<int>(m_MxDisImg.rows*m_SumTimes)),
			0, 0, INTER_CUBIC);
	}
	
	//调整图像大小以适合客户区显示
	if (!m_MxDisImg.empty())
	{
		CImage img;										//MFC封装可显示接口
		Mat watch = m_MxDisImg;
		CImgFormat::Mat2CImage(m_MxDisImg, img);
		POINT pt;
		pt.x = (rc.Width() - img.GetWidth()) / 2;
		pt.y = (rc.Height() - img.GetHeight()) / 2;
		img.Draw(pDC->m_hDC, pt);

		//画标尺
		pDC->SetTextColor(RGB(255, 128, 0));
		pDC->SetBkMode(TRANSPARENT);
		CRect txtRect;
		txtRect.top = pt.y + img.GetHeight()/4*3 + 2;
		txtRect.left = pt.x + img.GetWidth() - 10;
		txtRect.right = txtRect.left + 100;
		txtRect.bottom = txtRect.top + 100;
		CFont ft;
		ft.CreateFont(12, 0, 0, 0, FW_MEDIUM, FALSE, FALSE, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS,
			CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, "宋体");			//创建字体
		pDC->SelectObject(ft);
		pDC->DrawText("200mm",&txtRect,DT_LEFT);

		CPen pen(PS_SOLID,1,RGB(255,128,0));
		pDC->SelectObject(&pen);
		int nLineLong = img.GetHeight() / 2;	//标尺的线长
		pDC->MoveTo(pt.x + img.GetWidth() + 4, pt.y + img.GetHeight()/4);					//标尺竖线
		pDC->LineTo(pt.x + img.GetWidth() + 4, pt.y + img.GetHeight()/4*3);

		pDC->MoveTo(pt.x + img.GetWidth() - 6, pt.y + img.GetHeight()/4);					//第一根刻度，线长为8
		pDC->LineTo(pt.x + img.GetWidth() + 4, pt.y + img.GetHeight()/4);
		pDC->MoveTo(pt.x + img.GetWidth() - 6, pt.y + img.GetHeight()/4*3);					//最后一根刻度，线长为8
		pDC->LineTo(pt.x + img.GetWidth() + 4, pt.y + img.GetHeight()/4*3);
		pDC->MoveTo(pt.x + img.GetWidth() - 5, pt.y + img.GetHeight()/2);					//中间刻度，线长为7
		pDC->LineTo(pt.x + img.GetWidth() + 4, pt.y + img.GetHeight()/2);

		pDC->MoveTo(pt.x + img.GetWidth() - 3, pt.y + img.GetHeight()/4 + nLineLong/4);		//四分之一处的两根刻度线
		pDC->LineTo(pt.x + img.GetWidth() + 4, pt.y + img.GetHeight()/4 + nLineLong/4);

		pDC->MoveTo(pt.x + img.GetWidth() - 3, pt.y + img.GetHeight()/4 + nLineLong/4*3);
		pDC->LineTo(pt.x + img.GetWidth() + 4, pt.y + img.GetHeight()/4 + nLineLong/4*3);

		//20根小刻度线
		int smallScale = nLineLong / 20;		
		int tmp = nLineLong/4;
		for (int j=0; j<4; ++j)
		{
			for (int i=1; i<5; ++i)
			{
				pDC->MoveTo(pt.x + img.GetWidth() + 1, pt.y + img.GetHeight()/4 + smallScale*i + tmp*j);
				pDC->LineTo(pt.x + img.GetWidth() + 4, pt.y + img.GetHeight()/4 + smallScale*i + tmp*j);
			}
		}
	}
}


/***************************************************************************************
Function: 给该类传入需要绘制的图像数据（像素数据）
Input:    传入的像素数据 (Mat数据类型)
Output:  void
Description:  NULL
Return:   void
Others:   NULL
***************************************************************************************/
void CImgDataLayer::SetLayerImgData(Mat img)
{
	m_MxDisImg = img.clone();
}


/***************************************************************************************
Function: 图像像素层数据类中鼠标左键按下响应消息
Input:    鼠标点击点和相关信息
Output:  void
Description:  NULL
Return:   void
Others:   NULL
***************************************************************************************/
void CImgDataLayer::OnLButtonDown(UINT nFlags, CPoint point)
{

}


/***************************************************************************************
Function: 图像像素层数据类中鼠标左键抬起响应消息
Input:    鼠标点击点和相关信息
Output:  void
Description:  NULL
Return:   void
Others:   NULL
***************************************************************************************/
void CImgDataLayer::OnLButtonUp(UINT nFlags, CPoint point)
{

}