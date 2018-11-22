/**********************************************************************************
Copyright @ Robin 2015-2025
  �ļ���:  MatImg.cpp
  ��  ��:  Robin
  ��  ��:  ����׼��.dcm�ļ�(����dicom3.0��׼)��ȡ��OpenCV֧�ֵ�Mat�����������ļ�
          ʹ��vector�Զ�ȡ������ݽ��б��档
  ��  ��:  2015.11.05
  ��  ��:  ��Ӽ���xml�����ļ����ܺ���
***********************************************************************************/
#include "StdAfx.h"
#include "ImgSerial.h"
#include "Global.h"

//��ͬ������ʽ��dcm�ļ�UID����
const string losslessTransUID = "1.2.840.10008.1.2.4.70";						//JPEG Lossless,Non-Hierarchical
const string lossTransUID     = "1.2.840.10008.1.2.4.51";						//JPEG Extended
const string losslessP14      = "1.2.840.10008.1.2.4.57";						//JPEG Lossless
const string lossyP1          = "1.2.840.10008.1.2.4.50";						//JPEG Baseline
const string lossyRLE         = "1.2.840.10008.1.2.5";							//RLE Lossless

char			CImgSerial::flag		= 0x00;									//ͼ�����״̬
HANDLE			CImgSerial::m_hMutex	= NULL;									//������������ͬ��
CStringArray	CImgSerial::csa[8];												//ͼ��·��
list<sOneImg>	CImgSerial::li[8];												//ͼ����ʱ�洢
list<sOneImg>	CImgSerial::m_vImgSerial;										//ͼ��洢
vector<Mat>		CImgSerial::m_aip;												//AIP��ͼ��
vector<Mat>		CImgSerial::m_mskaip;											//AIPͼ���Ӧmask
vector<vector<Point> >	CImgSerial::cntr(5000);
vector<Vec4i>			CImgSerial::hy(20);


/*========================================================================================
* lpszPathName Dicomͼ���ȫ·��
==========================================================================================*/
CImgSerial::CImgSerial(CString lpszPathName)
{
	LoadDcm(lpszPathName);
}

/*========================================================================================
* ����Dcmͼ�񣬹�������ⲿ���ýӿ�,lpszPathName Dicomͼ���ȫ·��
==========================================================================================*/
BOOL CImgSerial::LoadDcm(CString lpszPathName)
{
	//ͼ�����ת��, ͼ����ص�ʱ������Ϣȫ������
	//��һ��ͼ��������ӵ�������,ָ��������itrָ�����еĿ�ͷ
	m_vImgSerial.push_back(Dcm2MatGroup(lpszPathName));										
	m_itr = m_vImgSerial.begin();											
	return TRUE;
}

//ͼ�����е����򣬰���zposition���������������
bool CompareRules(sOneImg& X, sOneImg& Y)
{
	if (X.fSliceLocation > Y.fSliceLocation)
		return true;
	else
		return false;
}

/*=====================================================================================*
�̼߳��غ���
*======================================================================================*/
DWORD WINAPI CImgSerial::ProcLoad(LPVOID lp)
{
	int num = (int)lp;
	for (int i=0;i<csa[num].GetSize();i++)
	{
		li[num].push_back(Dcm2MatGroup(csa[num].GetAt(i)));
	}
	WaitForSingleObject(m_hMutex, INFINITE);
	flag |= (0x01 << num); 
	ReleaseMutex(m_hMutex);
	return 0;
}

/*=====================================================================================*
�̼߳��غ���, ��̬�����������̣߳������߳�
*======================================================================================*/
BOOL CImgSerial::LoadDcms(CStringArray* paths)
{
	int cnt = paths->GetSize();

	for (int i=0; i<cnt; i++)
		csa[i%ThreadNum].Add(paths->GetAt(i));

	m_hMutex = CreateMutex(NULL, FALSE, NULL);									//��������������������ͬ��
	for (int i=0;i<ThreadNum;i++)
	{
		HANDLE han = CreateThread(NULL, 0, ProcLoad, (LPVOID)i, 0, NULL);		//�����߳�������ʾ��ȡ����
		CloseHandle(han);
	}

	while (true)																//�ȴ��߳�ִ�����
	{
		WaitForSingleObject(m_hMutex, INFINITE);
		if (flag == -1) break;
		ReleaseMutex(m_hMutex);
	}
	return TRUE;
}

