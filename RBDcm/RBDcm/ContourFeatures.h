#pragma once

//�Ҷȹ���������
const int HORIZON  = 1;
const int VERTICAL = 2;
const int ANGLE45  = 4;
const int ANGLE135 = 8;


//���������ص�������Ϣ
class CContourFeatures
{
public:
	CContourFeatures(void);					
	~CContourFeatures(void);
public:
	double m_fGrayMean;						//�ҶȾ�ֵ
	double m_fVariance;						//�Ҷȷ���
	double m_fArea;							//���
	double m_fCircumference;				//�ܳ�
	float  m_fDiameter;						//ֱ��
	double m_fSameCircle;					//Բ�ƶ�
	double m_fFlaten;						//���
	double m_fCompactedness;				//������
	double m_fConcavity;					//������
	double m_fSameRect;						//���ζ�
	double m_fEnergy;						//����
	double m_fIdm;							//����
	double m_fCon;							//�Աȶ�
	double m_fEntropy;						//��
	cv::Point2f m_ptPosition;				//λ��
public:
	cv::Mat calGLCM(cv::Mat src, int mode, int step = 2);
	void CalTextTure(cv::Mat src);			//���������ĸ�����
};

