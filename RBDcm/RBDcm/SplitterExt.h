#pragma once


// CSplitterExt
class CSplitterExt : public CSplitterWnd
{
	DECLARE_DYNAMIC(CSplitterExt)

public:
	CSplitterExt();
	virtual ~CSplitterExt();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
};


