/**********************************************************************************
  Copyright @ Robin 2015-2025
  File name:  IconButton.h
  Author: Robin   Version: V1.0   Date:2015.09.05 
  Description: Owner Draw Button Calss.
  Others:         
***********************************************************************************/
#pragma once
#include "afxwin.h"

class CIconButton : public CButton
{
	HICON m_hIcon;
public:
	void SetIcon(UINT nIcon)
	{
		m_hIcon = AfxGetApp()->LoadIcon(nIcon);
	}
public:
	CIconButton(void);
	~CIconButton(void);
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDIS);
};
