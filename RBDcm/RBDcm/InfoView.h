#pragma once
#include "ImgSerial.h"
#include "Type.h"



// CInfoView ��ͼ
class CInfoView : public CListView
{
	DECLARE_DYNCREATE(CInfoView)
public:
	sOneImg img;

protected:
	CInfoView();							// ��̬������ʹ�õ��ܱ����Ĺ��캯��
	virtual ~CInfoView();

public:
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();			//��ʼ������
	void SetImgData(sOneImg src);			//���������������
	void ShowData(void);					//��ʾ����
	void DeleteAll(void);					//ɾ������
};


