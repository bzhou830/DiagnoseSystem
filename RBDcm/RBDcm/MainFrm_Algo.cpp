#include "stdafx.h"
#include "MainFrm.h"
#include "Global.h"

void RemoveEx(std::vector<std::vector<cv::Point> > &contours)
{
	if(contours.size() <= 2)
		return;
	std::vector<cv::Point> remain[2];
	
	int area1 = contourArea(contours[0]);
	int area2 = contourArea(contours[1]);
	if(area1 > area2)
	{
		remain[0] = contours[0];
		remain[1] = contours[1];
	}
	else
	{
		remain[0] = contours[1];
		remain[1] = contours[0];
	}
		
	for(int i=2; i<contours.size(); i++)
	{
		if(contourArea(contours[i]) > area1)
		{
			area2 = area1;
			area1 = contourArea(contours[i]);
		}
	}
}

Mat GetTemplate(Mat& src)
{
	Mat rt;
	cv::Rect rect;
	bitwise_and(Global::maskcicle, src, rt);
	floodFill(rt, cv::Point(0, 0), Scalar(255, 255, 255), &rect, 5, 5);
	//ȥ���м��һЩ���ø���ϸ��
	Mat element = getStructuringElement(MORPH_ELLIPSE, Size( 2*2 + 1, 2*2+1 ), Point(2, 2));
	dilate(rt, rt, element);
	bitwise_not(rt, rt);
	cv::threshold(rt, rt, 250, 255, 0);  //��ֵ��
	//�ҵ��Ƚϴ������������
	std::vector<std::vector<cv::Point> > contours;		//�߽�㼯��
	std::vector<cv::Vec4i> hierarchy;					//�߽�
	findContours(rt.clone(), contours, hierarchy, 
		CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
	//std::vector<std::vector<cv::Point> >::const_iterator it = contours.begin();
	//�������������result�������Ľ�����ڲ�Ϊ��ɫ���ⲿΪ��ɫ
	drawContours(rt, contours, -1, Scalar(255), CV_FILLED, 1, hierarchy);
	return rt;
}


//����ʹ��
void CMainFrame::OnTest()
{
	clock_t start, finish;
	//2016.10.26 �ı䴰��λ��ȡͼ�񣬵õ�ģ��
	sOneImg img = m_pLUView->GetOneImg();
	Mat src = img.src_u16;
	Mat mod = img.masking;
	
	
/*
	//����һ��ԲȦ��ģ��
	Mat maskcicle = Mat(Size(512,512), CV_8UC1, Scalar(255,255,255));
	circle(maskcicle, cv::Point(255,255), 240, Scalar(0,0,0), -1);
	//Mat maskcicle = Mat::zeros(512, 512, CV_8UC1);
	//circle(maskcicle, cv::Point(255, 255), 250, Scalar(255, 255, 255), -1);
	sOneImg img = m_pRBView->GetOneImg();			//��ȡͼ��
	//ʹ��ԲȦģ��ȡ�����õĲ��֣��浽rt��
	Mat rt;cv::Rect rect;
	bitwise_or(maskcicle, img.pixle, rt);
	Mat mk1 = Mat::zeros( img.pixle.rows + 2, img.pixle.cols + 2, CV_8UC1 );
	medianBlur(rt,rt,3);
	floodFill(rt, mk1, cv::Point(255, 7), Scalar(0, 0, 0), &rect,
		5, 5, 8|FLOODFILL_MASK_ONLY|(255<<8));
	
	//cv::Rect rect;
	Mat tmp = img.pixle.clone();
	cv::imshow("ԭͼ", tmp);
	start = clock();

	//Mask��ԭͼ��Ҫ��һȦ
	Mat mask1 = Mat::zeros( img.pixle.rows + 2, img.pixle.cols + 2, CV_8UC1 );
	floodFill(img.pixle, mask1, cv::Point(165, 170), Scalar(0, 0, 0), &rect,
		5, 5, 8|FLOODFILL_MASK_ONLY|(255<<8));

	Mat mask = mask1( Range( 1, mask1.rows - 1 ), Range( 1, mask1.cols - 1 ) );
	cv::imshow("ˮ����1", mask);
	
	cv::threshold(img.pixle, img.pixle, 170.0, 255, THRESH_BINARY);		//��ֵ��
	//cv::imshow("��ֵ����", img.pixle);									//
	Mat maska = img.pixle.clone();										//
	Mat maskb = img.pixle.clone();
	floodFill(maska, cv::Point(0, 0), Scalar(255,255,255));
	floodFill(maskb, cv::Point(maskb.cols-1, maskb.rows-1), Scalar(255, 255, 255));
	//Mat maska = Mat::zeros( img.pixle.rows + 2, img.pixle.cols + 2, CV_8UC1 );
	//Mat maskb = Mat::zeros( img.pixle.rows + 2, img.pixle.cols + 2, CV_8UC1 );
	
	//�ӣ�0,0����ʼ����
	/*
	floodFill(img.pixle, maska, cv::Point(0, 0), Scalar(255,255,255), &rect,
		5, 5, 8|FLOODFILL_MASK_ONLY|(255<<8));

	//�ӣ�img.cols-1,img.rows-1����ʼ����
	floodFill(img.pixle, maskb, cv::Point(img.pixle.cols-1, img.pixle.rows - 1), Scalar(255,255,255), &rect,
		5, 5, 8|FLOODFILL_MASK_ONLY|(255<<8));
	bitwise_or(maska, maskb, maska);
	Mat mask = maska( Range( 1, maska.rows - 1 ), Range( 1, maska.cols - 1 ) );
	*/

	/*
	bitwise_or(maska, maskb, maska);
	//cv::imshow("ˮ����", maska);
	bitwise_not(maska, maska);
	
	//Mat mask = maska( Range( 1, maska.rows - 1 ), Range( 1, maska.cols - 1 ) );
	vector< vector<Point> > contours;
	
	//std::vector<cv::Vec4i> hi;
	cv::findContours(maska, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
	Mat dst = Mat::zeros(maska.rows, maska.cols, CV_8UC1);
	cv::drawContours(dst, contours, -1, Scalar(255, 255, 255), -1);		//�õ���ʵ��ȥ�ɰ�
	
	//*****�߽��޲������
	bitwise_and(dst, tmp, dst);											//�õ���ʵ����ͼ��
	cv::imshow("��ʵ��", dst);											//��ʾ��ʵ��ͼ��

	CSegmentOperat seg;
	//Mat t1 = seg.OstuSeg(dst);
	//Mat t2 = seg.MoMSeg(dst);
	//Mat t3 = seg.EntropySeg(dst);
	Mat t4;// = seg.IsodataSeg(dst);									//�ָ�õ� ���Ʒν��ͼ����Ĥģ��
	//cv::imshow("Ostu", t1);
	//cv::imshow("MoM",t2);
	//cv::imshow("Entropy",t3);
	
	//�����ﱨ����
	//bitwise_and(t4, tmp, dst);										//��ԭͼ�����λ������õ����Ʒν��ͼ��
	//cv::imshow("seg", t4);											//��ʾ�ָ�õ������Ʒν��ͼ��

	//����ͼ���ͼ����������ÿһ�����ƽ�ڽ�����ֵ����
	contours.clear();													//���ԭ�б߽�����

	std::vector<cv::Vec4i> hierarchy;									//�߽�
	
	findContours(dst.clone(), contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
	drawContours(dst, contours, -1, Scalar(255), CV_FILLED, 1, hierarchy);
	
	string str;
	for (int i = 0; i<contours.size(); ++i)					//��ÿһ�����ƽ��
	{
		cv::Rect rect = boundingRect( contours.at(i) );		//�ⲿ�����������
		
		//��ȡ�õ��ʸ״��͹�״��
		Mat Sag = m_ImgSerial.GetSagittal(rect.y + rect.height/2, fSlice);
		Mat Cor = m_ImgSerial.GetCoronal(rect.x + rect.width/2, fSlice);
		//ShowSagittal(rect.y + rect.height/2, rect.x + rect.width/2);
		//ShowCoronal(rect.x + rect.width/2, rect.y + rect.height/2);
		char tt[10];
		sprintf_s(tt,"%d",i);
		string abc(tt); 
		putText(dst, abc, Point(rect.x, rect.y),
			CV_FONT_HERSHEY_PLAIN, 0.7, Scalar(255, 255, 255), 1);
	}
	//��Ӿ���
	cv::imshow("Label", dst);
	
	
	//cv::findContours(t4, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
	
	//Mat tp = Mat::zeros(t4.rows, t4.cols, CV_8UC1);
	/*

	cv::drawContours(tp, contours, -1, Scalar(255, 255, 255), -1);
	cv::imshow("Contours",tp);
	bitwise_and(tp, tmp, dst);
	cv::imshow("ISOData", dst);
	*/

	//findContours(t4,);
	
	/*
	finish = clock();
	*/
	//MessageBox("","")

	/*Mat  et = getStructuringElement(MORPH_ELLIPSE, cv::Size(21,21));
	cv::morphologyEx(img.pixle,img.pixle,CV_MOP_ERODE,et);*/

	//cv::erode(img.pixle, img.pixle, et);

	/*cv::imshow("��ʴ��", img.pixle);*/	
	/*floodFill(img.pixle,cv::Point(160,160),Scalar(0,0,0));
	cv::imshow("ˮ����", img.pixle);*/

	/*vector< vector<Point> > contours;
	std::vector<cv::Vec4i> hi;
	cv::findContours(img.pixle, contours, CV_RETR_TREE, CV_CHAIN_APPROX_NONE);
	Mat dst = Mat::zeros(img.pixle.rows, img.pixle.cols, CV_8UC1);*/
	
	//������ͨ�����
	//contourArea();
	
	//cv::drawContours(dst, contours, -1, Scalar(255, 255, 255));
	//cv::namedWindow("����", CV_WINDOW_AUTOSIZE);					//��ʾ��ֵ��ͼ��
	//cv::imshow("����",dst);										//
}