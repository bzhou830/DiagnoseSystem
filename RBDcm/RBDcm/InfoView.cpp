// InfoView.cpp : 实现文件
#include "stdafx.h"
#include "RBDcm.h"
#include "InfoView.h"


// CInfoView
IMPLEMENT_DYNCREATE(CInfoView, CListView)

CInfoView::CInfoView()
{
}

CInfoView::~CInfoView()
{
}

BEGIN_MESSAGE_MAP(CInfoView, CListView)
END_MESSAGE_MAP()


// CInfoView 诊断
#ifdef _DEBUG
void CInfoView::AssertValid() const
{
	CListView::AssertValid();
}

#ifndef _WIN32_WCE
void CInfoView::Dump(CDumpContext& dc) const
{
	CListView::Dump(dc);
}
#endif
#endif //_DEBUG


// CInfoView 消息处理程序
void CInfoView::OnInitialUpdate()
{
	CListView::OnInitialUpdate();
	CListCtrl& list = GetListCtrl();
	LONG lStyle;
	lStyle = GetWindowLong(list.m_hWnd, GWL_STYLE);			//获取当前窗口风格
	lStyle &= ~LVS_TYPEMASK;								//清除显示方式位
	lStyle |= LVS_REPORT;									//设置报表风格
	SetWindowLong(list.m_hWnd, GWL_STYLE, lStyle);			//设置窗口风格
	DWORD dwStyle = list.GetExtendedStyle();				//获取扩展风格
	dwStyle |= LVS_EX_FULLROWSELECT;						//全行可选中
	dwStyle |= LVS_EX_GRIDLINES;							//行列线显示
	list.SetExtendedStyle(dwStyle);							//设置扩展风格
	list.SetBkColor(RGB(200, 200, 200));					//背景色设置
	list.SetTextBkColor(RGB(200, 200, 200));				//设置文本背景颜色
	list.SetTextColor(RGB(10, 10, 80));						//设置文本颜色
	list.InsertColumn(0, "序号", LVCFMT_CENTER, 80);			//插入列
	list.InsertColumn(1, "zPosition", LVCFMT_CENTER, 100);	//插入列
	list.InsertColumn(2, "像素位置", LVCFMT_CENTER, 100);	//插入列
	list.InsertColumn(3, "是否是结节", LVCFMT_CENTER, 100);	//插入列
}


void CInfoView::SetImgData(sOneImg src)
{
	img.pSingleNodules = src.pSingleNodules;
	if (img.pSingleNodules == NULL)
		return;
	ShowData();
}


void CInfoView::ShowData(void)
{
	CListCtrl& listctr = GetListCtrl();
	listctr.DeleteAllItems();
	int size = img.pSingleNodules->vcNodulePoint.size();
	for (int i=0; i<size; ++i)
	{
		int index = listctr.InsertItem(i, CType::int2CString(i));
		listctr.SetItemText(index, 1, CType::double2CString(img.pSingleNodules->ZPosition));
		list<NodulePoint>::iterator itr = img.pSingleNodules->vcNodulePoint.begin();
		std::advance(itr,i);
		NodulePoint pt = *itr;
		CString str;
		str.Format("(%d，%d)", pt.x, pt.y);
		listctr.SetItemText(index, 2, str);
		if (pt.nt == NONENODULE)
			listctr.SetItemText(index, 3, "FALSE");
		else if(pt.nt == ISNODULE)
			listctr.SetItemText(index, 3, "TRUE");
	}
}


void CInfoView::DeleteAll(void)
{
	CListCtrl& list = GetListCtrl();
	list.DeleteAllItems();
}
