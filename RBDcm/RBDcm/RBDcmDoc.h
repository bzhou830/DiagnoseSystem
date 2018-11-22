/**********************************************************************************
  Copyright @ Robin 2015-2025
  File name:  RBDcmDoc.h
  Author: Robin   Version: V1.0   Date:2015.11.05 
  Description: Software Frame  Docment Class 
               This class contain main image algorithm  
  Others:         
***********************************************************************************/
#pragma once

#include "Layer.h"
#include "afxtempl.h"
#include <vector>
#include "ImgSerial.h"
//#include "OprtView.h"   在此包含会出错
#include "ImgDataLayer.h"
#include "PlaySeriesDlg.h"
#include "SegmentOperat.h"
#include "EnhanceOperat.h"
#include "ClassficationOperat.h"


using namespace cv;    //for mat operator
using namespace std;   //for vector


class CRBDcmDoc : public CDocument
{
public:
	//the following three Mat are used to compatiable the old version
	Mat m_srcImage;						//src image
	Mat m_dstImage;						//dst image
	Mat m_showImage;					//showing image

	CImgSerial m_matimg;                //Mat-image structure
	CArray <CLayer*, CLayer*> m_ls;     //interface layers 
	CPlaySeriesDlg* m_PlayDlg;			//Play serial dialog
protected:
	CRBDcmDoc();
	DECLARE_DYNCREATE(CRBDcmDoc)
public:
	friend DWORD WINAPI PlaySerileProc(LPVOID lpParameter);

public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

public:
	virtual ~CRBDcmDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// 用于为搜索处理程序设置搜索内容的 Helper 函数
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
public:
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);   //文件打开
	//afx_msg void OnFileSave();		//文件保存	
	void Hist(void);				//图像直方图均衡
	void Cut(void);					//阈值分割
	void Rst(void);					//图像复原
	void Mask(void);				//图像掩膜
	void Median(void);				//中值滤波
	void DrawHist(void);			//绘制图像灰度直方图
	void Entropy(void);				//熵分割
	void Ostu(void);				//OSTU分割
	void Enhance(void);				//增强
	void SrcImg(void);				//显示原图像
	void SegLung(void);				//分割肺实质
public:
	static DWORD WINAPI PlaySerileProc(LPVOID lpParameter);
	afx_msg void OnImginfodis();
	afx_msg void OnUpdateImginfodis(CCmdUI *pCmdUI);
	//afx_msg void OnSerile();			//打开图像序列
	void ShowImg(void);					//向图像层写数据
	void FillHoles(Mat img);			//区域填充
	void AddTimeToStatusBar(int time);	//向状态栏写算法处理时间
	void SetStatusBar(CString str);		//设置状态栏信息
	void Video(void);					//打开摄像头
	void PlaySerile(void);				//播放图像序列
	void Last(void);					//上一张图像
	void Next(void);					//下一张图像
	void Mean(void);					//中值滤波
	void MoM(void);						//矩方法分割
	void DestroyObject(void);
	void LoadXml(void);
};



