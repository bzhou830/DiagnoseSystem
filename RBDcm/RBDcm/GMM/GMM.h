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

	double* m_priors;								// 高斯权重系数
	double** m_means;								// 高斯均值
	double** m_vars;								// 高斯方差
private:
	int m_dimNum;									// 样本维数
	int m_mixNum;									// 高斯单模型数目

	// A minimum variance is required. Now, it is the overall variance * 0.01.
	double* m_minVars;
	int m_maxIterNum;								// 最大迭代次数
	double m_endError;								// 停止迭代最小误差

private:
	// Return the "j"th pdf, p(x|j).
	double GetProbability(const double* x, int j);
	void Allocate();
	void Dispose();
};
