/*================================================================================
Copyright @ Robin 2015 - 2025
File name:  MatImg.h
Author: Robin   Version: V1.0   Date: 2015.11.05 
Description: Dicom images to Mat-info images Type, images elements saved in vector 
==================================================================================*/
#pragma once

//dcmtkͷ�ļ�
#include "dcmtk/config/osconfig.h"												//dcmtk���ļ�
#include "dcmtk/dcmdata/dctk.h"													//dcmtk���ļ�
#include "dcmtk/dcmimgle/dcmimage.h"											//dcmtk���ļ�
#include "xml/tinyxml.h"														//XML����
#include "ImgInfoLayer.h"														
#include "SegmentOperat.h"														//ͼ��ָ�
#include "Type.h"

const int ThreadNum = 8;

/*================================================================================
// ͼ��������
// ����Ϊͼ��Ĵ洢��ͼ�������ʽ����
==================================================================================*/
class CImgSerial
{
public:
	CImgSerial(void) : m_index(0)												//��Ĭ�Ϲ��캯��
	{
		m_vImgSerial.clear();													//��������
		m_itr = m_vImgSerial.begin();											//ָ��������ָʾλ��
	};

	~CImgSerial(void)															//��Ĭ����������
	{
		m_vImgSerial.clear();													//��������
	};

	CImgSerial(CString lpszPathName);											//���ع��캯��
	static sOneImg Dcm2MatGroup(CString lpszPathName);							//
	void Clear();																//����m_vMatGroup�ڵ�����
	sOneImg GetCurrentMatImg();													//��ȡ��ǰ��������ָͼ������
	list<sOneImg>::iterator GetBegin();											//ͼ��������ͼ�������λ��
	list<sOneImg>::iterator GetEnd();											//ͼ������β������λ��
	sOneImg NextMatImg();														//���������ƣ���ȡ��һ��ͼ��
	sOneImg LastMatImg();														//������ǰ�ƣ���ȡ��һ��ͼ��
	int	 GetImageNum();															//��ȡ��ǰ����ͼ������
	int  GetCurrentNum();														//��ȡ��ǰ��������ָͼ���������е����
	int  SegSeries();															//�ָ�����ͼ������
	BOOL LoadDcm(CString lpszPathName);											//����dcmͼ��
	BOOL LoadXml(CString lpszPathName);											//����xml�ļ�
	Mat  GetSagittal(int col,int height);										//��ȡʸ״������
	Mat  GetCoronal(int row,int height);										//��ȡ��״������
	inline BOOL isEmpty();														//�ж�ͼ��ͼ�������Ƿ�Ϊ��
	static list<sOneImg> m_vImgSerial;											//ͼ����������
	static vector<Mat> m_aip;													//AIP֮�������
	static vector<Mat> m_mskaip;
private:
	list<SingleImgNodule> m_vSingle;											//��Ž����������
	list<sOneImg>::iterator m_itr;												//ָ��ͼ���������еĵ�����
	Mat ConcavityRepair(Mat& Im, int hullgap = 50, int distgap = 100);			//�޲��߽�
	int m_index;																//���������������е�������
public:
	void AnalyseImg(void);
	void DealContours(void);
	void SortImg(void);															//��ͼ�����н�������
	void ImgAIP(int num);
	void mskAIP(int num);
	static DWORD WINAPI ProcLoad(LPVOID lp);
	static BOOL LoadDcms(CStringArray* paths);
	static HANDLE m_hMutex;
	static CStringArray csa[ThreadNum];
	static list<sOneImg> li[ThreadNum];
	static char flag;
	static vector<vector<Point> > cntr;
	static vector<Vec4i> hy;
	void Merge();
};

/*================================================================================
//��ȡ��ǰ��������ָ�洢ͼ�����е�������
==================================================================================*/
inline int CImgSerial::GetCurrentNum(void)
{
	return m_index;
}

/*================================================================================
//��ȡ��ǰ�洢ͼ�����е�ͼ����Ŀ
==================================================================================*/
inline int CImgSerial::GetImageNum(void)
{
	return m_vImgSerial.size();
}

/*================================================================================
//�����ǰ�洢����������
==================================================================================*/
inline void CImgSerial::Clear(void)
{
	m_vImgSerial.clear();
	m_aip.clear();
	m_mskaip.clear();
	for (int i=0; i<ThreadNum; i++)
		csa[i].RemoveAll();
	flag = 0x00;
}

/*================================================================================
//��ȡ��������ǰ��ָͼ������
//��������ǰ��ָ���ݵĿ���
==================================================================================*/
inline sOneImg CImgSerial::GetCurrentMatImg(void)
{
	return (*m_itr);
}

/*================================================================================
//��ȡ��ǰ�洢ͼ�����е���Ԫ�ص�����
==================================================================================*/
inline list<sOneImg>::iterator CImgSerial::GetBegin()
{
	list<sOneImg>::iterator ir = m_vImgSerial.begin();
	return ir;
}

/*================================================================================
//��ȡ��ǰ�洢ͼ�����е���Ԫ�ص�����
==================================================================================*/
inline list<sOneImg>::iterator CImgSerial::GetEnd()
{
	list<sOneImg>::iterator ir = m_vImgSerial.end();
	return (--ir);
}

/*================================================================================
//�жϵ�ǰͼ���������Ƿ����ͼ��
//�����ϲ��������ж��������Ƿ����ͼ���Ա��ⳬ���ڴ淶Χ
==================================================================================*/
inline BOOL CImgSerial::isEmpty()
{
	if (m_vImgSerial.empty())
		return TRUE;
	else
		return FALSE;
}

