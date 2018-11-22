/**********************************************************************************
  Copyright @ Robin 2015-2025
  File name:  RBMemroyDC.cpp
  Author: Robin   Version: V1.0   Date: 2015.11.05 
  Description: This class contain operation ablout CMemDC
  Others:         
***********************************************************************************/
#include "stdafx.h"
#include "RBMermoryDC.h"


/***************************************************************************************
Function: 从项目资源中导入位图
Input:    位图资源ID
Output:   NULL
Description: 从项目资源中导入位图资源到DC
Return:   BOOL 位图导入成功标志
Others:   NULL
***************************************************************************************/
BOOL CRBMemoryDC::LoadBitmap(UINT nBitmapID, CDC *pDC)
{
	CBitmap bitmap;               
	if(!bitmap.LoadBitmap(nBitmapID))   //导入位图（从资源中导入）
		return FALSE;                   //导入失败直接返回false
	BITMAP bmp;                         //位图文件
	bitmap.GetBitmap(&bmp);             //获取图像信息
	m_size.cx = bmp.bmWidth;            //位图高宽信息
	m_size.cy = bmp.bmHeight;    
	this->CreateCompatibleDC(pDC);      //创建兼容dc
	this->SelectObject(bitmap);         //将位图选入到兼容dc
	return TRUE;
}


/***************************************************************************************
Function: 从文件中导入位图
Input:    位图路径
Output:   BOOL
Description: 从指定未见路径中导入位图资源到DC
Return:   BOOL 位图导入成功标志
Others:   NULL
***************************************************************************************/
BOOL CRBMemoryDC::LoadBitmap(LPCSTR szBitmapFile, CDC *pDC)
{
	//从图像文件中导入位图
	HBITMAP hBitmap = (HBITMAP)LoadImage(AfxGetInstanceHandle(),
					   szBitmapFile, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	BITMAP bmp;
	GetObject(hBitmap, sizeof(bmp), &bmp);
	m_size.cx = bmp.bmWidth;
	m_size.cy = bmp.bmHeight;
	this->CreateCompatibleDC(pDC);
	this->SelectObject(hBitmap);
	return TRUE;
}


/***************************************************************************************
Function: 创建一个空白的DC
Input:    DC尺寸
Output:   BOOL
Description: 根据指定的尺寸创建一个空白的DC
Return:   BOOL 创建成功标志
Others:   NULL
***************************************************************************************/
BOOL CRBMemoryDC::Create(int cx, int cy, CDC* pDC)
{
	CBitmap bitmap;
	bitmap.CreateCompatibleBitmap(pDC, cx, cy);
	m_size.cx = cx;
	m_size.cy = cy;
	this->CreateCompatibleDC(pDC);
	this->SelectObject(&bitmap);
	return TRUE;
}


/***************************************************************************************
Function: 删除DC
Input:    NULL
Output:   BOOL
Description: 删除当前对象的资源
Return:   BOOL 删除成功标志
Others:   NULL
***************************************************************************************/
BOOL CRBMemoryDC::DeleteDC()
{
	if(!GetSafeHdc())
		return TRUE;
	CBitmap * pBitmap = GetCurrentBitmap();
	pBitmap->DeleteObject();
	return CDC::DeleteDC();
}



/***************************************************************************************
Function: 在指定区域绘图，并指定透明颜色
Input:    NULL
Output:   BOOL
Description: 在指定区域绘图，并指定透明颜色
Return:   void
Others:   NULL
***************************************************************************************/
void CRBMemoryDC::BitRgn(CRgn &rgn, COLORREF crTrans)
{
	int i=0, j=0;
	rgn.CreateRectRgn(0, 0, 0, 0);
	while(i < m_size.cx)
	{
		j = 0;
		while(j < m_size.cy)
		{
			if(GetPixel(i,j) - crTrans)
			{
				CRgn r;
				r.CreateRectRgn(i, j, i+1, j+1);
				rgn.CombineRgn(&rgn, &r, RGN_OR);
			}
			++j;
		}
		++i;
	}
}


/***************************************************************************************
Function: 在dc上进行透明贴图
Input:    NULL
Output:   void
Description: 
Return:   void
Others:   NULL
***************************************************************************************/
void CRBMemoryDC::BitTrans(int nXDst,int nYDst, int nWidth, int nHeight, 
						   CDC *pDC, int nXSrc, int nYSrc, COLORREF crTrans)
{
	CRBMemoryDC dcImage(nWidth, nHeight, pDC);   //创建临时DC
	CBitmap bmpMask;
	bmpMask.CreateBitmap(nWidth, nHeight, 1, 1, NULL);
	CDC  dcMask;                             
	dcMask.CreateCompatibleDC(pDC);
	dcMask.SelectObject(bmpMask);
	//将载入的位图复制到临时DC中
	dcImage.BitBlt(0, 0, nWidth, nHeight, this, nXSrc, nYSrc, SRCCOPY);  
	dcImage.SetBkColor(crTrans);   //设置临时DC的透明颜色
	//掩码DC的透明区域为白色，其他区域为黑色
	dcMask.BitBlt(0, 0, nWidth, nHeight, &dcImage, 0, 0, SRCCOPY);
	dcImage.SetBkColor(RGB(0,0,0));
	dcImage.SetTextColor(RGB(255,255,255));
	dcImage.BitBlt(0, 0, nWidth, nHeight, &dcMask, 0, 0,SRCAND);
	//目标DC的透明部分保持屏幕不变，其他部分变成黑色
	pDC->SetBkColor(RGB(255,255,255));
	pDC->SetTextColor(RGB(0,0,0));
	pDC->BitBlt(nXDst, nYDst, nWidth, nHeight, &dcMask, 0, 0, SRCAND);
	pDC->BitBlt(nXDst, nYDst, nWidth, nHeight, &dcImage, 0, 0, SRCPAINT);
}

