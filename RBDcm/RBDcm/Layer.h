#pragma once
#include "BaseStruct.h"


//含有抽象函数的类叫做抽象类
class CLayer
{
public:
	int m_nStatus;
	static CLayer* Create(int nType);                   //创建图层
	//鼠标及显示事件响应
	virtual void SetLayerImgData(Mat img) = 0;          // 设置层图像数据
	virtual sOneImgInfo GetLayerInfoData() = 0;            // 读取图像信息数据
	virtual void SetLayerInfoData(sOneImgInfo info) = 0;   // 设置图像信息数据
	virtual void OnDraw(CDC* pDC, CRect rect) = 0;                           //纯虚函数（抽象函数）: 无函数体
	virtual void OnLButtonDown(UINT nFlags, CPoint point) = 0;   //鼠标左键按下消息
	virtual void OnLButtonUp(UINT nFlags, CPoint point) = 0;     //鼠标左键抬起消息
	
public:
	CLayer(void);
	~CLayer(void);
};

