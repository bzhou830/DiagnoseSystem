#include "stdafx.h"
#include "MainFrm.h"
#include "TransVideoDlg.h"
#include "Histogram1d.h"
#include "GMM/GMM.h"
#include "GMM/KMeans.h"
#include <fstream>

using namespace std;

/*===========================================================================*
* ��֤��С��������Ƿ���Ͻ�ڵĴ�������
* ����Ϊ��С������εĳ��������5�����ص�
*============================================================================*/
bool CMainFrame::verifySizes(RotatedRect mr)
{
	Size2f sz  = mr.size;
	float width = sz.width;
	float height = sz.height;
	float rat = max(width, height) / min(width, height);						//��߱�
	if (width <= 5 || height <= 5)												//����С��2���أ���Ϊ��ϸС����
		return false;
	if (rat > 3)																//������ز��ɱ���������Ѫ��
		return false;
	return true;
}


/*===========================================================================*
* ��֤��������
* �������Ϊ�����е���Чͼ�����ظ�������ռ���������һ������
*============================================================================*/
bool CMainFrame::verifySizes(Mat m)
{
	int area = m.cols * m.rows;													//�����������
	int valid = 0;
	for (int k=0; k<m.rows; k++)												//��ȡ��ʵ������	
		for (int n=0; n<m.cols; n++)
			if (m.at<uchar>(k,n) != 0) valid++;
	if (valid*2 < area)
		return false;
	return true;
}


