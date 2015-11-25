#pragma once
#include "afxext.h"

//CSplitterWnd派生的子类，用来截获鼠标消息，以免鼠标可以拖动分割线
class CSplitterWndExt :
	public CSplitterWnd
{
public:
	CSplitterWndExt(void);
	~CSplitterWndExt(void);
	DECLARE_MESSAGE_MAP()
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
};



