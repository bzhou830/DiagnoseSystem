/***************************************************************************************
  Copyright @ Robin 2015-2025
  File name:  IconButton.cpp
  Author: Robin   Version: V1.0   Date:2015.09.05 
  Description: Owner Draw Button Calss.
  Others:         
***************************************************************************************/
#include "StdAfx.h"
#include "IconButton.h"


CIconButton::CIconButton(void)
{
}


CIconButton::~CIconButton(void)
{
}


/***************************************************************************************
Function: ����Icon button���棨�Ը����DrawItem����д��
Input:    LPDRAWITEMSTRUCT �Ի�Ŀ��ṹ��ָ��
Output:   void
Description:    ���Ƶ�Icon Button����Ԥ���õ�24X24�ĳߴ���Ƶ�
			    Icon Button�Ľ�����ɫΪ����ɫ
Return:   void
Others:   NULL
***************************************************************************************/
void CIconButton::DrawItem(LPDRAWITEMSTRUCT lpDIS)
{
	CDC dc;
	dc.Attach(lpDIS->hDC);
	CRect rc = lpDIS->rcItem;
	dc.FillSolidRect(rc, RGB(236,236,236));
	if (lpDIS->itemState & ODS_SELECTED)  //Button Clicked Set Rect offset
		rc.OffsetRect(2,2);
	//Draw Icon
	int n = rc.Width();
	CRect rt = rc;
	rt.right = n;
	rt.DeflateRect((n-20)/2, 4);
	::DrawIconEx(dc.GetSafeHdc(), rt.left, rt.top, m_hIcon, 20, 20, 0, 0, DI_NORMAL);
	//Draw Text
	rt.CopyRect(rc);
	//rt.left += 24;
	rc.bottom += 24;
	CString str;
	GetWindowText(str);
	CFont font;
	font.CreateFont(12,0,0,0,FW_MEDIUM,FALSE,FALSE,0,ANSI_CHARSET,OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,DEFAULT_PITCH | FF_SWISS, "����");
	dc.SelectObject(font);
	dc.SetTextColor(GetSysColor(COLOR_WINDOWTEXT));
	dc.DrawText(str, rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	//Draw Edge
	if(lpDIS->itemState & ODS_SELECTED)
		dc.DrawEdge(&lpDIS->rcItem, BDR_SUNKENOUTER, BF_RECT);
	else
		dc.DrawEdge(&lpDIS->rcItem, BDR_RAISEDOUTER, BF_RECT);
	if (lpDIS->itemState & ODS_FOCUS)
	{
		rt.CopyRect(rc);
		rt.DeflateRect(2, 2);
		dc.DrawFocusRect(rt);
	}
	dc.Detach();
}
