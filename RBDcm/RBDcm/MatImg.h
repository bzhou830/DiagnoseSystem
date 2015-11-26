/**********************************************************************************
  Copyright @ Robin 2015-2025
  File name:  MatImg.h
  Author: Robin   Version: V1.0   Date:2015.11.05 
  Description: Dcm images to Mat-info images Type, iamges elements all in vector 
  Others:         
***********************************************************************************/
#pragma once

//dcmtk头文件
#include "dcmtk/config/osconfig.h"
#include "dcmtk/dcmdata/dctk.h"
#include "dcmtk/dcmimgle/dcmimage.h"

#include "xml/tinyxml.h"							//XML操作
#include "ImgInfo.h"
#include "Type.h"




class CImgSerial
{
public:
	CImgSerial(void) : m_index(0)					//类默认构造函数
	{
		m_vImgSerial.clear();						//清理数据
		m_itr = m_vImgSerial.begin();				//指定迭代器指示位置
	};

	~CImgSerial(void)								//类默认析构函数
	{
		m_vImgSerial.clear();						//清理数据
	};

	CImgSerial(CString lpszPathName);				//重载构造函数
	sOneImg Dcm2MatGroup(CString lpszPathName);		//重载构造函数
	void Clear();									//清理m_vMatGroup内的数据
	sOneImg GetCurrentMatImg();						//获取当前迭代器所指图像数据
	list<sOneImg>::iterator GetBegin();				//图像序列首图像迭代器位置
	sOneImg NextMatImg();							//迭代器后移，获取下一张图像
	sOneImg LastMatImg();							//迭代器前移，获取上一张图像
	int GetImageNum();								//获取当前序列图像总数
	int GetCurrentNum();							//获取当前迭代器所指图像在序列中的序号
	BOOL LoadDcm(CString lpszPathName);				//加载dcm图像
	BOOL LoadXml(CString lpszPathName);				//加载xml文件

private:
	list<sOneImg> m_vImgSerial;						//图像数据序列
	list<SingleImgNodule> m_vSingle;				//存放解析结果序列
	list<sOneImg>::iterator m_itr;					//指向图像数据序列的迭代器
	int m_index;									//迭代器处于序列中的索引号
};


/***************************************************************************************
Function: 获取当前迭代器所指存储图像序列的索引号
Input:    NULL
Output:   索引号
Description: 
Return:   
Others:   NULL
***************************************************************************************/
inline int CImgSerial::GetCurrentNum(void)
{
	return m_index;
}


/***************************************************************************************
Function: 获取当前存储图像序列的图像数目
Input:    NULL
Output:   数目
Description: 
Return:   
Others:   NULL
***************************************************************************************/
inline int CImgSerial::GetImageNum(void)
{
	return m_vImgSerial.size();
}


/***************************************************************************************
Function: 清除当前存储的所有数据
Input:    NULL
Output:   如果迭代器所指的不是序列第一张则返回上一张，若是第一张则返回当前张
Description: 
Return:   MatGroup
Others:   NULL
***************************************************************************************/
inline void CImgSerial::Clear(void)
{
	m_vImgSerial.clear();
}


/***************************************************************************************
Function: 获取迭代器当前所指图像数据
Input:    NULL
Output:   迭代器当前所指数据的拷贝
Description: 
Return:   MatGroup
Others:   NULL
***************************************************************************************/
inline sOneImg CImgSerial::GetCurrentMatImg(void)
{
	return (*m_itr);
}


/***************************************************************************************
Function: 获取当前存储图像序列的首元素迭代器
Input:    NULL
Output:   迭代器类型
Description: 
Return:   
Others:   NULL
***************************************************************************************/
inline list<sOneImg>::iterator CImgSerial::GetBegin()
{
	list<sOneImg>::iterator ir = m_vImgSerial.begin();
	return ir;
}




