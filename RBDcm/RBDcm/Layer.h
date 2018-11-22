#pragma once
#include "BaseStruct.h"


//���г������������������
class CLayer
{
public:
	int m_nStatus;
	static CLayer* Create(int nType);                   //����ͼ��
	//��꼰��ʾ�¼���Ӧ
	virtual void SetLayerImgData(Mat img) = 0;          // ���ò�ͼ������
	virtual sOneImgInfo GetLayerInfoData() = 0;            // ��ȡͼ����Ϣ����
	virtual void SetLayerInfoData(sOneImgInfo info) = 0;   // ����ͼ����Ϣ����
	virtual void OnDraw(CDC* pDC, CRect rect) = 0;                           //���麯������������: �޺�����
	virtual void OnLButtonDown(UINT nFlags, CPoint point) = 0;   //������������Ϣ
	virtual void OnLButtonUp(UINT nFlags, CPoint point) = 0;     //������̧����Ϣ
	
public:
	CLayer(void);
	~CLayer(void);
};

