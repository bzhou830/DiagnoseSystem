#include "StdAfx.h"
#include "Features.h"


CFeatures::CFeatures(void)
{
}


CFeatures::~CFeatures(void)
{
}


/*

*/
double CFeatures::CalcuGrayMean()
{
	double fgary = 0.0;						
	int count = 0;							//记录非0灰度值的点
	for (int nX=m_rect.x; nX<(m_rect.x+m_rect.width); ++nX)
	{
		for (int nY=m_rect.y; nY<(m_rect.y+m_rect.height); ++nY)
		{
			if (m_SrcImg.at<uchar>(nY, nX) != 0)
			{
				fgary += m_SrcImg.at<uchar>(nY, nX);
				++count;
			}
		}
	}
	return fgary/count;
}


//计算特征
void CFeatures::CalculateFeatures()
{
	/*m_rect = boundingRect(*m_contour);
	m_fGrayMean = CalcuGrayMean();*/

	Mat drawing = Mat::zeros(m_SrcImg.size(), CV_8UC3 );
	for( int i = 0; i< m_contour->size(); i++ )
	{
		m_rect = boundingRect(m_contour->at(i));
		for (int nX=m_rect.x; nX<(m_rect.x+m_rect.width); ++nX)
		{
			for (int nY=m_rect.y; nY<(m_rect.y+m_rect.height); ++nY)
			{
				if (m_SrcImg.at<uchar>(nY,nX) == 0)
					continue;
				drawing.at<cv::Vec3b>(nY,nX)[0] = 0;
				drawing.at<cv::Vec3b>(nY,nX)[1] = 0;
				drawing.at<cv::Vec3b>(nY,nX)[2] = 255;
			}
		}
	}
	namedWindow("test", CV_WINDOW_AUTOSIZE);
	imshow("test",drawing);
}