/*===========================================================================*
* ��ȡ���ƽ�����򣬽���������洢��ͼ����
* �����ƽ�������д�ѡ
*============================================================================*/
void CMainFrame::OnBtnExtract()
{
	int size = m_ImgSerial.m_aip.size();												//aip֮���ͼ��
	
	for (int i = 0; i < size; i++)
	{
		Mat tmp = m_ImgSerial.m_aip[i].clone();											//���ұ߽��Ӱ��ͼ���ԭ����
		Mat watch = tmp.clone();
		CImgSerial::cntr.clear();
		CImgSerial::hy.clear();
		findContours(tmp, CImgSerial::cntr, CImgSerial::hy,						
			CV_RETR_EXTERNAL,
			CV_CHAIN_APPROX_NONE);
		
		vector<vector<Point> >::iterator it = CImgSerial::cntr.begin();
		vector<RotatedRect> rects;
		while (it != CImgSerial::cntr.end())											//ȥ�����Բ�����Ҫ�������
		{
			RotatedRect mr = minAreaRect(Mat(*it));										//��С�������
			if( !verifySizes(mr))
				it = CImgSerial::cntr.erase(it);										//���ͳߴ�Ҫ��ɾ���ñ߽�
			else
			{
				++it;
				rects.push_back(mr);													//����Ҫ��ı߽���δ���
			}
		}

		it = CImgSerial::cntr.begin();
		Rect rect;
		int j = 0;
		while (it != CImgSerial::cntr.end())
		{
			rect = boundingRect((Mat)*it);												//���ƽ�ڰ������
			
			Mat ss = m_ImgSerial.m_aip[i](Range(rect.y, rect.y + rect.height),
				Range(rect.x, rect.x + rect.width));									//ȡ����Ӧ���ǿ�ͼ��
					
			if (!verifySizes(ss))														//�Ӷϲ�ͼ���Ͻ�����֤
			{
				it++;
				continue;
			}
			

//�����е������ĵ���Ϊ�����㣬�����ʣ�
/*
			int curVal = m_ImgSerial.m_aip[i].at<uchar>(rect.y, rect.x);
			int row = rect.x;
			int col = rect.y;

			//�ҵ������������ĵ㣬�Ӹõ�����			
			for(int x = rect.x; x < rect.x + rect.height; x++)
			{
				for (int y=rect.y; y<rect.y + rect.width; y++)
				{
					if(m_ImgSerial.m_aip[i].at<uchar>(x,y) > curVal)
					{
						curVal = m_ImgSerial.m_aip[i].at<uchar>(x,y);
						row = x;
						col = y;
					}
				}
			}
*/

//ѡ��������е�
			int row = rect.y + rect.width/2;
			int col = rect.x + rect.height/2;

			Mat cor = Global::u16Tou8(m_ImgSerial.GetCoronal(col, fSlice));				//�õ�ʸ״��ͼ��
			Mat sag = Global::u16Tou8(m_ImgSerial.GetSagittal(row, fSlice));			//�ȵ���״��ͼ��

			
			//��ȡ�߶�,��λ�ڸ������λ��
			int height = (i+1) * m_aipFrameNum * fSlice / m_ImgSerial.GetImageNum()
				- (m_aipFrameNum / 2) * fSlice / m_ImgSerial.GetImageNum();

			//��������֤ (row, height) - ��״�� (col, height) - ʸ״��
			Mat mk_cor = cor(Range(height-rect.width, height+rect.width), Range(row-rect.width, row+rect.width));
			Mat mk_sag = sag(Range(height-rect.width, height+rect.width), Range(col-rect.height, col+rect.height));
			threshold(mk_cor, mk_cor, Global::ThresholdNum, 255, THRESH_TOZERO);
			threshold(mk_sag, mk_sag, Global::ThresholdNum, 255, THRESH_TOZERO);
			//int floodFill(InputOutputArray image, InputOutputArray mask, Point seedPoint,Scalar newVal, Rect* rect=0, Scalar loDiff=Scalar(), Scalar upDiff=Scalar(), int flags=4 )
			//Mat msk1 = Mat::zeros(cv::Size(cor.cols+2, cor.rows+2), CV_8UC1);
			//Mat msk2 = Mat::zeros(cv::Size(sag.cols+2, sag.rows+2), CV_8UC1);
			//cv::Rect rt;
			//floodFill(cor, msk1, cv::Point(row, height), Scalar(255), &rt, 10, 10, 8 | FLOODFILL_MASK_ONLY | 255<<8);
			//floodFill(sag, msk2, cv::Point(col, height), Scalar(255), &rt, 10, 10, 8 | FLOODFILL_MASK_ONLY | 255<<8);
			
			//cor.clone();
			//Mat msk2 = sag.clone();
			
			//floodFill(msk1, cv::Point(row, height), Scalar(255), &rt, 10, 10, 8);
			//floodFill(msk2, cv::Point(col, height), Scalar(255), &rt, 10, 10, 8);

			string filename = "E:\\training\\" + file + "\\" + std::to_string((long long)i);
			filename += "\\" + to_string((long long)j++);									//һ�����ƽ�ڣ�·��
								
			Global::CreateMuliteDirectory(filename.c_str());								//����·��
			imwrite(filename + "\\A.jpg", ss);											//�ϲ�ͼ
			imwrite(filename + "\\B.jpg", mk_cor);										//ʸ״��
			imwrite(filename + "\\C.jpg", mk_sag);										//��״��
			ofstream fout(filename + "\\pos.txt");
			//fout.open(, ios::in | ios::out);
			fout<<row<<","<<col;
			fout.close();
			
			it++;
		}
	}
}


/*===========================================================================*
* ��ʾƽ���ܶ�ͶӰ���ͼ
*============================================================================*/
void CMainFrame::OnBtnAIP()
{
	if(m_AipDlg != NULL)
	{
		m_AipDlg->SetImgSerial(m_ImgSerial.m_aip.begin(), m_ImgSerial.m_aip.size());
		m_AipDlg->ShowWindow(SW_NORMAL);
		return;
	}
	m_AipDlg = new CAIPDlg();
	m_AipDlg->Create(MAKEINTRESOURCE(IDD_DLG_AIP), NULL);
	CRect rct;
	m_AipDlg->GetWindowRect(&rct);
	m_AipDlg->MoveWindow(630, 62, rct.Width(),rct.Height());	//�ƶ����Ҳ෽��Աȹ۲�
	m_AipDlg->SetImgSerial(m_ImgSerial.m_aip.begin(), m_ImgSerial.m_aip.size());
	m_AipDlg->ShowWindow(SW_NORMAL);
}

void CMainFrame::OnBtnZoom()
{
	ShowZoom();
}

/*===========================================================================*
* �鿴ԭͼ
*============================================================================*/
void CMainFrame::OnBtnSrcimg()
{
	CSrcShowDlg dlg;
	dlg.SetImgData(m_ImgSerial.GetCurrentMatImg().src_u16.clone());
	dlg.DoModal();
}

