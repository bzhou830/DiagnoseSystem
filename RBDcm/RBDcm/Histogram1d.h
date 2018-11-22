#pragma once
class Histogram1d
{
private:
	int histSize[1];
	float hranges[2];
	const float *ranges[1];
	int channels[1];
public:
	Histogram1d(void)
	{
		histSize[0] = 256;
		hranges[0] = 0.0;
		hranges[1] = 255.0;
		ranges[0] = hranges;
		channels[0] = 0;
	}
	~Histogram1d(void);
	cv::MatND getHistogram(const cv::Mat &image)
	{
		cv::MatND hist;
		cv::calcHist(&image, 1, channels, cv::Mat(), hist, 1, histSize, ranges);
		return hist;
	}

	cv::Mat getHistogramImage(const cv::Mat &image)
	{
		cv::MatND hist = getHistogram(image);
		hist.at<float>(0) = 0;
		double maxval=0,minval=0;
		cv::minMaxLoc(hist, &minval, &maxval);
		cv::Mat histImg(histSize[0], histSize[0], CV_8U, cv::Scalar(255));
		int hpt = static_cast<int>(0.9 * histSize[0]);
		//maxval = 255;
		for(int h=1; h<histSize[0]; h++)
		{
			float binVal = hist.at<float>(h);
			int intensity = static_cast<int>(binVal*hpt/maxval);
			cv::line(histImg,cv::Point(h,histSize[0]),cv::Point(h,histSize[0] - intensity),cv::Scalar::all(0));
		}
		return histImg;
	}

};

