// SplitterExt.cpp : ʵ���ļ�
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



// CSplitterExt ��Ϣ�������




void CSplitterExt::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	//CSplitterWnd::OnMouseMove(nFlags, point);
}


void CSplitterExt::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	//CSplitterWnd::OnLButtonDown(nFlags, point);
}
