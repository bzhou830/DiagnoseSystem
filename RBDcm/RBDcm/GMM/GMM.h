#pragma once
#include <fstream>

class GMM
{
public:
	GMM(int dimNum = 1, int mixNum = 1);
	~GMM();

	void SetMaxIterNum(int i)	{ m_maxIterNum = i; }
	void SetEndError(double f)	{ m_endError = f; }

	int GetDimNum()			{ return m_dimNum; }
	int GetMixNum()			{ return m_mixNum; }
	int GetMaxIterNum()		{ return m_maxIterNum; }
	double GetEndError()	{ return m_endError; }

	double& Prior(int i)	{ return m_priors[i]; }
	double* Mean(int i)		{ return m_means[i]; }
	double* Variance(int i)	{ return m_vars[i]; }

	void setPrior(int i,double val)	{  m_priors[i]=val; }
	void setMean(int i,double *val)		{ for(int j=0;j<m_dimNum;j++) m_means[i][j]=val[j]; }
	void setVariance(int i,double *val)	{ for(int j=0;j<m_dimNum;j++) m_vars[i][j]=val[j]; }

	double GetProbability(const double* sample);

	void Init(double *data, int N);
	void Train(double *data, int N);

	friend std::ostream& operator<<(std::ostream& out, GMM& gmm);
	//friend std::istream& operator>>(std::istream& in, GMM& gmm);

	double* m_priors;								// ��˹Ȩ��ϵ��
	double** m_means;								// ��˹��ֵ
	double** m_vars;								// ��˹����
private:
	int m_dimNum;									// ����ά��
	int m_mixNum;									// ��˹��ģ����Ŀ

	// A minimum variance is required. Now, it is the overall variance * 0.01.
	double* m_minVars;
	int m_maxIterNum;								// ����������
	double m_endError;								// ֹͣ������С���

private:
	// Return the "j"th pdf, p(x|j).
	double GetProbability(const double* x, int j);
	void Allocate();
	void Dispose();
};
