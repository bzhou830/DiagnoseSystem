/**********************************************************************************
  Copyright @ Robin 2015-2025
  File name:  ImgInfo.cpp
  Author: Robin   Version: V1.0   Date:2015.11.05 
  Description:  Displayed layers about data besides image pixles 
				dicom image's relative data such as: Patient name .. 
  Others:         
***********************************************************************************/
#include "StdAfx.h"
#include "ImgInfo.h"


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
Output:  void
Description:  NULL
Return:   void
Others:   NULL
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
Description:  NULL
Return:   void
Others:   NULL
***************************************************************************************/
sOneImgInfo CImgInfoLayer::GetLayerInfoData()
{
	return m_info;
}


/***************************************************************************************
Function: 图像相关数据绘制
Input:    CDC* 绘制的目标DC  CRect 传入DC的相关区域
Output:  void
Description:  
Return:   void
Others:   NULL
***************************************************************************************/
void CImgInfoLayer::OnDraw(CDC* pDC, CRect rect)
{
	if(!m_info.isDisplay)
		return;
	CRect rc = rect;
	pDC->SetTextColor(RGB(255, 255, 0));
	pDC->SetBkMode(TRANSPARENT);
	CFont ft;
	ft.CreateFont(14, 0, 0, 0, FW_MEDIUM, FALSE, FALSE, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, "宋体");  //创建字体
	pDC->SelectObject(ft);
	if (isEmpty)				//图像数据层为空时则不写数据
		return;
	//写文字信息
	CRect rc1(0, 0, 300, 300);												//左上角
	CRect rc2(rc.Width() - 303, 0, rc.Width() - 3, 300);					//右上角
	CRect rc3(0, rc.Height() - 15*4, 300, rc.Height());						//左下角
	CRect rc4(rc.Width()-303, rc.Height()- 15*2, rc.Width(), rc.Height());  //右下角

	CString txt1 = "Institution Name: " + m_info.sInstitutionName + "\n" +
		           "Patient Name: " + m_info.sName + "\n" +
				   "Patient Sex: " + m_info.sSex + "\n" +
				   "Patient ID: " + m_info.sId + "\n" +
				   "Patient Age: " + m_info.sAge;

	CString txt2 = "Window Width: " + m_info.sWindowWidth + "\n" +
		           "Window Center: " + m_info.sWindowCenter;

	CString txt3 = "ImagePositionPatient \nRL: " + CType::double2CString(m_info.nPositionX) 
		           + "\nAP: " + CType::double2CString(m_info.nPositionY)  
				   + "\nHF: " + CType::double2CString(m_info.nPositionZ);

	CString txt4 = "\nCopyright@2015 Robin in UESTC...";
	pDC->DrawText(txt1, &rc1, DT_LEFT);
	pDC->DrawText(txt2, &rc2, DT_RIGHT);
	pDC->DrawText(txt3, &rc3, DT_LEFT);
	pDC->DrawText(txt4, &rc4, DT_RIGHT);
}


/***************************************************************************************
Function: 鼠标左键按下消息响应
Input:    鼠标点击位置及相关信息
Output:   NULL
Description:  
Return:   void
Others:   NULL
***************************************************************************************/
void CImgInfoLayer::OnLButtonDown(UINT nFlags, CPoint point)
{

}


/***************************************************************************************
Function: 鼠标左键抬起消息响应
Input:    鼠标抬起时位置
Output:   NULL
Description:  
Return:   void
Others:   NULL
***************************************************************************************/
void CImgInfoLayer::OnLButtonUp(UINT nFlags, CPoint point)
{

}


/***************************************************************************************
Function: 字符串分割函数
Input:    传入待分割的字符串 分割字符 分割得到的结构数目
Output:   分割得到的字符串数组
Description:  
Return:   void
Others:   NULL
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


