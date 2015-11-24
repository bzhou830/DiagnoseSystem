#include "StdAfx.h"
#include "Layer.h"
#include "ImgLayer.h"
#include "ImgInfo.h"


CLayer::CLayer(void)
{

}


CLayer::~CLayer(void)
{
}


CLayer* CLayer::Create(int nType)
{
	switch (nType)
	{
		case IMG_LAYER:
			return new CImgDataLayer;   //创建图像数据显示层
		case IMG_INFO_LAYER:
			return new CImgInfoLayer;    //创建图像信息显示层
		default:
			return NULL;
	}
}