/*===========================================================================*
* ����ͼ������
*============================================================================*/
void CMainFrame::OnBtnPlayimgs()
{
	if (m_PlayDlg != NULL)											//�������Ѿ������ڴ��У�����ʾ�Ϳ����ˣ����ⴴ���������
	{
		m_PlayDlg->SetImgSerial(m_ImgSerial.GetBegin(), m_ImgSerial.GetImageNum());
		m_PlayDlg->ShowWindow(SW_NORMAL);
		return;
	}
	m_PlayDlg = new CPlaySeriesDlg();
	//CLUView *pView = ((CMainFrame*)AfxGetMainWnd())->m_pLUView;
	m_PlayDlg->Create(MAKEINTRESOURCE(IDD_PLAY_SERI), NULL);
	m_PlayDlg->SetImgSerial(m_ImgSerial.GetBegin(), m_ImgSerial.GetImageNum());
	m_PlayDlg->ShowWindow(SW_NORMAL);
}

//����XML����
void CMainFrame::OnBtnLoadxml()
{
	CFileDialog dlg(TRUE);														//�ļ��򿪶Ի���
	dlg.m_ofn.lpstrTitle = "��Xml�ļ�";											//ָ�����ļ��Ի�������
	//�ļ�������
	dlg.m_ofn.lpstrFilter = "xml Files(*.xml)\0*.xml\0All Files(*.*)\0*.*\0\0";  
	if (IDCANCEL == dlg.DoModal())												//�����ļ��򿪶Ի���ѡ��ȡ����ֱ�ӷ���
		return;
	CString strFileName = dlg.GetPathName();									//��ȡ�ļ�·��+�ļ���
	m_ImgSerial.LoadXml(strFileName);											//����xml�ļ�
	sOneImg info = m_ImgSerial.GetCurrentMatImg();
	m_pInfoView->SetImgData(info);												//����Ϣ��ʾ���ڴ����ڵ���Ϣ
}

//��ʾ��һ�ϲ�
void CMainFrame::lastimg()
{
	sOneImg img = m_ImgSerial.LastMatImg();
	m_pLUView->SetImgData(img);													//����1�Ŵ�����
	CString str;
	str.Format("��ǰͼ����š�%d��", m_ImgSerial.GetCurrentNum());
	SetStateText(2, str);
}

//��ʾ��һ�ϲ�
void CMainFrame::nextimg()
{
	sOneImg img = m_ImgSerial.NextMatImg();
	m_pLUView->SetImgData(img);													//����1�Ŵ�����
	CString str;
	str.Format("��ǰͼ����š�%d��", m_ImgSerial.GetCurrentNum());
	SetStateText(2, str);
}

