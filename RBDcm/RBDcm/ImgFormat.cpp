/**********************************************************************************
  Copyright @ Robin 2015-2025
  File name:  ImgFormat.h
  Author: Robin   Version: V1.0   Date:2015.11.05 
  Description:   
  Others:         
***********************************************************************************/
#include "StdAfx.h"
#include "ImgFormat.h"


CImgFormat::CImgFormat(void)
{
}

CImgFormat::~CImgFormat(void)
{
}


/***************************************************************************************
Function: 将Mat图像数据类型转化成CImage类对象
Input:    Mat 数据  CImage 图像 适合MFC界面显示
Output:  void
Description:  NULL
Return:   void
Others:   NULL
***************************************************************************************/
void CImgFormat::Mat2CImage(Mat &mat, CImage &img)
{
	int width  = mat.cols;
	int height = mat.rows;
	int channel = mat.channels();

	img.Destroy();
	img.Create(width, height, 8*channel);

	BYTE* ps;
	BYTE* pimg = (BYTE*)img.GetBits();
	int step = img.GetPitch();

	//单通道图像有调色板
	if (1 == channel)
	{
		int MaxColors = 256;
		RGBQUAD *ColorTable = new RGBQUAD[MaxColors];
		img.GetColorTable(0, MaxColors, ColorTable);
		for (int i = 0; i < MaxColors; i++)
		{
			ColorTable[i].rgbRed = (BYTE)i;
			ColorTable[i].rgbGreen = (BYTE)i;
			ColorTable[i].rgbBlue = (BYTE)i;
		}
		img.SetColorTable(0, MaxColors, ColorTable);
		delete[] ColorTable;
	}

	for (int i = 0; i < height; i++)
	{
		ps = mat.ptr<BYTE>(i);
		for (int j = 0; j < width; j++)
		{
			if (1 == channel)
			{
				*(pimg + i*step +j) = ps[j];
			}
			else if (3 == channel)
			{
				*(pimg + i*step + j*3) = ps[j*3];
				*(pimg + i*step + j*3 + 1) = ps[j*3 + 1];
				*(pimg + i*step + j*3 + 2) = ps[j*3 + 2];
			}
		}
	}
}

