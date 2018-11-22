#pragma once
#include "afxext.h"
#include "RBMermoryDC.h"
#include "resource.h"

//CSplitterWnd派生的子类，用来截获鼠标消息，以免鼠标可以拖动分割线
class CSplitterWndExt :
	public CSplitterWnd
{
private:
	CRect m_rectBtn;
public:
	CSplitterWndExt(void);
	~CSplitterWndExt(void);
	DECLARE_MESSAGE_MAP()
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	virtual void OnDrawSplitter(CDC* pDC, ESplitType nType, const CRect& rect);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
};

