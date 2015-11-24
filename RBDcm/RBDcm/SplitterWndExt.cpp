#include "stdafx.h"
#include "SplitterWndExt.h"


CSplitterWndExt::CSplitterWndExt(void)
{
}


CSplitterWndExt::~CSplitterWndExt(void)
{
}

BEGIN_MESSAGE_MAP(CSplitterWndExt, CSplitterWnd)
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()


void CSplitterWndExt::OnLButtonDown(UINT nFlags, CPoint point)
{
	//CSplitterWnd::OnLButtonDown(nFlags, point);
}


void CSplitterWndExt::OnMouseMove(UINT nFlags, CPoint point)
{
	//CSplitterWnd::OnMouseMove(nFlags, point);
}
