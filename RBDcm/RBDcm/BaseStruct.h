/***************************************************************************************
  Copyright @ Robin 2015-2025
  File name:  BaseStruct.h
  Author: Robin   Version: V1.0   Date:2015.09.05 
  Description:  Commom data structure definition 
                Contain OpenCV header files 
  Others:      This File would be contiand in stdafx.h file.
               In order to simplify other files not to re-contain this file.
***************************************************************************************/
#ifndef __BASESTRCUT__
#define __BASESTRCUT__

//OpenCV header files 
#include <opencv2/highgui/highgui.hpp>
#include <cv.hpp>
#include <opencv2/opencv.hpp>
#include <core/core.hpp>

//STL header files
#include <list>
using namespace cv;				    //opencv namespace 
using namespace std;


//图像层类型定义 m_nType
#define IMG_LAYER             0    //图像数据层类型号
#define IMG_INFO_LAYER        1    //图像信息层类型号


//与一张dcm图像相关的信息
struct sOneImgInfo 
{
	CString sId;					//Patient ID
	CString sName;					//Patient Name 
	CString sInstitutionName;		//Insitiyution Name
	CString sSex;					//Patient Sex
	CString sAge;					//Patient Age
	CString sWindowCenter;          //Window Center
	CString sWindowWidth;           //Window Width
	CString sImgPositionPatient;    //Image Position Patient
	double nPositionX;				//top-left position of X
	double nPositionY;				//top-left position of Y
	double nPositionZ;				//Key infomation about images
	bool isDisplay;					//is Displayed in the screen flag
	//对运算符 = 进行重载
	sOneImgInfo& operator= (sOneImgInfo& vl)
	{
		sId = vl.sId;
		sName = vl.sName;
		sInstitutionName = vl.sInstitutionName;
		sSex = vl.sSex;
		sAge = vl.sAge;
		sWindowCenter = vl.sWindowCenter;
		sWindowWidth = vl.sWindowWidth;
		sImgPositionPatient = vl.sImgPositionPatient;
		nPositionX = vl.nPositionX;
		nPositionY = vl.nPositionY;
		nPositionZ = vl.nPositionZ;
		isDisplay = vl.isDisplay;
		return *this;
	}
};


/**************************************************************************************
 结节类型
 1、NONENODULE 非结节
 2、ISNODULE   结节
***************************************************************************************/
enum NoduleType
{
	NONENODULE=0,
	ISNODULE=1
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
	Mat pixle;									//图像的像素数据
	sOneImgInfo info;							//图像的附加信息
	//单张图像结节位置信息,指向的是另一个序列的
	SingleImgNodule* pSingleNodules;			
};



#endif



