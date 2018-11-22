/***************************************************************************************
  Copyright @ Robin 2015-2025
  File name:  BaseStruct.h
  Author: Robin   Version: V1.0   Date:2015.09.05
  Description:  程序通用结构封装
                OpenCV头文件包含在此
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

//图像层类型定义 m_nType
const int IMG_LAYER        =     0;		//图像数据层类型号
const int IMG_INFO_LAYER   =     1;		//图像信息层类型号
const int IMG_TAG		   =     2;		//标记层

//与一张dcm图像相关的信息
struct sOneImgInfo
{
	CString sId;					//Patient ID        病人ID
	CString sName;					//Patient Name      病人姓名
	CString sInstitutionName;		//Insitiyution Name 检查机构名称
	CString sSex;					//Patient Sex       病人性别
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
 结节类型
 1、NONENODULE 非结节
 2、ISNODULE   结节
***************************************************************************************/
enum NoduleType
{
	NONENODULE = 0,
	ISNODULE = 1
};

/**************************************************************************************
 xml中结节标注点的信息
 1、结节点的位置
 2、结节点的标注型别
***************************************************************************************/
struct NodulePoint
{
	int x;
	int y;
	NoduleType nt;
};

/**************************************************************************************
单张图像中结节点信息
1、该张图在图像序列中的索引
2、单张图像的z向坐标
3、单张图像中结节点的集合
***************************************************************************************/
struct SingleImgNodule
{
	int index;
	list<NodulePoint> vcNodulePoint;
	double ZPosition;
};

/***************************************************************************************
每张图像对应的信息结构：
1、像素数据（从dcm文件中获取）
2、图像的附加信息，eg: PatientName ect.. （从dcm文件中获取）
3、专家标注出的该图像中包含的结节、未定性结节以及非病灶的位置 (对应文件夹下xml文件中获取)
***************************************************************************************/
struct sOneImg
{
	Mat masking;								//图像的像素数据
	Mat src_u16;								//16bits灰度图像数据
	Mat matSel;									//分割后图像
	DicomImage* pDcmimg;						//原图像数据
	sOneImgInfo info;							//图像的附加信息
	//单张图像结节位置信息, 指向的是另一个序列
	SingleImgNodule* pSingleNodules;
	bool isGood;								//标明该张图像是否正常
	double fSliceLocation;
	double fSliceSpacing;
	
	//构造时对指针变量清空
	sOneImg()
	{
		pDcmimg = NULL;
		pSingleNodules = NULL;
		isGood = false;
	}
	//析构时清空数据
	~sOneImg()
	{
		if (!pDcmimg)
			delete pDcmimg;
		if (!pSingleNodules)
			delete pSingleNodules;
	}
};

//视图窗口中的图像信息
struct sViewOneImg
{
	Mat *pPixle;								//指向像素数据指针
	sOneImgInfo info;							//图像的附加信息
	sViewOneImg()								//构造函数
	{
		pPixle = NULL;
	}
	~sViewOneImg()								//析构函数
	{
		if (!pPixle)
			delete pPixle;
	}
};

// 传入进度条对象
// 1、该张图在图像序列中的索引
// 2、单张图像的z向坐标
struct Process
{
	int cur;					//当前进度
	int sum;					//总量程
};

#endif
