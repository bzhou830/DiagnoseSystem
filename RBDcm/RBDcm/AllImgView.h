#pragma once


// CAllImgView ��ͼ
class CAllImgView : public CView
{
	DECLARE_DYNCREATE(CAllImgView)
protected:
	CAllImgView();							// ��̬������ʹ�õ��ܱ����Ĺ��캯��
	virtual ~CAllImgView();

public:
	virtual void OnDraw(CDC* pDC);			// ��д�Ի��Ƹ���ͼ
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	DECLARE_MESSAGE_MAP()
};


