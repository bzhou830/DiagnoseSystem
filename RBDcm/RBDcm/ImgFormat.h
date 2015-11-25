/**********************************************************************************
  Copyright @ Robin 2015-2025
  File name:  ImgFormat.h
  Author: Robin   Version: V1.0   Date:2015.11.05 
  Description:   
  Others:         
***********************************************************************************/
#pragma once

#include <cv.hpp>
#include <core/core.hpp>

using namespace cv;

class CImgFormat
{
public:
	CImgFormat(void);
	~CImgFormat(void);
public:
	static void Mat2CImage(Mat &mat, CImage &img);   //将Mat数据转换成MFC适合显示的数据
	
};

