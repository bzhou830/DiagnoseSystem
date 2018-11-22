/**********************************************************************************
  Copyright @ Robin 2015-2025
  File name:  ImgInfo.cpp
  Author: Robin   Version: V1.0   Date:2015.11.05 
  Description:  Displayed layers about data besides image pixles 
				dicom image's relative data such as: Patient name 
***********************************************************************************/
#include "StdAfx.h"
#include "ImgInfoLayer.h"


CImgInfoLayer::CImgInfoLayer(void)
{
	//memset(&m_info, 0, sizeof(m_info));
	isEmpty = TRUE;
}

CImgInfoLayer::~CImgInfoLayer(void)
{
}

/***************************************************************************************
Function: 传入图像相关数据层数据
Input:    图像相关数据结构体
***************************************************************************************/
void CImgInfoLayer::SetLayerInfoData(sOneImgInfo info)
{
	m_info = info;
	isEmpty = FALSE;
}

/***************************************************************************************
Function: 获取当前图像相关数据层数据
Input:    NULL
Output:  图像相关数据结构体
***************************************************************************************/
sOneImgInfo CImgInfoLayer::GetLayerInfoData()
{
	return m_info;
}

/***************************************************************************************
Function: 图像相关数据绘制
Input:    CDC* 绘制的目标DC  CRect 传入DC的相关区域
***************************************************************************************/
void CImgInfoLayer::OnDraw(CDC* pDC, CRect rect)
{
	if(!m_info.isDisplay)
		return;
	CRect rc = rect;
	pDC->SetTextColor(RGB(255, 128, 0));
	pDC->SetBkMode(TRANSPARENT);
	CFont ft;
	ft.CreateFont(12, 0, 0, 0, FW_MEDIUM, FALSE, FALSE, 0, GB2312_CHARSET, OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, "宋体");  //创建字体
	pDC->SelectObject(ft);
	if (isEmpty)				//图像数据层为空时则不写数据
		return;
	//写文字信息
	CRect rc1(3, 3, 300, 300);												//左上角
	CRect rc2(rc.Width() - 303, 3, rc.Width() - 3, 300);					//右上角
	CRect rc3(3, rc.Height() - 15*3, 300, rc.Height());						//左下角
	CRect rc4(rc.Width()-303, rc.Height()- 15*2, rc.Width(), rc.Height());  //右下角

	CString txt1 = "检查设备: " + m_info.sInstitutionName + "\r\n" +
		           "姓名: " + m_info.sName + "\r\n" +
				   "性别: " + m_info.sSex + "\r\n" +
				   "ID号: " + m_info.sId + "\r\n" +
				   "年龄: " + m_info.sAge + "\r\n";

	CString tmp;
	tmp.Format("%.2lf",m_info.slicesickness);
	CString txt2 = "窗宽: " + m_info.sWindowWidth + " \r\n" +
		           "窗位: " + m_info.sWindowCenter + " \r\n" +
				   "层厚: " + tmp + "mm\r\n";

	CString txt3 = "RL: " + CType::double2CString(m_info.nPositionX) 
		           + "\r\nAP: " + CType::double2CString(m_info.nPositionY)  
				   + "\r\nHF: " + CType::double2CString(m_info.nPositionZ)
				   + "\r\n";

	CString txt4 = "\nUESTC-Robin\n";

	DrawMultLineText(pDC, rc1, 2, DT_LEFT, txt1);
	DrawMultLineText(pDC, rc2, 2, DT_RIGHT, txt2);
	DrawMultLineText(pDC, rc3, 2, DT_LEFT, txt3);
	pDC->DrawText(txt4, &rc4, DT_RIGHT);
}

/*===========================================================================*
* 在指定DC上写多行文字
*============================================================================*/
void CImgInfoLayer::DrawMultLineText(CDC *pDC, CRect rect, int nRowDis, UINT nFromat, CString strText)
{
	int len = strText.GetLength();
	int j = 0;
	for(int i = 0; i < len; i++)
	{
		if(strText[i] != '\n')
			continue;
		CString str  = strText.Mid(j, i - j);
		str += '\n';
		pDC->DrawText(str, &rect, nFromat);
		rect.top += (12 + nRowDis);
		j = i+1;
	}
}


/***************************************************************************************
Function: 鼠标左键按下消息响应
Input:    鼠标点击位置及相关信息
***************************************************************************************/
void CImgInfoLayer::OnLButtonDown(UINT nFlags, CPoint point)
{
}


/***************************************************************************************
Function: 鼠标左键抬起消息响应
Input:    鼠标抬起时位置
***************************************************************************************/
void CImgInfoLayer::OnLButtonUp(UINT nFlags, CPoint point)
{
}


/***************************************************************************************
Function: 字符串分割函数
Input:    传入待分割的字符串 分割字符 分割得到的结构数目
Output:   分割得到的字符串数组
***************************************************************************************/
CString* CImgInfoLayer::SplitString(CString str, char split, int& iSubStrs)
{
	int iPos = 0;				//分割符位置
	int iNums = 0;				//分割符的总数
	CString strTemp = str;
	CString strRight;
	//先计算子字符串的数量
	while (iPos != -1)
	{
		iPos = strTemp.Find(split);
		if (iPos == -1)
		{
			break;
		}
		strRight = strTemp.Mid(iPos + 1, str.GetLength());
		strTemp = strRight;
		iNums++;
	}
	if (iNums == 0)				//没有找到分割符
	{
		//子字符串数就是字符串本身
		iSubStrs = 1; 
		return NULL;
	}
	//子字符串数组
	iSubStrs = iNums + 1; //子串的数量 = 分割符数量 + 1
	CString* pStrSplit;
	pStrSplit = new CString[iSubStrs];
	strTemp = str;
	CString strLeft;
	for (int i = 0; i < iNums; i++)
	{
		iPos = strTemp.Find(split);
		//左子串
		strLeft = strTemp.Left(iPos);
		//右子串
		strRight = strTemp.Mid(iPos + 1, strTemp.GetLength());
		strTemp = strRight;
		pStrSplit[i] = strLeft;
	}
	pStrSplit[iNums] = strTemp;
	return pStrSplit;
}


