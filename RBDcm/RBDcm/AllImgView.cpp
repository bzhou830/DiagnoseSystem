// AllImgView.cpp : ʵ���ļ�
//
#include "stdafx.h"
#include "RBDcm.h"
#include "AllImgView.h"

// CAllImgView
IMPLEMENT_DYNCREATE(CAllImgView, CView)

CAllImgView::CAllImgView()
{
}

CAllImgView::~CAllImgView()
{
}

BEGIN_MESSAGE_MAP(CAllImgView, CView)
END_MESSAGE_MAP()

// CAllImgView ��ͼ
void CAllImgView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: �ڴ���ӻ��ƴ���
}


// CAllImgView ���

#ifdef _DEBUG
void CAllImgView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void CAllImgView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG

// CAllImgView ��Ϣ�������
