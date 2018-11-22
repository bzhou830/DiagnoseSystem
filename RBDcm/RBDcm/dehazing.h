#include <cv.h>
#include <highgui.h>
#include <omp.h>
#include <emmintrin.h>
#include "windows.h"


#define CLIP(x) ((x)<(0)?0:((x)>(255)?(255):(x)))
#define CLIP_Z(x) ((x)<(0)?0:((x)>(1.0f)?(1.0f):(x)))
#define CLIP_TRS(x) ((x)<(0.1f)?0.1f:((x)>(1.0f)?(1.0f):(x)))

using namespace std;

class dehazing
{
public:
	dehazing();
	dehazing(int nW, int nH, bool bPrevFlag, bool bPosFlag);
	dehazing(int nW, int nH, int nTBlockSize, bool bPrevFlag, bool bPosFlag, float fL1, float fL2, int nGBlock);
	~dehazing(void);
	
	void	HazeRemoval(IplImage* imInput, IplImage* imOutput, int nFrame);
	void	ImageHazeRemoval(IplImage* imInput, IplImage* imOutput);	
	void	LambdaSetting(float fLambdaLoss, float fLambdaTemp);
	void	DecisionUse(bool bChoice);
	void	TransBlockSize(int nBlockSize);
	void	FilterBlockSize(int nBlockSize);
	void	AirlightSerachRange(POINT pointTopLeft, POINT pointBottomRight);
	void	SetFilterStepSize(int nStepsize);
	void	PreviousFlag(bool bPrevFlag);
	void	FilterSigma(float nSigma);
	bool	Decision(IplImage* imInput, IplImage* imOutput, int nThreshold);

	int*	GetAirlight();
	int*	GetYImg();
	float*	GetTransmission();
	
private:

	//320*240 size
	float*	m_pfSmallY;			//Y image
	float*	m_pfSmallPk_p;		//(Y image) - (mean of Y image)
	float*	m_pfSmallNormPk;	//Normalize된 Y image
	float*	m_pfSmallInteg;		//Gaussian weight가 적용된 transmission 결과
	float*	m_pfSmallDenom;		//Gaussian weight가 저장된 행렬

	int*	m_pnSmallYImg;		//입력 영상의 Y채널

	int*	m_pnSmallRImg;		//입력 영상의 R채널
	int*	m_pnSmallGImg;		//입력 영상의 G채널
	int*	m_pnSmallBImg;		//입력 영상의 B채널

	float*	m_pfSmallTransP;	//이전 프레임의 transmission 영상
	float*	m_pfSmallTrans;		//초기 transmission 영상
	float*	m_pfSmallTransR;	//정련된 transmission 영상
	int*	m_pnSmallYImgP;		//이전 프레임의 Y채널

	int*	m_pnSmallRImgP;		//이전 프레임의 Y채널
	int*	m_pnSmallGImgP;		//이전 프레임의 Y채널
	int*	m_pnSmallBImgP;		//이전 프레임의 Y채널

	//Original size
	float*	m_pfY;				//Y image
	float*	m_pfPk_p;			//(Y image) - (mean of Y image)
	float*	m_pfNormPk;			//Normalize된 Y image
	float*	m_pfInteg;			//Gaussian weight가 적용된 transmission 결과
	float*	m_pfDenom;			//Gaussian weight가 저장된 행렬
	
	int*	m_pnYImg;			//입력 영상의 Y채널
	int*	m_pnYImgP;			//입력 영상의 Y채널

	int*	m_pnRImg;			//입력 영상의 Y채널
	int*	m_pnGImg;			//입력 영상의 Y채널
	int*	m_pnBImg;			//입력 영상의 Y채널

	int*	m_pnRImgP;			//입력 영상의 Y채널
	int*	m_pnGImgP;			//입력 영상의 Y채널
	int*	m_pnBImgP;			//입력 영상의 Y채널

