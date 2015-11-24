// SplitterWndEx.cpp : 实现文件
//

#include "stdafx.h"
#include "MySplitterWndEx.h"


// CSplitterWndEx

IMPLEMENT_DYNAMIC(CMySplitterWndEx, CSplitterWnd)

CMySplitterWndEx::CMySplitterWndEx()
{
	m_cxSplitterGap = 2;
	m_cxSplitter = 5;
	m_cySplitterGap = 2;
	m_cySplitter = 5;
}

CMySplitterWndEx::~CMySplitterWndEx()
{
}


BEGIN_MESSAGE_MAP(CMySplitterWndEx, CSplitterWnd)
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()



// CSplitterWndEx 消息处理程序




void CMySplitterWndEx::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	//CSplitterWnd::OnLButtonDown(nFlags, point);
}


void CMySplitterWndEx::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	//CSplitterWnd::OnMouseMove(nFlags, point);
}


void CMySplitterWndEx::OnDrawSplitter(CDC* pDC, ESplitType nType, const CRect& rect)
{
	CSplitterWnd::OnDrawSplitter(pDC, nType, rect);
	if (pDC != NULL)
	{
		CBrush  brush;
		brush.CreateSolidBrush(GetSysColor(COLOR_HIGHLIGHT));
		//pDC->SelectObject(&brush);
		pDC->FillRect(&rect,&brush);
	}
}
