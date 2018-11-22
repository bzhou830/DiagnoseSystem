// InfoView.cpp : ʵ���ļ�
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


// CInfoView ���
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


// CInfoView ��Ϣ�������
void CInfoView::OnInitialUpdate()
{
	CListView::OnInitialUpdate();
	CListCtrl& list = GetListCtrl();
	LONG lStyle;
	lStyle = GetWindowLong(list.m_hWnd, GWL_STYLE);			//��ȡ��ǰ���ڷ��
	lStyle &= ~LVS_TYPEMASK;								//�����ʾ��ʽλ
	lStyle |= LVS_REPORT;									//���ñ�����
	SetWindowLong(list.m_hWnd, GWL_STYLE, lStyle);			//���ô��ڷ��
	DWORD dwStyle = list.GetExtendedStyle();				//��ȡ��չ���
	dwStyle |= LVS_EX_FULLROWSELECT;						//ȫ�п�ѡ��
	dwStyle |= LVS_EX_GRIDLINES;							//��������ʾ
	list.SetExtendedStyle(dwStyle);							//������չ���
	list.SetBkColor(RGB(200, 200, 200));					//����ɫ����
	list.SetTextBkColor(RGB(200, 200, 200));				//�����ı�������ɫ
	list.SetTextColor(RGB(10, 10, 80));						//�����ı���ɫ
	list.InsertColumn(0, "���", LVCFMT_CENTER, 40);			//������
	list.InsertColumn(1, "zPos", LVCFMT_CENTER, 45);	    //������
	list.InsertColumn(2, "����λ��", LVCFMT_CENTER, 60);	    //������
	list.InsertColumn(3, "�Ƿ���", LVCFMT_CENTER, 70);	    //������
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
		str.Format("(%d��%d)", pt.x, pt.y);
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
