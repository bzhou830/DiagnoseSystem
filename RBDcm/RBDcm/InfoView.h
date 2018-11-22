#pragma once
#include "ImgSerial.h"
#include "Type.h"



// CInfoView 视图
class CInfoView : public CListView
{
	DECLARE_DYNCREATE(CInfoView)
public:
	sOneImg img;

protected:
	CInfoView();							// 动态创建所使用的受保护的构造函数
	virtual ~CInfoView();

public:
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();			//初始化函数
	void SetImgData(sOneImg src);			//向对象中设置数据
	void ShowData(void);					//显示数据
	void DeleteAll(void);					//删除所有
};


