// Dock.cpp : 实现文件
//

#include "stdafx.h"
#include "Dock.h"


// CDock

IMPLEMENT_DYNAMIC(CDock, CDockablePane)

CDock::CDock()
{

}

CDock::~CDock()
{
}


BEGIN_MESSAGE_MAP(CDock, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
END_MESSAGE_MAP()



// CDock 消息处理程序




int CDock::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;
	if (!m_listBox.Create(WS_CHILD | WS_VISIBLE |LVS_EX_GRIDLINES, CRect(0, 0, 0, 0), this, 0))
	{
		TRACE0("创建listbox失败");
		return -1;
	}

	return 0;
}


void CDock::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);
	if (GetSafeHwnd() == NULL)
	{
		return;
	}
	if (m_listBox.GetSafeHwnd() != NULL)
	{
		CRect rectClient;
		GetClientRect(rectClient);
		m_listBox.SetWindowPos(NULL, rectClient.left, rectClient.top, 
			rectClient.Width(), rectClient.Height(), SWP_NOACTIVATE | SWP_NOZORDER);
	}
}
