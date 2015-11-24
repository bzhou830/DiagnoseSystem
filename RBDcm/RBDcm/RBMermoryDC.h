/**********************************************************************************
  Copyright @ Robin 2015-2025
  File name:  RBMemroyDC.h
  Author: Robin   Version: V1.0   Date: 2015.11.05 
  Description: This class contain operation ablout CMemDC
  Others:         
***********************************************************************************/
#ifndef __RBMEMORYDC_H
#define __RBMEMORYDC_H

class CRBMemoryDC : public CDC
{
private:
	CSize m_size;   //内存DC的尺寸
public:
	CRBMemoryDC()
	{
		m_size.cx = m_size.cy = 0; 
	}
	
	CRBMemoryDC(UINT nBitmapID, CDC* pDC = NULL)
	{
		LoadBitmap(nBitmapID, pDC);
	}
	
	CRBMemoryDC(LPCSTR szBitmapFile, CDC *pDC = NULL)
	{
		LoadBitmap(szBitmapFile, pDC);
	}
	
	CRBMemoryDC(int cx, int cy, CDC* pDC = NULL)
	{
		Create(cx, cy, pDC);
	}
	
	~CRBMemoryDC()
	{
		DeleteDC();
	}

	//加载位图资源
	BOOL LoadBitmap(UINT nBitmapID, CDC *pDC = NULL);
	BOOL LoadBitmap(LPCSTR szBitmapFile, CDC *pDC = NULL);
	BOOL Create(int cx, int cy, CDC* pDC = NULL);
	BOOL DeleteDC();
	void BitRgn(CRgn &rgn, COLORREF crTrans);
	void BitTrans(int nXDst,int nYDst, int nWidth, int nHeight, CDC *pDC, int nXSrc, int nYSrc, COLORREF crTrans);

public:
	inline int Width(){      //获取内存DC宽度
		return m_size.cx;
	}
	
	inline int Height(){     //获取内存DC高度
		return m_size.cy;
	}
	
};

#endif  //__MEMORYDC_H