	float*	m_pfTransmission;	//초기 transmission
	float*	m_pfTransmissionP;	//초기 transmission
	float*	m_pfTransmissionR;	//정련된 transmission 영상
	
	//////////////////////////////////////////////////////////////////////////
	int		m_nStepSize;		//Guided filter의 step size;
	float*	m_pfGuidedLUT;		//Guided filter 내의 gaussian weight를 위한 LUT
	float	m_fGSigma;			//Guided filter 내의 gaussian weight에 대한 sigma

	int		m_anAirlight[3];	// atmospheric light value
	uchar	m_pucGammaLUT[256];	//감마 보정을 위한 LUT
	float	m_pfExpLUT[256];	//Transmission 계산시, 픽셀 차이에 대한 weight용 LUT

	int		m_nAirlight;		//안개값(grey)
	
	bool	m_bPreviousFlag;	//이전 프레임 이용 여부
	float	m_fLambda1;			//Loss cost
	float	m_fLambda2;			//Temporal cost

	int		m_nWid;				//너비
	int		m_nHei;				//높이

	int		m_nTBlockSize;		// Block size for transmission estimation
	int		m_nGBlockSize;		// Block size for guided filter

	//Airlight search range
	int		m_nTopLeftX;				
	int		m_nTopLeftY;
	int		m_nBottomRightX;
	int		m_nBottomRightY;

	bool	m_bPostFlag;		// Flag for post processing(deblocking)
	// function.cpp
	
	void	DownsampleImage();
	void	DownsampleImageColor();
	void	UpsampleTransmission();
	void	MakeExpLUT();
	void	GuideLUTMaker();
	void	GammaLUTMaker(float fParameter);
	void	IplImageToInt(IplImage* imInput);
	void	IplImageToIntColor(IplImage* imInput);
	void	IplImageToIntYUV(IplImage* imInput);
	
	// dehazing.cpp
	void	AirlightEstimation(IplImage* imInput);
	void	RestoreImage(IplImage* imInput, IplImage* imOutput);
	void	PostProcessing(IplImage* imInput, IplImage* imOutput);

	// TransmissionRefinement.cpp
	void	TransmissionEstimation(int *pnImageY, float *pfTransmission, int *pnImageYP, float *pfTransmissionP, int nFrame, int nWid, int nHei);
	void	TransmissionEstimationColor(int *pnImageR, int *pnImageG, int *pnImageB, float *pfTransmission,int *pnImageRP, int *pnImageGP, int *pnImageBP, float *pfTransmissionP,int nFrame, int nWid, int nHei);

	float	NFTrsEstimation(int *pnImageY, int nStartX, int nStartY, int nWid, int nHei);
	float	NFTrsEstimationP(int *pnImageY, int *pnImageYP, float *pfTransmissionP, int nStartX, int nStartY, int nWid, int nHei);

	float	NFTrsEstimationColor(int *pnImageR, int *pnImageG, int *pnImageB, int nStartX, int nStartY, int nWid, int nHei);
	float	NFTrsEstimationPColor(int *pnImageR, int *pnImageG, int *pnImageB, int *pnImageRP, int *pnImageGP, int *pnImageBP, float *pfTransmissionP, int nStartX, int nStartY, int nWid, int nHei);	
	
	// guided filter.cpp
	void	CalcAcoeff(float* pfSigma, float* pfCov, float* pfA1, float* pfA2, float* pfA3, int nIdx);
	void	BoxFilter(float* pfInArray, int nR, int nWid, int nHei, float*& fOutArray);
	void	BoxFilter(float* pfInArray1, float* pfInArray2, float* pfInArray3, int nR, int m_nWid, int m_nHei, float*& pfOutArray1, float*& pfOutArray2, float*& pfOutArray3);

	void	GuidedFilterY(int nW, int nH, float fEps);
	void	GuidedFilter(int nW, int nH, float fEps);
	void	GuidedFilterShiftableWindow(float fEps);
	
	void	FastGuidedFilterS();
	void	FastGuidedFilter();

};


