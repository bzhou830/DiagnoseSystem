#include "stdafx.h"
#include "SplitterWndExt.h"


CSplitterWndExt::CSplitterWndExt(void)
{
	m_cxSplitterGap = 12;				//边界调宽，使得有地方绘制箭头
	m_cxSplitter = 12;
	m_cySplitterGap = 5;
	m_cySplitter = 5;
}


CSplitterWndExt::~CSplitterWndExt(void)
{
}

BEGIN_MESSAGE_MAP(CSplitterWndExt, CSplitterWnd)
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDBLCLK()
END_MESSAGE_MAP()


void CSplitterWndExt::OnLButtonDown(UINT nFlags, CPoint point)
{
	//CSplitterWnd::OnLButtonDown(nFlags, point);
}


void CSplitterWndExt::OnMouseMove(UINT nFlags, CPoint point)
{
	//CSplitterWnd::OnMouseMove(nFlags, point);
}


void CSplitterWndExt::OnDrawSplitter(CDC* pDC, ESplitType nType, const CRect& rect)
{
	CRBMemoryDC mdc;
	mdc.LoadBitmap(IDB_BITMAP_LEFT);
	int left, min;
	CRect rc;
	GetWindowRect((LPRECT)&rc);
	GetColumnInfo(0, left, min);
	m_rectBtn.SetRect(left+4, rect.Height()/2 - 16, left+10, rect.Height()/2 + 15);
	if (pDC != NULL)
		pDC->BitBlt(m_rectBtn.left, m_rectBtn.top, m_rectBtn.Width(), m_rectBtn.Height(), &mdc, 0, 0, SRCCOPY);
	CSplitterWnd::OnDrawSplitter(pDC, nType, rect);
}



void CSplitterWndExt::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	int left,min;
	CMainFrame *pMain = (CMainFrame*)AfxGetMainWnd();
	GetColumnInfo(0,left,min);
	if (0 == left || left == min)			//面板已经被收缩了
	{
		SetColumnInfo(0,200,10);			//展开面板
		pMain->m_isLeftPaneMin = FALSE;
	}
	else									//面板没有被收缩
	{
		SetColumnInfo(0,0,10);				//收缩面板
		pMain->m_isLeftPaneMin = TRUE;
	}
	RecalcLayout();
	CSplitterWnd::OnLButtonDblClk(nFlags, point);
	pMain->m_splt2.RecalcLayout();
	pMain->ResetFrm();
}


