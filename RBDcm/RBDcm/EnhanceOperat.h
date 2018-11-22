/**********************************************************************************
  Copyright @ Robin 2015-2025
  File name:  EnhanceOperat.h
  Author: Robin   Version: V1.0   Date:2015.11.05 
  Description: this class contain all relevent Enhance algorithms in the project.  
  Others:         
***********************************************************************************/
#pragma once

Mat ManyLevelHessianEnhance2(cv::Mat src);

class CEnhanceOperat
{
public:
	CEnhanceOperat(void);
	~CEnhanceOperat(void);
	static Mat HessianEnhance(Mat src, int nLeverL, int nLeverR);
};

