/***************************************************************************************
  Copyright @ Robin 2015-2025
  File name:  BaseStruct.h
  Author: Robin   Version: V1.0   Date:2015.09.05
  Description:  ����ͨ�ýṹ��װ
                OpenCVͷ�ļ������ڴ�
  Others:      This File would be contiand in stdafx.h file.
               In order to simplify other files not to re-contain this file.
***************************************************************************************/
#ifndef __BASESTRCUT__
#define __BASESTRCUT__

//OpenCV header files
#include <opencv.hpp>

#include "dcm/include/dcmtk/dcmimgle/dcmimage.h"//for type DicomImage

//STL header files
#include <list>

using namespace cv;						//opencv namespace
using namespace std;

//ͼ������Ͷ��� m_nType
const int IMG_LAYER        =     0;		//ͼ�����ݲ����ͺ�
const int IMG_INFO_LAYER   =     1;		//ͼ����Ϣ�����ͺ�
const int IMG_TAG		   =     2;		//��ǲ�

//��һ��dcmͼ����ص���Ϣ
struct sOneImgInfo
{
	CString sId;					//Patient ID        ����ID
	CString sName;					//Patient Name      ��������
	CString sInstitutionName;		//Insitiyution Name ����������
	CString sSex;					//Patient Sex       �����Ա�
	CString sAge;					//Patient Age
	CString sWindowCenter;          //Window Center
	CString sWindowWidth;           //Window Width
	CString sImgPositionPatient;    //Image Position Patient
	double slicesickness;
	double nPositionX;				//top-left position of X
	double nPositionY;				//top-left position of Y
	double nPositionZ;				//Key infomation about images
	bool isDisplay;					//is Displayed in the screen flag
};

/**************************************************************************************
 �������
 1��NONENODULE �ǽ��
 2��ISNODULE   ���
***************************************************************************************/
enum NoduleType
{
	NONENODULE = 0,
	ISNODULE = 1
};

/**************************************************************************************
 xml�н�ڱ�ע�����Ϣ
 1����ڵ��λ��
 2����ڵ�ı�ע�ͱ�
***************************************************************************************/
struct NodulePoint
{
	int x;
	int y;
	NoduleType nt;
};

/**************************************************************************************
����ͼ���н�ڵ���Ϣ
1������ͼ��ͼ�������е�����
2������ͼ���z������
3������ͼ���н�ڵ�ļ���
***************************************************************************************/
struct SingleImgNodule
{
	int index;
	list<NodulePoint> vcNodulePoint;
	double ZPosition;
};

/***************************************************************************************
ÿ��ͼ���Ӧ����Ϣ�ṹ��
1���������ݣ���dcm�ļ��л�ȡ��
2��ͼ��ĸ�����Ϣ��eg: PatientName ect.. ����dcm�ļ��л�ȡ��
3��ר�ұ�ע���ĸ�ͼ���а����Ľ�ڡ�δ���Խ���Լ��ǲ����λ�� (��Ӧ�ļ�����xml�ļ��л�ȡ)
***************************************************************************************/
struct sOneImg
{
	Mat masking;								//ͼ�����������
	Mat src_u16;								//16bits�Ҷ�ͼ������
	Mat matSel;									//�ָ��ͼ��
	DicomImage* pDcmimg;						//ԭͼ������
	sOneImgInfo info;							//ͼ��ĸ�����Ϣ
	//����ͼ����λ����Ϣ, ָ�������һ������
	SingleImgNodule* pSingleNodules;
	bool isGood;								//��������ͼ���Ƿ�����
	double fSliceLocation;
	double fSliceSpacing;
	
	//����ʱ��ָ��������
	sOneImg()
	{
		pDcmimg = NULL;
		pSingleNodules = NULL;
		isGood = false;
	}
	//����ʱ�������
	~sOneImg()
	{
		if (!pDcmimg)
			delete pDcmimg;
		if (!pSingleNodules)
			delete pSingleNodules;
	}
};

//��ͼ�����е�ͼ����Ϣ
struct sViewOneImg
{
	Mat *pPixle;								//ָ����������ָ��
	sOneImgInfo info;							//ͼ��ĸ�����Ϣ
	sViewOneImg()								//���캯��
	{
		pPixle = NULL;
	}
	~sViewOneImg()								//��������
	{
		if (!pPixle)
			delete pPixle;
	}
};

// �������������
// 1������ͼ��ͼ�������е�����
// 2������ͼ���z������
struct Process
{
	int cur;					//��ǰ����
	int sum;					//������
};

#endif
