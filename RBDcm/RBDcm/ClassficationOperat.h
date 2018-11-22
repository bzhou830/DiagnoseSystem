/**********************************************************************************
  Copyright @ Robin 2015-2025
  File name:  ClassficationOperat.h
  Author: Robin   Version: V1.0   Date:2015.11.05 
  Description: this class contain all relevent classifications in the project.  
  Others:         
***********************************************************************************/

#pragma once

//分类器类
class CClassficationOperat
{
private:
	//CvSVM				m_svmClassifier;	//SVM分类器
public:
	CClassficationOperat(void);
	~CClassficationOperat(void);
	bool Training(void);
};

