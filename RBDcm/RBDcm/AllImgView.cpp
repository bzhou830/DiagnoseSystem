// AllImgView.cpp : 实现文件
//

#include "stdafx.h"
#include "RBDcm.h"
#include "AllImgView.h"


// CAllImgView

IMPLEMENT_DYNCREATE(CAllImgView, CView)

CAllImgView::CAllImgView()
{

}

CAllImgView::~CAllImgView()
{
}

BEGIN_MESSAGE_MAP(CAllImgView, CView)
END_MESSAGE_MAP()


// CAllImgView 绘图

void CAllImgView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: 在此添加绘制代码
}


// CAllImgView 诊断

#ifdef _DEBUG
void CAllImgView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void CAllImgView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG


// CAllImgView 消息处理程序
