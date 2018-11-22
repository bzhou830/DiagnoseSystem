#pragma once


// CSplitterWndEx

class CMySplitterWndEx : public CSplitterWnd
{
	DECLARE_DYNAMIC(CMySplitterWndEx)

public:
	CMySplitterWndEx();
	virtual ~CMySplitterWndEx();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	virtual void OnDrawSplitter(CDC* pDC, ESplitType nType, const CRect& rect);
};