/*===========================================================================*
* ��˹���ģ��, �ָ����ƽ��
*============================================================================*/
void CMainFrame::OnBtnGMM()
{
	Mat src;
	int size = m_ImgSerial.m_aip.size();
	double *data = new double[512 * 512 * size];								//����
	int index = 0;
	//ͳ�Ƴ��������еķ�������
	for (int i=0;i<size;i++)
	{
		for (int m=0; m<512; m++)												//��ȡ��ʵ������
		{	
			for (int n=0; n<512; n++)
			{
				if (m_ImgSerial.m_mskaip[i].at<uchar>(m, n) != 0)
					data[index++] = m_ImgSerial.m_aip[i].at<uchar>(m, n);
			}
		}
	}

	if (Global::isDumpCsv)														//��ȡ�����ļ��趨ֵ���ж��Ƿ񵼳�csv�ļ�
	{
		ofstream savefile("data.csv");
		for (int i=0;i<index-1;i++)
			savefile<<data[i]<<",";
		savefile<<data[index-1];
		savefile.close();
	}

	GMM *gmm = new GMM(1, 6);													//һά���ݣ�2�����ģ��
	gmm->Train(data, index);
	gmm->SetEndError(0.01);
	double g1[3] = {gmm->m_priors[0], gmm->m_means[0][0], gmm->m_vars[0][0]};	//������˹ģ�͵Ĳ���
	double g2[3] = {gmm->m_priors[1], gmm->m_means[1][0], gmm->m_vars[1][0]};
	double g3[3] = {gmm->m_priors[2], gmm->m_means[2][0], gmm->m_vars[2][0]};
	double g4[3] = {gmm->m_priors[3], gmm->m_means[3][0], gmm->m_vars[3][0]};
	double g5[3] = {gmm->m_priors[4], gmm->m_means[4][0], gmm->m_vars[4][0]};
	double g6[3] = {gmm->m_priors[5], gmm->m_means[5][0], gmm->m_vars[5][0]};
	delete gmm;
	int th = max(max(g1[1], g2[1]), g3[1]);
	Mat mmt = getStructuringElement(MORPH_ELLIPSE, Size(3, 3), Point(1, 1));
	for (int i=0;i<size;i++)
	{
		threshold(m_ImgSerial.m_aip[i], m_ImgSerial.m_aip[i], th, 255, THRESH_TOZERO);
		//morphologyEx(m_ImgSerial.m_aip[i], m_ImgSerial.m_aip[i], MORPH_OPEN, mmt);//����󶼱����Բ�Σ�����
	}
	Global::ThresholdNum = th;
	/*
	for (int i=0; i<size; ++i)
	{
		GMM *gmm = new GMM(1, 2);												//һά���ݣ�2�����ģ��
		src = m_ImgSerial.m_mskaip[i];
		bitwise_and(m_ImgSerial.m_mskaip[i],m_ImgSerial.m_aip[i],m_ImgSerial.m_aip[i]);
		for (int m=0; m<512; m++)												//��ȡ��ʵ������
		{	
			for (int n=0; n<512; n++)
			{
				if (m_ImgSerial.m_mskaip[i].at<uchar>(m,n) != 0)
					data[index++] = m_ImgSerial.m_aip[i].at<uchar>(m, n);
			}
		}

		if (index >= 500)
		{
			gmm->Train(data, index);
			double g1[3] = {gmm->m_priors[0], gmm->m_means[0][0], gmm->m_vars[0][0]};
			double g2[3] = {gmm->m_priors[1], gmm->m_means[1][0], gmm->m_vars[1][0]};
			//���㽻��
			int th = (int)(gmm->m_means[0][0] + 3* sqrt(gmm->m_vars[0][0]));//Global::GuassAcross(g1, g2);
			Mat low, high;
			threshold(m_ImgSerial.m_aip[i], low, th, 255, THRESH_TOZERO); //��ֵ��ͼ��
			threshold(m_ImgSerial.m_aip[i], high, 130, 255, THRESH_TOZERO);
			m_ImgSerial.m_aip[i] = low;
			th = 0;
		}
		delete gmm;
		index = 0;
	}
	*/
	delete[] data;
}

void CMainFrame::OnBtnHistimg()
{
}

/*ͼ������ת������Ƶ*/
void CMainFrame::OnBtnCamera()
{
	CTransVideoDlg dlg(m_ImgSerial.m_vImgSerial);
	dlg.DoModal();
}

void CMainFrame::OnBtnEntropy()
{
	AfxMessageBox("��û����ֲ�����");
	std::ifstream file("c:\\abcd.csv", ifstream::in);	//��ȡcsv�ļ�
	string line, path, classlabel;
	Mat dst = Mat::zeros(40, 10, CV_32F);
	vector<int> row(12, 1);
	for (int k=0;k<28;++k)
		row.push_back(0);
	int i = 0, j = 0;
	while (getline(file, line))
	{
		stringstream liness(line);
		while (getline(liness, path, ','))
		{
			dst.at<float>(i,j) = static_cast<float>(CType::string2Double(path));
			j++;
		}
		j = 0, i++;
	}
	Mat fe(row);
	FileStorage fs("C:\\ttt.xml", FileStorage::WRITE);
	fs << "TrainingData" << dst;
	fs << "classes" << fe;
	fs.release();
}

void CMainFrame::OnBtnOtsu()
{
	CSegmentOperat seg;
	sOneImg src = m_pLUView->GetOneImg();
	cv::Mat m_dstImage = seg.OstuSeg(src.masking.clone()).clone();
	sOneImg info = m_ImgSerial.GetCurrentMatImg();
	info.masking = m_dstImage.clone();
	m_pRUView->SetImgData(info);
}