/*=====================================================================================*
�ϲ����ص�ͼ����Դ
*======================================================================================*/
void CImgSerial::Merge()
{
	m_vImgSerial.clear();
	for (int i=0;i<ThreadNum;i++)
	{
		m_vImgSerial.splice (m_vImgSerial.begin(), li[i]);
	}
}

/***************************************************************************************
Function: Dcmͼ��ת�������״���Ķ���Matͼ�� + ������Ϣ��
Input:    LPCTSTR lpszPathName dcm ͼ���ȫ·��
Output:   dcmת�����Mat + info�ṹ����
Description: ʹ��DCMTK�⺯����ԭDCMͼ��ת��Ϊ8bit�Ҷ�ͼ�����ʧ����
***************************************************************************************/
sOneImg CImgSerial::Dcm2MatGroup(CString lpszPathName)
{
	sOneImg mg;																//����һ��MatGroup��
	int num = 0;
	DcmFileFormat fileformat;
	OFCondition oc = fileformat.loadFile((LPSTR)(LPCTSTR)lpszPathName);		//��ȡһ��dcm�ļ�
	if(!oc.good())															//�ж��ļ��Ƿ�򿪳ɹ�
	{
		AfxMessageBox("���ļ�ʧ��, ����������dicom�ļ���");						//�ļ���ʧ�ܺ��˳�
		((CMainFrame*)AfxGetMainWnd())->m_Log.WriteLog("%s",
			"���ļ�ʧ���ˣ��ļ�����������dicom�ļ�");
		return mg;
	}

	DcmDataset *mDataset = fileformat.getDataset();							//��ȡ���ݼ�
	E_TransferSyntax xfer = mDataset->getOriginalXfer();					//�����﷨

	const char *pStr;
	mg.info.isDisplay = TRUE;												//��ʾͼ����Ϣ
	mDataset->findAndGetString(DCM_PatientName, pStr);						//��������
	mg.info.sName = pStr;
	mDataset->findAndGetString(DCM_PatientID, pStr);						//����ID
	mg.info.sId = pStr;
	mDataset->findAndGetString(DCM_InstitutionName, pStr);
	mg.info.sInstitutionName  = pStr;										//��������
	mDataset->findAndGetString(DCM_PatientSex, pStr);
	mg.info.sSex  = pStr;													//�����Ա�
	mDataset->findAndGetString(DCM_ImagePositionPatient, pStr);
	mg.info.sImgPositionPatient = pStr;										//����ͼ��λ��
	mDataset->findAndGetString(DCM_SliceThickness, pStr);
	mg.info.slicesickness = CType::pChar2Double(pStr);						//CT���

	const char* thick;
	mDataset->findAndGetString(DCM_SliceLocation, thick);					//��λ
	mg.fSliceLocation = CType::pChar2Double(thick);
	
	mDataset->findAndGetString(DCM_PixelSpacing, thick);					//�������ļ�ļ��
	CString* pPixelSpacing = CImgInfoLayer::SplitString(thick, '\\', num);
	mg.fSliceSpacing = CType::CString2Double(pPixelSpacing[0]);

	//������Ƭ����X��Y��Z��������
	CString* pPosition = CImgInfoLayer::SplitString(mg.info.sImgPositionPatient,'\\', num);
	if (pPosition != NULL && num == 3)										//û�����ݸ�����ʱ��д��
	{
		mg.info.nPositionX = CType::CString2Double(pPosition[0]);
		mg.info.nPositionY = CType::CString2Double(pPosition[1]);
		mg.info.nPositionZ = CType::CString2Double(pPosition[2]);
	}
	else
		mg.info.nPositionX = mg.info.nPositionY = mg.info.nPositionZ = 0;

	//��ȡ���ݼ��еĴ���λ����
	mDataset->findAndGetString(DCM_WindowCenter, pStr);						//��ȡԭͼ���д�λ
	mg.info.sWindowCenter = pStr;											//���õ�������д�λ
	mDataset->findAndGetString(DCM_WindowWidth, pStr);						//��ȡԭͼ���д���
	mg.info.sWindowWidth = pStr;											//���õ�������д���

	const char* transferSyntax;
	fileformat.getMetaInfo()->findAndGetString(DCM_TransferSyntaxUID, transferSyntax);

	if(transferSyntax == losslessTransUID || transferSyntax == lossTransUID ||
		transferSyntax == losslessP14 || transferSyntax == lossyP1 || transferSyntax == lossyRLE)
	{
		AfxMessageBox("��֧��ͼ������ʽ��");
		return mg;
	}
	
	DicomImage *dcmImg = new DicomImage(lpszPathName);							//����·������dicomͼ��

	mg.pDcmimg = dcmImg;														//����dcmͼ��
	int width  = dcmImg->getWidth();											//��ȡͼ����
	int height = dcmImg->getHeight();											//��ȡͼ��߶�
	
	dcmImg->setWindow(-500, 1200);												//���ô���λ  �δ�
	Uint16 * pixel = (Uint16 *)(dcmImg->getOutputData(16));
	mg.src_u16 = Mat(width, height, CV_16UC1, pixel).clone();
	//ģ�崰
	dcmImg->setWindow(-318, 203);
	char* pimod = (char *)(dcmImg->getOutputData(8));							//����ָ��
	mg.masking = Mat(width, height, CV_8UC1, pimod).clone();					//��ʵ��ģ��
	//m_index = 1;																//��ͼ���룬��λ����һ����
	//delete dcmImg;															//��ɾ��ͼ�񣬺�������λ��Ҫʹ��
	mg.isGood = true;
	return mg;
}

