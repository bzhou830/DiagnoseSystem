#include "StdAfx.h"
#include "Global.h"

Mat Global::maskcicle = Mat(Size(512,512), CV_8UC1, Scalar(0));			//包含肺区的圆形
int Global::LUNGAREA = 800;												//肺实质区的最小面积
CIniReader Global::g_Ini("./rbini.ini");
bool Global::isDumpCsv = false;
int Global::ThresholdNum = 0;

Global::Global(void)
{
}

Global::~Global(void)
{
}

void Global::InitStatics()
{
	//圆形模板
	circle(maskcicle, Point(255, 255), 240, Scalar(255), -1);
	//rectangle(maskcicle,Point(250, 45), Point(255, 510), Scalar(0), -1);
	//rectangle(maskcicle,Point(250, 1), Point(255, 60), Scalar(0), -1);
	isDumpCsv = g_Ini.ReadBoolean("DUMPFILE", "ImgData", false);
}

bool Global::GetFolder(std::string& folderpath, const char* szCaption, HWND hOwner)   
{   
	bool retVal = false; 
	BROWSEINFO bi;   
	memset(&bi, 0, sizeof(bi));   
	bi.ulFlags   = BIF_USENEWUI;   
	bi.hwndOwner = hOwner;   
	bi.lpszTitle = szCaption;   
	::OleInitialize(NULL);   
	LPITEMIDLIST pIDL = ::SHBrowseForFolder(&bi);   
	if(pIDL != NULL)   
	{   
		char buffer[_MAX_PATH] = {'\0'};   
		if(::SHGetPathFromIDList(pIDL, buffer) != 0)   
		{   
			folderpath = buffer;   
			retVal = true;   
		}          
		CoTaskMemFree(pIDL);   
	}   
	::OleUninitialize();   
	return retVal;   
}

Mat Global::u16Tou8(const Mat& src)
{
	Mat dst(src.size(),CV_8UC1);
	for (int i = 0; i < src.rows; i++)
		for (int j = 0; j < src.cols; j++)
			dst.at<uchar>(i,j) = (src.at<Uint16>(i, j)) >> 8;
	return dst;
}

Mat Global::Mask(Mat& src, Mat &mask)
{
	Mat dst = Mat::zeros(src.size(), CV_8UC1);
	for (int i = 0; i < src.rows; i++)
		for (int j = 0; j < src.cols; j++)
			if (mask.at<uchar>(i,j) != 0)
				dst.at<uchar>(i,j) = (src.at<Uint16>(i, j)) >> 8;
	return dst;
}


Mat Global::Mask16(Mat& src, Mat &mask)
{
	Mat dst = Mat::zeros(src.size(), CV_16UC1);
	for (int j=0;j<dst.rows;j++)
	{
		unsigned short* data = src.ptr<unsigned short>(j);
		unsigned short* pOut = dst.ptr<unsigned short>(j);
		uchar* pMask = mask.ptr<uchar>(j);
		for (int i=0;i<dst.cols;i++)
		{
			if (pMask[i] != 0)
				pOut[i] = data[i];
		}
	}
	return dst;
}

/*========================================================================================
* g1[0-2] 系数， 均值， 方差
* g2[0-2] 系数， 均值， 方差
==========================================================================================*/
int Global::GuassAcross(double g1[3], double g2[3])
{
	double sigma1 = sqrtf(g1[2]);												//开方
	double sigma2 = sqrtf(g2[2]);												
	double t1;
	double t2;
	int index = 60;
	double gap = 0;
	double max_gap = 0;
	for (int i=60; i<90; i++)
	{
		t1 = GetCoeff(sigma1, g1[0]) * GetExp(sigma1, g1[1], i);
		t2 = GetCoeff(sigma2, g2[0]) * GetExp(sigma2, g2[1], i);
		gap = t1 - t2;
		if (abs(gap) > max_gap)
		{
			max_gap = abs(gap);
			index = i;
		}
	}
	return index;
}

/*========================================================================================
* 1 / sqrt(2*pi) = 0.398947
* 乘以10000倍
======================================================== ==================================*/
double Global::GetCoeff(double sigma, double coeff)
{
	double rt = ((3989.47 * coeff) / sigma);
	return rt;
}

/*========================================================================================
* -(x-miu)^2 / 2 * sigma^2
==========================================================================================*/
double Global::GetExp(double sigma, double mean, int x)
{
	double index = -(x - mean) * (x-mean) * 0.5 / (sigma*sigma); 
	double rt = exp(index);
	return rt;
}

BOOL Global::FolderExists(CString s)  
{   
	DWORD attr;    
	attr = GetFileAttributes(s);    
	return (attr != (DWORD)(-1) ) &&   
		( attr & FILE_ATTRIBUTE_DIRECTORY);    
}

BOOL Global::CreateMuliteDirectory(CString P)
{   
	int len=P.GetLength();   
	if ( len <2 ) return false;    
	if('\\'==P[len-1])  
	{  
		P=P.Left(len-1);  
		len=P.GetLength();  
	}  
	if ( len <=0 ) return false;  
	if (len <=3)   
	{  
		if (FolderExists(P))return true;  
		else return false;   
	}  
	if (FolderExists(P))return true;  
	CString Parent;  
	Parent=P.Left(P.ReverseFind('\\') );   
	if(Parent.GetLength()<=0)return false;    
	BOOL Ret=CreateMuliteDirectory(Parent);    
	if(Ret)    
	{   
		SECURITY_ATTRIBUTES sa;   
		sa.nLength=sizeof(SECURITY_ATTRIBUTES);   
		sa.lpSecurityDescriptor=NULL;   
		sa.bInheritHandle=0;   
		Ret=(CreateDirectory(P,&sa)==TRUE);   
		return Ret;   
	}   
	else  
		return FALSE;   
}
