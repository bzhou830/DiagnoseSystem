/**********************************************************************************
  Copyright @ Robin 2015-2025
  File name:  RBDcmDoc.h
  Author: Robin   Version: V1.0   Date:2015.11.05 
  Description: Software Frame  Docment Class 
               This class contain main image algorithm  
  Others:         
***********************************************************************************/
#pragma once

#include "Layer.h"
#include "afxtempl.h"
#include <vector>
#include "ImgSerial.h"
//#include "OprtView.h"   �ڴ˰��������
#include "ImgDataLayer.h"
#include "PlaySeriesDlg.h"
#include "SegmentOperat.h"
#include "EnhanceOperat.h"
#include "ClassficationOperat.h"


using namespace cv;    //for mat operator
using namespace std;   //for vector


class CRBDcmDoc : public CDocument
{
public:
	//the following three Mat are used to compatiable the old version
	Mat m_srcImage;						//src image
	Mat m_dstImage;						//dst image
	Mat m_showImage;					//showing image

	CImgSerial m_matimg;                //Mat-image structure
	CArray <CLayer*, CLayer*> m_ls;     //interface layers 
	CPlaySeriesDlg* m_PlayDlg;			//Play serial dialog
protected:
	CRBDcmDoc();
	DECLARE_DYNCREATE(CRBDcmDoc)
public:
	friend DWORD WINAPI PlaySerileProc(LPVOID lpParameter);

public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

public:
	virtual ~CRBDcmDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// ����Ϊ����������������������ݵ� Helper ����
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
public:
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);   //�ļ���
	//afx_msg void OnFileSave();		//�ļ�����	
	void Hist(void);				//ͼ��ֱ��ͼ����
	void Cut(void);					//��ֵ�ָ�
	void Rst(void);					//ͼ��ԭ
	void Mask(void);				//ͼ����Ĥ
	void Median(void);				//��ֵ�˲�
	void DrawHist(void);			//����ͼ��Ҷ�ֱ��ͼ
	void Entropy(void);				//�طָ�
	void Ostu(void);				//OSTU�ָ�
	void Enhance(void);				//��ǿ
	void SrcImg(void);				//��ʾԭͼ��
	void SegLung(void);				//�ָ��ʵ��
public:
	static DWORD WINAPI PlaySerileProc(LPVOID lpParameter);
	afx_msg void OnImginfodis();
	afx_msg void OnUpdateImginfodis(CCmdUI *pCmdUI);
	//afx_msg void OnSerile();			//��ͼ������
	void ShowImg(void);					//��ͼ���д����
	void FillHoles(Mat img);			//�������
	void AddTimeToStatusBar(int time);	//��״̬��д�㷨����ʱ��
	void SetStatusBar(CString str);		//����״̬����Ϣ
	void Video(void);					//������ͷ
	void PlaySerile(void);				//����ͼ������
	void Last(void);					//��һ��ͼ��
	void Next(void);					//��һ��ͼ��
	void Mean(void);					//��ֵ�˲�
	void MoM(void);						//�ط����ָ�
	void DestroyObject(void);
	void LoadXml(void);
};



