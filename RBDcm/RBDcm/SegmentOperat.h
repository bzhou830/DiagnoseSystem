#pragma once
#include "ImgSerial.h"

class CImgSerial;

class CSegmentOperat
{
public:
	CSegmentOperat(void);
	~CSegmentOperat(void);
	bool IsodataSeg(Mat &src, int n = 120);						//��ֵ�����ָ�
	Mat OstuSeg(Mat src);										//Ostu�ָ�
	Mat EntropySeg(Mat src);									//����طָ�
	Mat MoMSeg(Mat src);										//�������ַ�
	Mat GetLungArea(Mat src);									//ȡ�÷�ʵ������
	Mat GetRealLung(Mat src);									//��ȡ��ʵ��
	Mat RegionGrow(Mat src, Point2i pt, int th = 5);			//����������ȱʡ��ֵΪ5
	Mat GetObjectCenter(Mat src, vector<Point2i>& vcPoint);		//��ȡ��Ƭ��ʵ�ʵ�����
	void RegionGrow3D(CImgSerial &img, Point3i pt,int th=5);	//3ά������������
};