/***************************************************************************************
Function: ����������һ��ͼƬ������������ƶ�
Input:    NULL
Output:   �����������ָ�Ĳ����������һ���򷵻���һ�ţ��������һ���򷵻ص�ǰ��
Description: ע�����е�end()���������е����һ�ţ������������һ�ŵ���һ��Ԫ��λ�ã��ǲ��ɷ��ʵ��ڴ���
Return:   MatGroup
***************************************************************************************/
sOneImg CImgSerial::NextMatImg()
{
	if (++m_itr != m_vImgSerial.end())
	{
		++m_index;																//ͼ�������ݼ�
		return *m_itr;
	}
	else
	{
		MessageBox(NULL,"�Ѿ������һ��ͼƬ�ˣ�","RBDcm��ʾ��",MB_OK);
		return *(--m_itr);
	}
}

/***************************************************************************************
Function: ����������һ��ͼƬ������������ƶ�
Input:    NULL
Output:   �����������ָ�Ĳ������е�һ���򷵻���һ�ţ����ǵ�һ���򷵻ص�ǰ��
Return:   MatGroup
***************************************************************************************/
sOneImg CImgSerial::LastMatImg()
{
	if (m_itr != m_vImgSerial.begin())
	{
		m_itr--;
		--m_index;			//ͼ�������ݼ�
		return *m_itr;
	}
	else
	{
		MessageBox(NULL,"�Ѿ��ǵ�һ��ͼƬ�ˣ�","RBDcm��ʾ��",MB_OK);
		return *(m_itr);
	}
}


