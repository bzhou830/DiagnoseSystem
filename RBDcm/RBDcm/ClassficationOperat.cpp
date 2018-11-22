/**********************************************************************************
  Copyright @ Robin 2015-2025
  File name:  ClassficationOperat.cpp
  Author: Robin   Version: V1.0   Date:2015.11.05 
  Description: this class contain all relevent classifications in the project.  
  Others:         
***********************************************************************************/
#include "StdAfx.h"
#include "ClassficationOperat.h"

//构造函数
CClassficationOperat::CClassficationOperat(void)
{
}

//析构函数
CClassficationOperat::~CClassficationOperat(void)
{	
}


//分类器训练
bool CClassficationOperat::Training(void)
{
	/*
	//读取训练数据文件
	CFileDialog dlg(TRUE);
	dlg.m_ofn.lpstrTitle = "打开训练数据文件";
	dlg.m_ofn.lpstrFilter = "XML Files(*.XML)\0*.XML\0All Files(*.*)\0*.*\0\0";
	if (IDCANCEL == dlg.DoModal())						//弹出文件打开对话框，选择取消则直接返回
		return false;
	CString strFileName = dlg.GetPathName();			//获取文件路径+文件名
	string str(strFileName.GetBuffer());
	cv::FileStorage fs;
	fs.open(str,FileStorage::READ);

	//训练SVM分类器
	Mat SVM_TrainingData;
	Mat SVM_Classes;
	fs["TrainingData"] >> SVM_TrainingData;
	fs["classes"] >> SVM_Classes;
	CvSVMParams SVM_params;
	SVM_params.svm_type    = CvSVM::C_SVC;
	SVM_params.kernel_type = CvSVM::RBF;
	SVM_params.term_crit   = cvTermCriteria(CV_TERMCRIT_ITER, 100, 1e-6);
	try
	{
		m_svmClassifier.train(SVM_TrainingData, SVM_Classes, Mat(), Mat(), SVM_params);
	}
	catch(CException *e)
	{
		TCHAR szError[255];
		e->GetErrorMessage(szError, 255);
		return false;
	}
	*/
	return true;
}

