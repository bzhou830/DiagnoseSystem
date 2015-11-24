#pragma once
#include "Layer.h"
#include "ImgLayer.h"
#include "ImgInfo.h"
#include "RBMermoryDC.h"
#include "SegmentOperat.h"

// CSegView 视图
class CSegView : public CView
{
	DECLARE_DYNCREATE(CSegView)
public:
	sOneImg m_sOneImgShow;				//showing image
	CArray <CLayer*, CLayer*> m_ls;     //interface layers 
protected:
	CSegView();							// 动态创建所使用的受保护的构造函数
	virtual ~CSegView();

public:
	virtual void OnDraw(CDC* pDC);      // 重写以绘制该视图
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
	void SetImgData(sOneImg src);
	void SegRealLung(cv::Mat src);
	void GetSegRealLungs(sOneImg& seg);
};