/***************************************************************************************
Function: ����Xml�ļ�
Input:    �ļ�·��+�ļ���
***************************************************************************************/
BOOL CImgSerial::LoadXml(CString lpszPathName)
{
	const char* pFile = (LPSTR)(LPCTSTR)lpszPathName;						//XML�ļ�·��
	m_vSingle.clear();														//����ǰ�������Ϣ
	TiXmlDocument doc(pFile);												//������ȡXML��ʱ����
	BOOL loadOK = doc.LoadFile();											//����XML�ļ�
	if (!loadOK)															//����ʧ�ܵ�����ʾ���˳��ú���
	{
		MessageBox(NULL, "xml�ļ���ȡʧ�ܣ�", "RBDcm��ʾ��", MB_OK);
		return FALSE;
	}

	TiXmlElement* root = doc.RootElement();									//XML�ĸ��ڵ�
	for (TiXmlNode* SpecialistItem = root->FirstChild("readingSession");    //��readingSession�ڵ����ѭ��
		SpecialistItem; SpecialistItem = SpecialistItem->NextSibling("readingSession"))
	{
		TiXmlNode* unblindedReadNodule = SpecialistItem->FirstChild("unblindedReadNodule");		//���
		TiXmlNode* nonNodule = SpecialistItem->FirstChild("nonNodule");							//�ǽ��

		while (unblindedReadNodule)								//�ڵ�unblindedReadNoduleѭ��
		{
			TiXmlNode* roi = unblindedReadNodule->FirstChild("roi");
			while (roi)											//�ڵ�roiѭ��
			{
				SingleImgNodule single;
				TiXmlNode* imageZposition = roi->FirstChild("imageZposition");	 //imageZposition��Ϣ
				const char* Zposition = imageZposition->ToElement()->GetText();
				single.ZPosition = CType::pChar2Double(Zposition);
				TiXmlNode* edgeMap = roi->FirstChild("edgeMap");
				while (edgeMap)													//edgeMap�ڵ�ѭ��
				{
					TiXmlNode* xCoord = edgeMap->FirstChild("xCoord");
					TiXmlNode* yCoord = edgeMap->FirstChild("yCoord");
					const char* szX = xCoord->ToElement()->GetText();
					const char* szY = yCoord->ToElement()->GetText();
					NodulePoint pt;
					pt.nt = ISNODULE;
					pt.x = CType::pChar2int(szX);
					pt.y = CType::pChar2int(szY);
					single.vcNodulePoint.push_back(pt);
					edgeMap = edgeMap->NextSibling("edgeMap");
				}
				m_vSingle.push_back(single);
				roi = roi->NextSibling("roi");
			}
			unblindedReadNodule = unblindedReadNodule->NextSibling("unblindedReadNodule");
		}

		while (nonNodule)
		{
			TiXmlNode* roi = nonNodule->FirstChild("roi");
			while (roi)
			{
				SingleImgNodule single;
				TiXmlNode* imageZposition = roi->FirstChild("imageZposition");		//��ȡ�����µĽڵ�
				const char* Zposition = imageZposition->ToElement()->GetText();		//zPosition����ֵ
				single.ZPosition = CType::pChar2Double(Zposition);
				TiXmlNode* edgeMap = roi->FirstChild("edgeMap");
				while (edgeMap)
				{
					TiXmlNode* xCoord = edgeMap->FirstChild("xCoord");
					TiXmlNode* yCoord = edgeMap->FirstChild("yCoord");
					const char* szX = xCoord->ToElement()->GetText();
					const char* szY = yCoord->ToElement()->GetText();
					NodulePoint pt;
					pt.nt = NONENODULE;
					pt.x = CType::pChar2int(szX);
					pt.y = CType::pChar2int(szY);
					single.vcNodulePoint.push_back(pt);
					edgeMap = edgeMap->NextSibling("edgeMap");
				}
				//��ŵ�������
				m_vSingle.push_back(single);
				roi = roi->NextSibling("roi");
			}
			nonNodule = nonNodule->NextSibling("nonNodule");
		}
	}

	//������ɵ���Ϣ��ԭ���ݽ��й���
	list<sOneImg>::iterator itr = m_vImgSerial.begin();
	int count = 1;
	for (;itr != m_vImgSerial.end(); ++count)
	{
		double dbTmp = itr->info.nPositionZ;
		list<SingleImgNodule>::iterator itrSg = m_vSingle.begin();			//xml������Ϣ
 		for (; itrSg != m_vSingle.end(); ++itrSg)							//�ڽ����ı��в���ƥ���Ȼ����
		{
			double tmp = itrSg->ZPosition;
			if (fabs(tmp - dbTmp) > 0.1)
			{
				itr->pSingleNodules = NULL;									//û�������Ϣʱ�����ָ��
				++count;
				continue;
			}
			itr->pSingleNodules = &(*itrSg);								//���д���˽����Ϣ��ֱ�ӽ������ֲ�ѯ
			break;
		}
		++itr;
	}
	return TRUE;
}



