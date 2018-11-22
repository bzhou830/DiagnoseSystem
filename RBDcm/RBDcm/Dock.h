#pragma once


// CDock

class CDock : public CDockablePane
{
	DECLARE_DYNAMIC(CDock)

public:
	CDock();
	virtual ~CDock();

protected:
	DECLARE_MESSAGE_MAP()
	CListBox m_listBox;
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
};