void CMainFrame::OnBtnNudoul()
{
	//(m_pMainFrm->m_pClassier)->SetImgData(img);
	sOneImg img = (m_pLDView)->GetOneImg();				//��ȡ��ڴ�����ʾ��ͼ�����ƽ�ڵĻҶ�ͼ��
	std::vector<std::vector<cv::Point> > contours;			//�߽�㼯��
	std::vector<cv::Vec4i> hierarchy;						//�߽�

	cv::medianBlur(img.masking, img.masking,3);
	findContours(img.masking.clone(), contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
	drawContours(img.masking, contours, -1, Scalar(255), CV_FILLED, 1, hierarchy);

	sOneImg simg;	//��ʵ�ʴ����е�ͼ��
	(m_pRUView)->GetSegRealLungs(simg);
	bitwise_and(simg.masking, img.masking, img.masking);
	COneImgFeatures features;
	features.SetMatImgAndContours(img.masking, contours);
	features.CalculateFeatures();

	//namedWindow("aaa",CV_WINDOW_AUTOSIZE);				//��ʾ�ָ��Ľ��
	//imshow("aaa",img.pixle);
}


//״̬���������ְ�ť��Ӧ
void CMainFrame::OnBtnMom()					//����
{
	sOneImg img;							//��ʵ�ʴ����е�ͼ��
	(m_pRUView)->GetSegRealLungs(img);		//
	medianBlur(img.masking, img.masking, 3);
	(m_pLDView)->SegNodules(img);
}

//״̬����ֵ�����㷨��Ӧ
void CMainFrame::OnBtnMean()
{
	sOneImg img;
	(m_pRUView)->GetSegRealLungs(img);
	medianBlur(img.masking, img.masking, 3);
	(m_pLDView)->SegNodulesMean(img);
}

//����ѡ��ͼ��
void CMainFrame::OnSaveimg()
{
	CFileDialog dlg(FALSE);
	dlg.m_ofn.lpstrTitle = "�ļ�����";
	dlg.m_ofn.lpstrFilter = "Bmp Files(*.bmp)\0*.bmp\0All Files(*.*)\0*.*\0\0";
	dlg.m_ofn.lpstrDefExt = "bmp";
	sOneImg img = m_pLUView->GetOneImg();
	if (dlg.DoModal() == IDOK)
		imwrite((LPCTSTR)dlg.GetPathName(), img.masking);
}

//��ͼ���������м��
void CMainFrame::OnBtnTest()
{
}


//����ѵ��
void CMainFrame::OnTraining()
{
	/*
	CFileDialog dlg(TRUE);
	dlg.m_ofn.lpstrTitle = "��ѵ�������ļ�";
	dlg.m_ofn.lpstrFilter = "XML Files(*.XML)\0*.XML\0All Files(*.*)\0*.*\0\0";
	if (IDCANCEL == dlg.DoModal())						//�����ļ��򿪶Ի���ѡ��ȡ����ֱ�ӷ���
		return;
	CString strFileName = dlg.GetPathName();			//��ȡ�ļ�·��+�ļ���
	string str(strFileName.GetBuffer());
	cv::FileStorage fs;
	fs.open(str, FileStorage::READ);
	Mat SVM_TrainingData;
	Mat SVM_Classes;
	fs["TrainingData"] >> SVM_TrainingData;
	fs["classes"] >> SVM_Classes;
	CvSVMParams SVM_params;
	SVM_params.svm_type    = CvSVM::C_SVC;
	SVM_params.kernel_type = CvSVM::RBF;
	SVM_params.term_crit   = cvTermCriteria(CV_TERMCRIT_ITER, 100, 1e-6);
	try
	{
		m_svmClassifier.train(SVM_TrainingData, SVM_Classes, Mat(), Mat(), SVM_params);
	}
	catch(CException *e)
	{
		TCHAR szError[255];
		e->GetErrorMessage(szError, 255);
		CString str;
		str.Format("ѵ���������ˣ��쳣ԭ��\r\n %s", szError);
		MessageBox(str, "RBDcm��ʾ��", MB_OK);
		return;
	}
	MessageBox("������ѵ�����������^_^", "RBDcm��ʾ��", MB_OK);
	*/
}