/*===========================================================================*
* �ָ�ͼ�����У��õ���ʵ���ɰ�
*============================================================================*/
int CImgSerial::SegSeries()
{
	list<sOneImg>::iterator itr = m_vImgSerial.begin();							//
	clock_t start, finish;														//
	start = clock();
	cv::Rect rect;
	Mat mt = getStructuringElement(MORPH_ELLIPSE, Size(7, 7), Point(3, 3));		//7*7Բ��ģ��
	Mat mmt = getStructuringElement(MORPH_ELLIPSE, Size(3, 3), Point(1, 1));	//3*3Բ��ģ��
	Mat msk;																		
	double sum = (double)m_vImgSerial.size();
	for (int i=1; itr != m_vImgSerial.end(); ++itr, ++i)						//���ÿһ֡ͼ����зָ�
	{
		cntr.clear(); hy.clear();												//����߽�
		msk = itr->masking.clone();
		bitwise_and(Global::maskcicle, msk, msk);
		floodFill(msk, cv::Point(0, 0), Scalar(255), &rect, 5, 5);
		threshold(msk, msk, 254, 255, 0);
		morphologyEx(msk, msk, MORPH_CLOSE, mt);								//�����㣬ȥ��CT��
		bitwise_not(msk, msk);
		findContours(msk, cntr, hy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);

		Mat tmp = Mat::zeros(msk.size(), CV_8UC1);
		if(!cntr.empty() && !hy.empty())
		{
			for (unsigned int idx = 0; idx < cntr.size(); idx++)
			{
				if(contourArea(cntr[idx]) < Global::LUNGAREA) continue;			//�������ɸѡ
				drawContours(tmp, cntr, idx, Scalar(255), CV_FILLED, 8, hy);
			}
		}
		erode(tmp, tmp, mmt);
		
		morphologyEx(tmp, tmp, MORPH_OPEN, mt);
		/*
		//2016.12.2���÷ֶ��޲��ķ�ʽ
		if ((i/sum < 0.1) || (i/sum > 0.9))
			;
		else if ((i/sum < 0.2) || (i/sum > 0.75))									//����ҳ�ͷ���ҳ
			tmp = ConcavityRepair(tmp, 20, 5);
		else
			tmp = ConcavityRepair(tmp);												//�߽��޲�
		*/
		itr->masking = tmp;
	}
	finish = clock();
	return (finish - start);
}

/*========================================================================================
* ��ȡʸ״��
==========================================================================================*/
Mat CImgSerial::GetSagittal(int col, int height)
{
	list<sOneImg>::iterator itr = m_vImgSerial.begin();
	Mat dst;
	for (; itr != m_vImgSerial.end(); ++itr)
	{
		Mat tmp = Global::Mask16(itr->src_u16, itr->masking).clone();
		dst.push_back(tmp.rowRange(col, col+1));
	}
	Mat rt;
	cv::resize(dst,rt,cv::Size(dst.cols, height),INTER_CUBIC);
	return rt.clone();
}


/*========================================================================================
* ��ȡ��״��
==========================================================================================*/
Mat CImgSerial::GetCoronal(int row, int height)
{
	list<sOneImg>::iterator itr = m_vImgSerial.begin();
	Mat dst;
	for (; itr != m_vImgSerial.end(); ++itr)
	{
		Mat tmp = Global::Mask16(itr->src_u16, itr->masking).clone();
		tmp = tmp.colRange(row, row + 1);
		tmp = tmp.t();
		dst.push_back(tmp);
	}
	Mat rt;
	cv::resize(dst, rt, cv::Size(dst.cols, height), INTER_CUBIC);
	return rt.clone();
}


//����ͼ�����Ƭλ������
void CImgSerial::SortImg(void)
{
	m_vImgSerial.sort(CompareRules);			//�������������
	m_itr = m_vImgSerial.begin();				//�������������ʧЧ�����»�ȡ
}


