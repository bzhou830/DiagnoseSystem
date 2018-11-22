#pragma once
/*全局量和全局对象*/

#include "Common/ini.h"

class Global
{
public:
	Global(void);
	~Global(void);
	static Mat maskcicle;
	static int ThresholdNum;
	static int LUNGAREA;
	static void InitStatics();
	static bool GetFolder(std::string& folderpath, const char* szCaption = NULL, HWND hOwner = NULL);
	static Mat u16Tou8(const Mat& src);
	static Mat Mask(Mat& src, Mat &mask);
	static Mat Mask16(Mat& src, Mat &mask);
	static int GuassAcross(double g1[3], double g2[3]);
	static bool isDumpCsv;
	static BOOL FolderExists(CString s);
	static BOOL CreateMuliteDirectory(CString P);
private:
	static double GetCoeff(double sigma, double coeff);
	static double GetExp(double sigma, double mean, int x);
	static CIniReader g_Ini;
};

