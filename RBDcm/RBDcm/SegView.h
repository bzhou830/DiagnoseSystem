#pragma once
#include "Layer.h"
#include "ImgDataLayer.h"
#include "ImgInfoLayer.h"
#include "RBMermoryDC.h"
#include "SegmentOperat.h"

// CSegView ��ͼ
class CRUView : public CView
{
	DECLARE_DYNCREATE(CRUView)
public:
	sViewOneImg m_sOneImgShow;			//showing image
	CArray <CLayer*, CLayer*> m_ls;     //interface layers 
protected:
	CRUView();							// ��̬������ʹ�õ��ܱ����Ĺ��캯��
	virtual ~CRUView();

public:
	virtual void OnDraw(CDC* pDC);      // ��д�Ի��Ƹ���ͼ
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
	void SetImgData(sOneImg src);
	void SegRealLung(cv::Mat src);
	void GetSegRealLungs(sOneImg& seg);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	BOOL m_isViewMax;
};


