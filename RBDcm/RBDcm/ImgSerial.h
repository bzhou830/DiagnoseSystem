/*================================================================================
Copyright @ Robin 2015 - 2025
File name:  MatImg.h
Author: Robin   Version: V1.0   Date: 2015.11.05 
Description: Dicom images to Mat-info images Type, images elements saved in vector 
==================================================================================*/
#pragma once

//dcmtk头文件
#include "dcmtk/config/osconfig.h"												//dcmtk库文件
#include "dcmtk/dcmdata/dctk.h"													//dcmtk库文件
#include "dcmtk/dcmimgle/dcmimage.h"											//dcmtk库文件
#include "xml/tinyxml.h"														//XML操作
#include "ImgInfoLayer.h"														
#include "SegmentOperat.h"														//图像分割
#include "Type.h"

const int ThreadNum = 8;

/*================================================================================
// 图像序列类
// 作用为图像的存储和图像的序列式操作
==================================================================================*/
class CImgSerial
{
public:
	CImgSerial(void) : m_index(0)												//类默认构造函数
	{
		m_vImgSerial.clear();													//清理数据
		m_itr = m_vImgSerial.begin();											//指定迭代器指示位置
	};

	~CImgSerial(void)															//类默认析构函数
	{
		m_vImgSerial.clear();													//清理数据
	};

	CImgSerial(CString lpszPathName);											//重载构造函数
	static sOneImg Dcm2MatGroup(CString lpszPathName);							//
	void Clear();																//清理m_vMatGroup内的数据
	sOneImg GetCurrentMatImg();													//获取当前迭代器所指图像数据
	list<sOneImg>::iterator GetBegin();											//图像序列首图像迭代器位置
	list<sOneImg>::iterator GetEnd();											//图像序列尾迭代器位置
	sOneImg NextMatImg();														//迭代器后移，获取下一张图像
	sOneImg LastMatImg();														//迭代器前移，获取上一张图像
	int	 GetImageNum();															//获取当前序列图像总数
	int  GetCurrentNum();														//获取当前迭代器所指图像在序列中的序号
	int  SegSeries();															//分割现有图像序列
	BOOL LoadDcm(CString lpszPathName);											//加载dcm图像
	BOOL LoadXml(CString lpszPathName);											//加载xml文件
	Mat  GetSagittal(int col,int height);										//获取矢状面数据
	Mat  GetCoronal(int row,int height);										//获取冠状面数据
	inline BOOL isEmpty();														//判断图像图像序列是否为空
	static list<sOneImg> m_vImgSerial;											//图像数据序列
	static vector<Mat> m_aip;													//AIP之后的序列
	static vector<Mat> m_mskaip;
private:
	list<SingleImgNodule> m_vSingle;											//存放解析结果序列
	list<sOneImg>::iterator m_itr;												//指向图像数据序列的迭代器
	Mat ConcavityRepair(Mat& Im, int hullgap = 50, int distgap = 100);			//修补边界
	int m_index;																//迭代器处于序列中的索引号
public:
	void AnalyseImg(void);
	void DealContours(void);
	void SortImg(void);															//对图像序列进行排序
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
//获取当前迭代器所指存储图像序列的索引号
==================================================================================*/
inline int CImgSerial::GetCurrentNum(void)
{
	return m_index;
}

/*================================================================================
//获取当前存储图像序列的图像数目
==================================================================================*/
inline int CImgSerial::GetImageNum(void)
{
	return m_vImgSerial.size();
}

/*================================================================================
//清除当前存储的所有数据
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
//获取迭代器当前所指图像数据
//迭代器当前所指数据的拷贝
==================================================================================*/
inline sOneImg CImgSerial::GetCurrentMatImg(void)
{
	return (*m_itr);
}

/*================================================================================
//获取当前存储图像序列的首元素迭代器
==================================================================================*/
inline list<sOneImg>::iterator CImgSerial::GetBegin()
{
	list<sOneImg>::iterator ir = m_vImgSerial.begin();
	return ir;
}

/*================================================================================
//获取当前存储图像序列的首元素迭代器
==================================================================================*/
inline list<sOneImg>::iterator CImgSerial::GetEnd()
{
	list<sOneImg>::iterator ir = m_vImgSerial.end();
	return (--ir);
}

/*================================================================================
//判断当前图像容器中是否存在图像
//供给上层程序调用判断序列中是否存在图像，以避免超出内存范围
==================================================================================*/
inline BOOL CImgSerial::isEmpty()
{
	if (m_vImgSerial.empty())
		return TRUE;
	else
		return FALSE;
}

