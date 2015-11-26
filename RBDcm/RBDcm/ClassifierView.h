#pragma once
#include "Layer.h"
#include "RBMermoryDC.h"
#include "SegmentOperat.h"

// CClassifierView 视图

class CClassifierView : public CView
{
	DECLARE_DYNCREATE(CClassifierView)
public:
	sOneImg m_sOneImgShow;						//showing image
	CArray <CLayer*, CLayer*> m_ls;     		//interface layers 
protected:
	CClassifierView();							// 动态创建所使用的受保护的构造函数
	virtual ~CClassifierView();

public:
	virtual void OnDraw(CDC* pDC);      		// 重写以绘制该视图
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	DECLARE_MESSAGE_MAP()
public:
	void SetImgData(sOneImg src);
	void SegNodules(sOneImg src);
};


