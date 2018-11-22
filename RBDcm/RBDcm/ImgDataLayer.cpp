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
Function: ͼ�����ݲ����
Input:    CDC* ���Ƶ�Ŀ��DC  CRect ����DC���������
Output:  void
Description:  �ڴ����DCĿ�������л���ͼ�񣬸����Ƿ���Ҫ����������ͼ��Ĵ�С
              ����ͼ�����ͼ��ĳߴ��ע
			  ͼ�������ʵ�ֻ���cubic��ֵ�㷨������opencv�ڲ�����resizeʵ��
			  tips:  ͼ�������pryUp() �� prydown()���Կ����޸�ʹ��
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
		//ʹ��Cubic��ֵ�㷨
		resize(m_MxDisImg, m_MxDisImg, 
			Size( static_cast<int>(m_MxDisImg.cols*m_SumTimes), static_cast<int>(m_MxDisImg.rows*m_SumTimes)),
			0, 0, INTER_CUBIC);
	}
	
	//����ͼ���С���ʺϿͻ�����ʾ
	if (!m_MxDisImg.empty())
	{
		CImage img;										//MFC��װ����ʾ�ӿ�
		Mat watch = m_MxDisImg;
		CImgFormat::Mat2CImage(m_MxDisImg, img);
		POINT pt;
		pt.x = (rc.Width() - img.GetWidth()) / 2;
		pt.y = (rc.Height() - img.GetHeight()) / 2;
		img.Draw(pDC->m_hDC, pt);

		//�����
		pDC->SetTextColor(RGB(255, 128, 0));
		pDC->SetBkMode(TRANSPARENT);
		CRect txtRect;
		txtRect.top = pt.y + img.GetHeight()/4*3 + 2;
		txtRect.left = pt.x + img.GetWidth() - 10;
		txtRect.right = txtRect.left + 100;
		txtRect.bottom = txtRect.top + 100;
		CFont ft;
		ft.CreateFont(12, 0, 0, 0, FW_MEDIUM, FALSE, FALSE, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS,
			CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, "����");			//��������
		pDC->SelectObject(ft);
		pDC->DrawText("200mm",&txtRect,DT_LEFT);

		CPen pen(PS_SOLID,1,RGB(255,128,0));
		pDC->SelectObject(&pen);
		int nLineLong = img.GetHeight() / 2;	//��ߵ��߳�
		pDC->MoveTo(pt.x + img.GetWidth() + 4, pt.y + img.GetHeight()/4);					//�������
		pDC->LineTo(pt.x + img.GetWidth() + 4, pt.y + img.GetHeight()/4*3);

		pDC->MoveTo(pt.x + img.GetWidth() - 6, pt.y + img.GetHeight()/4);					//��һ���̶ȣ��߳�Ϊ8
		pDC->LineTo(pt.x + img.GetWidth() + 4, pt.y + img.GetHeight()/4);
		pDC->MoveTo(pt.x + img.GetWidth() - 6, pt.y + img.GetHeight()/4*3);					//���һ���̶ȣ��߳�Ϊ8
		pDC->LineTo(pt.x + img.GetWidth() + 4, pt.y + img.GetHeight()/4*3);
		pDC->MoveTo(pt.x + img.GetWidth() - 5, pt.y + img.GetHeight()/2);					//�м�̶ȣ��߳�Ϊ7
		pDC->LineTo(pt.x + img.GetWidth() + 4, pt.y + img.GetHeight()/2);

		pDC->MoveTo(pt.x + img.GetWidth() - 3, pt.y + img.GetHeight()/4 + nLineLong/4);		//�ķ�֮һ���������̶���
		pDC->LineTo(pt.x + img.GetWidth() + 4, pt.y + img.GetHeight()/4 + nLineLong/4);

		pDC->MoveTo(pt.x + img.GetWidth() - 3, pt.y + img.GetHeight()/4 + nLineLong/4*3);
		pDC->LineTo(pt.x + img.GetWidth() + 4, pt.y + img.GetHeight()/4 + nLineLong/4*3);

		//20��С�̶���
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
Function: �����ഫ����Ҫ���Ƶ�ͼ�����ݣ��������ݣ�
Input:    ������������� (Mat��������)
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
Function: ͼ�����ز���������������������Ӧ��Ϣ
Input:    �������������Ϣ
Output:  void
Description:  NULL
Return:   void
Others:   NULL
***************************************************************************************/
void CImgDataLayer::OnLButtonDown(UINT nFlags, CPoint point)
{

}


/***************************************************************************************
Function: ͼ�����ز���������������̧����Ӧ��Ϣ
Input:    �������������Ϣ
Output:  void
Description:  NULL
Return:   void
Others:   NULL
***************************************************************************************/
void CImgDataLayer::OnLButtonUp(UINT nFlags, CPoint point)
{

}