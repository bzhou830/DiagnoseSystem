// SplitterExt.cpp : 实现文件
//

#include "stdafx.h"
#include "RBDcm.h"
#include "SplitterExt.h"


// CSplitterExt

IMPLEMENT_DYNAMIC(CSplitterExt, CWnd)

CSplitterExt::CSplitterExt()
{

}

CSplitterExt::~CSplitterExt()
{
}


BEGIN_MESSAGE_MAP(CSplitterExt, CWnd)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

// CSplitterExt 消息处理程序

void CSplitterExt::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	//CSplitterWnd::OnMouseMove(nFlags, point);
}


void CSplitterExt::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	//CSplitterWnd::OnLButtonDown(nFlags, point);
}
