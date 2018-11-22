#pragma once
#include "Layer.h"
#include "RBMermoryDC.h"
#include "SegmentOperat.h"
#include "EnhanceOperat.h"

// CClassifierView 视图

class CLDView : public CView
{
	DECLARE_DYNCREATE(CLDView)
public:
	sOneImg m_sOneImgShow;						//showing image
	CArray <CLayer*, CLayer*> m_ls;     		//interface layers 
protected:
	CLDView();							// 动态创建所使用的受保护的构造函数
	virtual ~CLDView();

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
	void SegNodulesMean(sOneImg src);
	void HenssisenEnhance(sOneImg src);
	sOneImg GetOneImg();
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	BOOL m_isViewMax;
};