//����������ͼ����з��������ֽ�ڵ�
void CImgSerial::AnalyseImg(void)
{
	list<sOneImg>::iterator ir = m_vImgSerial.begin();
	vector<vector<Point> > contours;
	//Mat tmp;
	for (; ir != m_vImgSerial.end(); ++ir)													//�������е�ÿһ��ͼ��
	{
		cv::findContours(ir->matSel, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
		for (unsigned int i = 0; i<contours.size(); ++i)
		{
			//����һ������е���������
			cv::Rect rect = boundingRect(contours.at(i));									//��Ӿ�������
			for(int nx = rect.x; nx < (rect.x + rect.width); ++nx)
			{
				for (int ny = rect.y; ny < (rect.y + rect.height); ++ny)
				{
					if (ir->matSel.at<uchar>(ny, nx) == 0)
						continue;
					list<sOneImg>::iterator itr = ir;
					itr++;
					while(itr != m_vImgSerial.end() && itr->matSel.at<uchar>(ny, nx) == 255)
					{
						cv::Rect rt;
						Mat img = itr->matSel;
						floodFill(itr->matSel, Point(nx, ny), 0, &rt, 5, 5);				//��ˮ���
						floodFill(itr->masking, Point(nx, ny), 0, &rt, 5, 5);				//��ԭͼ����������
						itr++;
					}
				}
			}
		}
		contours.clear();
	}
}


void CImgSerial::DealContours(void)
{
}


/*========================================================================================
* ���������ļ��ξ���
==========================================================================================*/
int dist(const Point& x, const Point& y)
{
	return (x.x - y.x) * (x.x - y.x) + (x.y - y.y) * (x.y - y.y);
}


/*========================================================================================
* �޲�ͼ��߽�, �����ֵͼ��
==========================================================================================*/
Mat CImgSerial::ConcavityRepair(Mat& Im, int hullgap, int distgap)
{
	vector<Vec4i> hy;															//�߽�㼯��
	vector<vector<Point> > cntr;												//�߽��
	findContours(Im, cntr, hy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0));	//���ұ߽�
	vector<vector<Point> > hull(cntr.size());									//͹���㼯��
	for (int i = 0; i < cntr.size(); i++)										//����͹��
		convexHull(Mat(cntr[i]), hull[i], false);
	Mat draw = Mat::zeros(Im.size(), CV_8UC1);									//���չʾ

	for (int i = 0; i < cntr.size(); i++)
	{
		drawContours(draw,cntr,i,Scalar(255),1,8,vector<Vec4i>(),0,Point());	//����ԭ�߽�
		//1.����͹������޲���߽�
		for (int k = 0; k < hull[i].size()-1; k++)
		{
			if (abs(hull[i][k].x - hull[i][k+1].x) + abs(hull[i][k].y - hull[i][k+1].y) < hullgap)
				line(draw, hull[i][k], hull[i][k+1], Scalar(255), 1);			//����
		}
		//2.�ٽ����޲��ڱ߽�
		for (int k = 0; k < cntr[i].size(); k++)
		{
			for (int l = k+1; l < cntr[i].size(); l++)
			{
				if (dist(cntr[i][k], cntr[i][l]) < distgap)
					line(draw, cntr[i][k], cntr[i][l], Scalar(255), 1);			//����
			}
		}
	}
	cntr.clear(); hy.clear();													//����߽�
	findContours(draw, cntr, hy, CV_RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);		//���ұ߽�
	drawContours(draw, cntr, -1, Scalar(255), CV_FILLED);						//���߽�
	return draw;
}


/*========================================================================================
* ���е�ƽ���ܶ�ͶӰ�㷨��ԭ����û5�Ž���һ�ε���
==========================================================================================*/
void CImgSerial::ImgAIP(int num)
{
	int count = 1;	
	list<sOneImg>::iterator it = m_vImgSerial.begin();							//������������
	Mat tp = Global::Mask(it->src_u16.clone(),it->masking).clone();
	tp /= num;
	for (it++; it != m_vImgSerial.end();it++ ,count++)
	{
		if (count % num != 0)													//��δ����num�ţ������
			tp += Global::Mask(it->src_u16.clone(),it->masking).clone()/num;
		else
		{
			m_aip.push_back(tp);												//����num�Ž�ͼ����б���
			tp = Global::Mask(it->src_u16.clone(),it->masking).clone();
			tp /= num;
		}
	}
	if (count%num != 0)
	{
		tp = tp * num / (count % num);
		m_aip.push_back(tp);
	}
}

/*========================================================================================
* ���е�ƽ���ܶ�ͶӰ��ģ��
==========================================================================================*/
void CImgSerial::mskAIP(int num)
{
	int count = 1;	
	list<sOneImg>::iterator it = m_vImgSerial.begin();							//������������
	Mat tp = it->masking.clone();
	tp /= num;
	for (it++; it != m_vImgSerial.end();it++ ,count++)
	{
		if (count % num != 0)													//��δ�������ţ������
			bitwise_or(tp, it->masking, tp);
		else
		{
			m_mskaip.push_back(tp);												//����num�Ž�ͼ����б���
			tp = it->masking.clone();
		}
	}
	if (count%num != 0)
	{
		m_mskaip.push_back(tp);
	}
}

