/**********************************************************************************
  Copyright @ Robin 2015-2025
  File name:  ImgInfo.h
  Author: Robin   Version: V1.0   Date:2015.11.05 
  Description:  Displayed layers about data besides image pixles 
				dicom image's relative data such as: Patient name .. 
  Others:         
***********************************************************************************/
#pragma once
#include "layer.h"
#include "Type.h"


class CImgInfoLayer : public CLayer
{
private:
	void SetLayerInfoData(sOneImgInfo info);
	sOneImgInfo GetLayerInfoData();
	void SetLayerImgData(Mat img){};						// ���ò�ͼ�����ݣ�����Ҫ��������д���̳и����
	void OnDraw(CDC* pDC,CRect rect);                               
	void OnLButtonDown(UINT nFlags, CPoint point);			//
	void OnLButtonUp(UINT nFlags, CPoint point);			//
private:
	sOneImgInfo m_info;										//ͼ����Ϣ�ṹ��
	BOOL isEmpty;
public:
	CImgInfoLayer(void);
	~CImgInfoLayer(void);
	void DrawMultLineText(CDC *pDC, CRect rect, int nRowDis, UINT nFromat, CString strText);

	static CString* SplitString(CString str, char split, int& iSubStrs);
};

